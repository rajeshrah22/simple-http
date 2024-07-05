#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 1024 * 64
#define MAX_HEADERS 64
#define MAX_BODY_SIZE 1024

struct URI {
  int uri_length;
  char path[256];
  char query_string[256];
};

struct http_request_line {
  struct URI uri;
  char method[16];
  char version[16];
};

struct http_header {
  char name[64];
  char value[256];
};

struct http_request {
  int header_count;
  struct http_request_line request_line;
  struct http_header headers[MAX_HEADERS];
  char body[MAX_BODY_SIZE];
};

int  main() {
  struct sockaddr_in addr;
  int cfd;  // connection file descriptor
  socklen_t peer_addr_size;
  char buf[BUFSIZE];

  int sfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
  if (sfd == -1) {
    perror("socket failiure\n");
    return -1;
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(3000);
  addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

  if (bind(sfd, (struct sockaddr*) &addr, sizeof(addr)) == -1)
    perror("bind failiure\n");

  if (listen(sfd, 5) == -1)
    perror("listen failiure\n"); 

  cfd = accept(sfd, (struct sockaddr *) &addr, &peer_addr_size);
  if (cfd == -1)
    perror("accept error\n");

  ssize_t n = recv(cfd, buf, BUFSIZE, 0);
  printf("data: %s\n", buf);

  struct http_request request;

  // PARSE REQUEST:
  // get method
  int i = 0;
  for(; i < n; i++) {
    if(buf[i] != ' ')
      continue;
    else
      break;
  }
  
  strncpy(request.request_line.method, buf, i);

  int uri_idx = 0;
  for(++i; i < n; i++) {
    if (buf[i] == '?' || buf[i] == ' ')
      break;
    request.request_line.uri.path[uri_idx] = buf[i];
    uri_idx++;
  }

  request.request_line.uri.uri_length = uri_idx;

  printf("URI: %s | LEN: %d\n", request.request_line.uri.path, request.request_line.uri.uri_length);

  // deconstruct URL into scheme, server, path, query string
  // read headers [key]: [value]
  // read body

  // SEND RESPONSE:
  // Response code and description
  // headers
  // body

  return 0;
}

