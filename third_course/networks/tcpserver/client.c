#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <locale.h>

#define BUFFSIZE 4096

#if BUFFSIZE < 8
  #error "BUFFSIZE must be at least 8"
#endif

void send_file(int32_t sockfd, char* filename) {
  char buff[4096];
  int32_t strl;
  uint32_t tmp;
  uint64_t filesize;
  ssize_t confirm;
  size_t r, left_to_read, server_received;
  FILE* file;

  file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open file\n");
    return;
  }

  strl = (int32_t)strlen(filename);

  fseek(file, 0, SEEK_END);
  filesize = ftell(file);
  fseek(file, 0, SEEK_SET);

  *((int16_t*)buff) = htons(strl);

  fprintf(stdout, "Filename size %d, file size %llu\n", strl, filesize);

  write(sockfd, buff, 2);
  write(sockfd, filename, strl);

  tmp = htonl((uint32_t)(filesize >> 32));

  memcpy(buff, &tmp, 4);

  tmp = htonl((uint32_t)(filesize & 0xFFFFFFFF));

  memcpy(buff + 4, &tmp, 4);

  write(sockfd, buff, 8);

  left_to_read = filesize;

  while(left_to_read != 0) {
    r = fread(buff, 1, BUFFSIZE, file);

    write(sockfd, buff, r);

    left_to_read -= r;
  }

  confirm = recv(sockfd, &server_received, sizeof(server_received), 0);

  if (confirm == sizeof(server_received)) {
    if (server_received == filesize) {
      fprintf(stdout, "Server confirmed file transfer\n");
    }
    else {
      fprintf(stdout, "Server didn't receive file\n");
    }
  }
  else {
    fprintf(stderr, "Failed to recieve confirmation\n");
  }

  fclose(file);
}

int32_t main(int32_t argc, char** argv) {
  int32_t sockfd;
  int16_t port;
  struct sockaddr_in serveraddr;

  setlocale(LC_ALL, "");

  if(argc < 4) {
    fprintf(stderr, "Specify ip address, port and path to file\n");
    return -1;
  }

  port = (int16_t)atoi(*(argv + 2));

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    fprintf(stderr, "Failed to get socket\n");
    return -1;
  }

  memset(&serveraddr, 0, sizeof(serveraddr));

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = inet_addr(*(argv + 1));
  serveraddr.sin_port = htons(port);

  if (connect(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) != 0) {
    fprintf(stderr, "Failed to connect\n");
    return -1;
  }

  send_file(sockfd, *(argv + 3));

  close(sockfd);
  return 0;
}
