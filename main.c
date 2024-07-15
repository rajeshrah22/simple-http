#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <pthread.h>

#define BUFSIZE 1024 * 64

#define OK "HTTP/1.0 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_LENGTH "Content-Length: %d\r\n"
#define DATE "Date: "

#define PORT 3000
#define LOCALHOST "127.0.0.1"
#define BACKLOG 25

#define DEFAULT_PATH "./root/index.html"

enum http_method { GET, OTHER };

struct span {
  char *str;
  int length;
};

int build_http_response(const struct span path, char *buf) {
  int response_size = 0;
  char file_buf[BUFSIZE];

  char root_path[] = "./root";

  char open_path[BUFSIZE];
  if (path.length > 1) {
    snprintf(open_path, sizeof(open_path), "%s%s", root_path, path.str);
  } else {
    strncpy(open_path, DEFAULT_PATH, sizeof(open_path) - 1);
  }

  int fd = open(open_path, O_RDONLY);
  if (fd == -1) {
    perror("http_response: Failed to open file");
    return -1;
  }

  ssize_t file_size = read(fd, file_buf, BUFSIZE);

  if (file_size == -1) {
    perror("Failed to read file");
    close(fd);
    return -1; // Return error code
  }

  strcpy(buf, OK);
  strcpy(&buf[17], CONTENT_TYPE);
  response_size = (int)file_size + strlen(buf) + 2;
  sprintf(&buf[43], CONTENT_LENGTH, response_size);
  strcat(buf, "\r\n");
  strcat(buf, file_buf);
  strcat(buf, "\r\n");

  close(fd);

  return response_size;
}

void* handle_connection(void* cfd_void) {
  int cfd = *((int*)cfd_void);
  char buf[BUFSIZE];
  ssize_t res = recv(cfd, buf, BUFSIZE, 0);

  enum http_method method;

  if (buf[0] == 'G') {
    method = GET;
  } else {
    method = OTHER;
  }

  char path[256];

  int idx = 0;
  for(; idx < (int)res; idx++) {
    if(buf[idx] != '/')
      continue;
    else
    break;
  }

  int path_idx = 0;
  for(; idx < (int)res; idx++) {
    if(buf[idx] != '?' && buf[idx] != ' ') {
      path[path_idx] = buf[idx];
      path_idx++;
    } else {
      break;
    }
  }

  path[path_idx] = 0;
  struct span path_span;
  path_span.str = path;
  path_span.length = path_idx;
 
  memset(buf, 0, BUFSIZE);

  switch(method) {
  case GET:
    int response_size = build_http_response(path_span, buf);
    if (response_size < 0)
      perror("failed build http response\n");

    res = send(cfd, buf, response_size, 0);

    if (res != response_size)
      perror("response not completely sent\n");

    memset(buf, 0, BUFSIZE);

    break;

  default:
    break;
  }

  close(cfd);
  free(cfd_void);
  return 0;
}

int init_server(struct sockaddr_in *addr) {

  int sfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
  if (sfd == -1) {
    perror("socket failiure\n");
    return -1;
  }

  int opt = 1;
  if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    close(sfd);
    return -1;
  }

  if (bind(sfd, (struct sockaddr*) addr, sizeof(struct sockaddr_in)) == -1) {
    perror("bind failiure\n");
    return -1;
  }

  if (listen(sfd, BACKLOG) == -1) {
    perror("listen failiure\n"); 
    return -1;
  }
  return sfd;
}

int  main() {
  struct sockaddr_in addr;
  socklen_t peer_addr_size;
  pthread_t thread;

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_addr(LOCALHOST);
  peer_addr_size = sizeof(struct sockaddr_in);

  int sfd = init_server(&addr);

  if (sfd == -1) {
    perror("Server init failiure\n");
    return -1;
  }

  while(1) {
    int *cfd = malloc(sizeof(int));
    *cfd = accept(sfd, (struct sockaddr *) &addr, &peer_addr_size);

    if (*cfd == -1)
      perror("accept error\n");

    pthread_create(&thread, 0, handle_connection, cfd);
  }

  close(sfd);

  return 0;
}
