#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 1024 * 64
#define MAX_HEADERS 64
#define MAX_BODY_SIZE 1024

struct http_request_line {
  char method[16];
  char version[16];
  char uri[256];
};

struct http_header {
  char value[256];
  char name[64];
};

struct URI {
  char path[256];
  char query_string[256];
};

struct http_request {
  struct http_request_line request_line;
  struct http_header headers[MAX_HEADERS];
  int header_count;
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
  int i = 0;
  for(; i < n; i++)
    if(buf[i] != ' ')
      continue;
    else
      break;
  
  strncpy(request.request_line.method, buf, i + 1);
  printf("METHOD: %s\n", request.request_line.method);
  // get request type

  // deconstruct URL into scheme, server, path, query string
  // read headers [key]: [value]
  // read body

  // SEND RESPONSE:
  // Response code and description
  // headers
  // body

}

