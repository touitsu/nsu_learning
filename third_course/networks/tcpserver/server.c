#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <locale.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#define BUFFSIZE 4096

#if BUFFSIZE < 8
  #error "BUFFSIZE must be at least 8"
#endif

typedef struct client {
  int32_t client_sock;
  struct sockaddr_in client_addr;
} client_t;

static inline const char* get_filename(const char* path) {
  return strrchr(path, '/') == NULL ? path : strrchr(path, '/') + 1;
}

void* client_handler(void* arg) {
  client_t* client_info;
  int32_t port;
  int32_t client_sock, filename_size;
  uint32_t tmp;
  uint64_t file_size;
  char client_ip[INET_ADDRSTRLEN];
  size_t recv_bytes;
  char* filename;
  char* filepath;
  char buff[BUFFSIZE];
  ssize_t recived, left_to_recive, was_left, sent;
  FILE* file;
  struct timespec start_time, last_print_time, current_time;

  client_info = (client_t*)arg;
  client_sock = client_info->client_sock;
  port = ntohs(client_info->client_addr.sin_port);

  inet_ntop(AF_INET, &(client_info->client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

  fprintf(stdout, "Handling %s:%d\n", client_ip, port);

  free(client_info);

  recived = recv(client_sock, buff, 2, MSG_WAITALL);

  if (recived == 0 || recived < 2) {
    fprintf(stderr, "Client %s:%d didn't send anything\n", client_ip, port);
    close(client_sock);
    pthread_exit(NULL);
  }

  filename_size = ntohs(*((int16_t*)buff));

  if (filename_size > 4096) {
    fprintf(stderr, "Client %s:%d send too long file name (%d)\n", client_ip, port, filename_size);
    close(client_sock);
    pthread_exit(NULL);
  }

  filename = (char*)malloc(sizeof(char) * (filename_size + 1));

  fprintf(stdout, "Waiting for %d chars from %s:%d\n", filename_size, client_ip, port);

  if (filename_size < BUFFSIZE) {
    recived = recv(client_sock, buff, filename_size, MSG_WAITALL);

    if (recived != filename_size) {
      fprintf(stderr, "Failed to recieve filename from %s:%d\n", client_ip, port);
      free(filename);
      close(client_sock);
      pthread_exit(NULL);
    }

    memcpy(filename, buff, filename_size);
  }
  else {
    left_to_recive = filename_size;

    while(left_to_recive != 0) {
      recived = recv(client_sock, buff, BUFFSIZE < filename_size? BUFFSIZE : filename_size, MSG_WAITALL);

      if (recived != BUFFSIZE < filename_size? BUFFSIZE : filename_size) {
        fprintf(stderr, "Failed to recieve filename from %s:%d", client_ip, port);
        free(filename);
        close(client_sock);
        pthread_exit(NULL);
      }

      memcpy(filename + (filename_size - left_to_recive), buff, recived);

      left_to_recive -= recived;
    }
  }

  *(filename + filename_size) = '\0';

  fprintf(stdout, "Filename: %s\n", get_filename(filename));

  recived = recv(client_sock, buff, 8, MSG_WAITALL);

  if (recived < 8) {
    fprintf(stderr, "Failed to recieve file size from %s:%d\n", client_ip, port);
    free(filename);
    close(client_sock);
    pthread_exit(NULL);
  }

  memcpy(&tmp, buff, 4);

  file_size = (uint64_t)ntohl(tmp) << 32;

  memcpy(&tmp, buff + 4, 4);

  file_size += ntohl(tmp);

  fprintf(stdout, "File size: %llu\n", file_size);

  if (file_size > (((uint64_t)1) << 40)) {
    fprintf(stderr, "Client %s:%d tried to send file bigger than 1 tb\n", client_ip, port);
    free(filename);
    close(client_sock);
    pthread_exit(NULL);
  }

  mkdir("./uploads", 0755);

  filepath = (char*)malloc(sizeof(char) * (strlen(get_filename(filename)) + 1 + 10));

  snprintf(filepath, strlen(get_filename(filename)) + 10 + 1, "./uploads/%s\0", get_filename(filename));

  free(filename);

  file = fopen(filepath, "wb");
  if (!file) {
    fprintf(stderr, "Server failed to create file for client %s:%d\n", client_ip, port);
    free(filepath);
    close(client_sock);
    pthread_exit(NULL);
  }

  fprintf(stdout, "Created file for client %s:%d, recieving contents...\n", client_ip, port);

  left_to_recive = file_size;
  was_left = left_to_recive;

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  clock_gettime(CLOCK_MONOTONIC, &last_print_time);

  while(left_to_recive != 0) {

    recived = recv(client_sock, buff, left_to_recive > BUFFSIZE? BUFFSIZE: left_to_recive, MSG_DONTWAIT);

    clock_gettime(CLOCK_MONOTONIC, &current_time);

    if (recived > 0) {
      if (current_time.tv_sec - last_print_time.tv_sec >= 3) {

        fprintf(stdout, "Client %s:%d\nRecived %llu bytes out of %llu bytes, speed is %lf mb/s and instant speed is %lf mb/s\n", client_ip, port, file_size - left_to_recive, file_size, ((double)(file_size - left_to_recive))/(current_time.tv_sec - start_time.tv_sec + (current_time.tv_nsec - start_time.tv_nsec)/1e9)/((double) (1 << 20)), ((double)(file_size - left_to_recive))/((double)(1<<20)) / (current_time.tv_sec - last_print_time.tv_sec + (current_time.tv_nsec - last_print_time.tv_nsec)/1e9));

        clock_gettime(CLOCK_MONOTONIC, &last_print_time);
        if (was_left == left_to_recive) {
          fprintf(stdout, "client %s:%d didn't send anything in 3 seconds\n", client_ip, port);
          break;
        }

        was_left = left_to_recive;
      }

      fwrite(buff, 1, recived, file);

      left_to_recive -= recived;
    }
    else {
      clock_gettime(CLOCK_MONOTONIC, &current_time);

      if (current_time.tv_sec - last_print_time.tv_sec >= 3) {
        fprintf(stdout, "client %s:%d didn't send anything in 3 seconds\n", client_ip, port);
        break;
      }
    }
  }

  fprintf(stdout, "Client %s:%d\nRecived %llu bytes out of %llu bytes, speed is %lf mb/s and instant speed is %lf mb/s\n", client_ip, port, file_size - left_to_recive, file_size, ((double)(file_size - left_to_recive))/(current_time.tv_sec - start_time.tv_sec + (current_time.tv_nsec - start_time.tv_nsec)/1e9)/((double) (1 << 20)), ((double)(file_size - left_to_recive))/((double)(1<<20)) / (current_time.tv_sec - last_print_time.tv_sec + (current_time.tv_nsec - last_print_time.tv_nsec)/1e9));

  if (!fclose(file)) {
    fprintf(stdout, "Server failde to close file\n");
    recv_bytes = 0;

    sent = send(client_sock, &recv_bytes, sizeof(recv_bytes), 0);
  }
  else {
    recv_bytes = file_size - left_to_recive;

    sent = send(client_sock, &recv_bytes, sizeof(recv_bytes), 0);

    if (sent != sizeof(recv_bytes)) {
      fprintf(stderr, "Failed to send ACK to client %s:%d\n", client_ip, port);
    }
    else {
      fprintf(stdout, "Send %s:%d transfer confirmation, thread exits...\n", client_ip, port);
    }
  }

  free(filepath);

  close(client_sock);
  pthread_exit(NULL);
}

int32_t main(int32_t argc, char** argv) {
  int32_t server_socket, client_socket, opt;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_len = sizeof(client_addr);
  pthread_t tmp;
  pthread_attr_t attr;
  client_t* client_info;

  setlocale(LC_ALL, "");

  if (argc < 2) {
    fprintf(stderr, "Specify port\n");
    return -1;
  }

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    fprintf(stderr, "Server failed to create socket\n");
    return -1;
  }

  opt = 1;

  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
    fprintf(stderr, "Failed to set reuse to socket\n");
    close(server_socket);
    return -1;
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(atoi(*(argv + 1)));

  if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
    fprintf(stderr, "Server failed to bind\n");
    close(server_socket);
    return -1;
  }

  if (listen(server_socket, 255) == -1) {
    fprintf(stderr, "Server listen failed\n");
    close(server_socket);
    return -1;
  }

  fprintf(stdout, "Server listens on %s\n", *(argv + 1));

  pthread_attr_init(&attr);

  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  while (1) {
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
      fprintf(stderr, "Accept failed\n");
      continue;
    }

    client_info = (client_t*)malloc(sizeof(client_t));

    client_info->client_sock = client_socket;
    client_info->client_addr = client_addr;

    pthread_create(&tmp, &attr, client_handler, (void*)client_info);
  }

  pthread_attr_destroy(&attr);

  close(server_socket);

  return 0;
}
