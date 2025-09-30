#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>

#define PORT "12345"
#define HEARTBEAT_INTERVAL 1
#define TIMEOUT 3

typedef struct Node {
    char ip[INET6_ADDRSTRLEN];
    int32_t pid;
    time_t last_seen;
    struct Node* next;
} Node;

Node* head = NULL;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
int32_t sockfd;
struct sockaddr_storage group_addr;
socklen_t group_addr_len;
int32_t my_pid;

int8_t list_changed = 0;

void print_list() {
  Node* current = head;
  printf("Active nodes: ");

  if (!current) {
    printf("(none)");
  }

  while (current != NULL) {
    printf("%s:%d ", current->ip, current->pid);
    current = current->next;
  }

  printf("\n");
}

int8_t add_or_update_node(const char* ip, int32_t pid) {
  Node* current;
  Node* new_node;
  int8_t found;

  found = 0;

  pthread_mutex_lock(&list_mutex);

  current = head;
  while (current != NULL) {

    if (strcmp(current->ip, ip) == 0 && current->pid == pid) {
      current->last_seen = time(NULL);
      found = 1;
      break;
    }

    current = current->next;
  }

  if (!found) {

    new_node = (Node*)malloc(sizeof(Node));

    if (!new_node) {
      pthread_mutex_unlock(&list_mutex);
      return 0;
    }

    strcpy(new_node->ip, ip);
    new_node->pid = pid;
    new_node->last_seen = time(NULL);
    new_node->next = head;
    head = new_node;
    printf("New node detected: %s (PID: %d)\n", ip, pid);
    list_changed = 1;
  }

  pthread_mutex_unlock(&list_mutex);
  return !found;
}

int8_t cleanup_nodes() {
  time_t now = time(NULL);
  Node** current;
  Node* temp;
  int8_t removed = 0;

  pthread_mutex_lock(&list_mutex);
  current = &head;

  while (*current != NULL) {

    if (now - (*current)->last_seen > TIMEOUT) {
      temp = *current;
      *current = (*current)->next;
      printf("Node timeout: %s (PID: %d)\n", temp->ip, temp->pid);
      free(temp);
      removed = 1;
      list_changed = 1;
    }

    else {
      current = &(*current)->next;
    }
  }

  pthread_mutex_unlock(&list_mutex);
  return removed;
}

void* heartbeat_sender(void* arg) {
  char message[64];
  int32_t message_len;
  unsigned char loop = 1;

  if (group_addr.ss_family == AF_INET) {
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
  }
  else {
    setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, &loop, sizeof(loop));
  }

  snprintf(message, sizeof(message), "HEARTBEAT:%d", my_pid);
  message_len = strlen(message);

  printf("Heartbeat sender started. My PID: %d\n", my_pid);

  while (1) {
    sendto(sockfd, message, message_len, 0, (struct sockaddr*)&group_addr, group_addr_len);
    sleep(HEARTBEAT_INTERVAL);
  }

  return NULL;
}

void* cleanuper(void* arg) {
  while(1) {
    sleep(1);
    cleanup_nodes();

    if (list_changed) {
      print_list();
      list_changed = 0;
    }
  }

  return NULL;
}

void* listener(void* arg) {
  char buf[1024];
  struct sockaddr_storage sender_addr;
  socklen_t sender_len;
  ssize_t numbytes;
  char ip_str[INET6_ADDRSTRLEN];
  int32_t received_pid;

  printf("Listener started\n");

  while (1) {
    sender_len = sizeof(sender_addr);
    numbytes = recvfrom(sockfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&sender_addr, &sender_len);

    if (numbytes > 0) {
      *(buf + numbytes) = '\0';

      if (strncmp(buf, "HEARTBEAT:", 10) == 0) {
        received_pid = atoi(buf + 10);

        if (received_pid == my_pid) {
          continue;
        }

        if (sender_addr.ss_family == AF_INET) {
          inet_ntop(AF_INET, &((struct sockaddr_in*)&sender_addr)->sin_addr, ip_str, sizeof(ip_str));
        }
        else {
          inet_ntop(AF_INET6, &((struct sockaddr_in6*)&sender_addr)->sin6_addr, ip_str, sizeof(ip_str));
        }

        add_or_update_node(ip_str, received_pid);
      }
    }
  }

  return NULL;
}

int32_t main(int32_t argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <multicast_address>\n", argv[0]);
    return -1;
  }

  my_pid = getpid();

  struct addrinfo hints, *res;
  int32_t reuse;
  struct sockaddr_storage bind_addr;
  socklen_t bind_len;
  struct sockaddr_in* ipv4;
  struct sockaddr_in6* ipv6;
  struct ip_mreq mreq;
  struct ipv6_mreq mreq6;
  pthread_t sender_thread, listener_thread, cleanup_thread;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_NUMERICHOST;

  if (getaddrinfo(argv[1], PORT, &hints, &res) != 0) {
    fprintf(stderr, "getaddrinfo failed\n");
    return -1;
  }

  memcpy(&group_addr, res->ai_addr, res->ai_addrlen);
  group_addr_len = res->ai_addrlen;

  sockfd = socket(res->ai_family, SOCK_DGRAM, 0);
  if (sockfd < 0) {
    fprintf(stderr, "socket\n");
    freeaddrinfo(res);
    return -1;
  }

  reuse = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    fprintf(stderr, "SO_REUSEADDR\n");
    close(sockfd);
    freeaddrinfo(res);
    return -1;
  }

  memset(&bind_addr, 0, sizeof(bind_addr));

  if (res->ai_family == AF_INET) {
    ipv4 = (struct sockaddr_in*)&bind_addr;
    ipv4->sin_family = AF_INET;
    ipv4->sin_addr.s_addr = INADDR_ANY;
    ipv4->sin_port = htons(12345);
    bind_len = sizeof(*ipv4);
  }
  else {
    ipv6 = (struct sockaddr_in6*)&bind_addr;
    ipv6->sin6_family = AF_INET6;
    ipv6->sin6_addr = in6addr_any;
    ipv6->sin6_port = htons(12345);
    bind_len = sizeof(*ipv6);
  }

  if (bind(sockfd, (struct sockaddr*)&bind_addr, bind_len) < 0) {
    fprintf(stderr, "bind\n");
    close(sockfd);
    freeaddrinfo(res);
    return -1;
  }

  if (res->ai_family == AF_INET) {

    mreq.imr_multiaddr = ((struct sockaddr_in*)&group_addr)->sin_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
 
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
      fprintf(stderr, "IP_ADD_MEMBERSHIP\n");
      close(sockfd);
      freeaddrinfo(res);
      return -1;
    }
  }

  else {

    mreq6.ipv6mr_multiaddr = ((struct sockaddr_in6*)&group_addr)->sin6_addr;
    mreq6.ipv6mr_interface = 0;
    if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &mreq6, sizeof(mreq6)) < 0) {
      fprintf(stderr, "IPV6_ADD_MEMBERSHIP\n");
      close(sockfd);
      freeaddrinfo(res);
      return -1;
    }
  }

  freeaddrinfo(res);

  printf("Multicast client started. Group: %s, My PID: %d\n", *(argv +1), my_pid);

  pthread_create(&sender_thread, NULL, heartbeat_sender, NULL);
  pthread_create(&listener_thread, NULL, listener, NULL);
  pthread_create(&cleanup_thread, NULL, cleanuper, NULL);

  pthread_join(sender_thread, NULL);
  pthread_join(listener_thread, NULL);
  pthread_join(cleanup_thread, NULL);

  close(sockfd);
  return 0;
}
