#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#define BUFSIZE 1024 * 64

#define OK "HTTP/1.0 200 OK\r\n"
#define CONTENT_TYPE "Content-Type: text/html\r\n"
#define CONTENT_LENGTH "Content-Length: %d\r\n"
#define DATE "Date: "

#define PORT 3000
#define LOCALHOST "127.0.0.1"
#define BACKLOG 50

enum http_method { GET, OTHER };

int build_http_response(char *buf) {
  int response_size = 0;
  char file_buf[BUFSIZE];

  int fd = open("./root/index.html", O_RDONLY);
  if (fd == -1) {
    perror("Failed to open index.html");
  }

  ssize_t file_size = read(fd, file_buf, BUFSIZE);

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

void handle_connection(int cfd, char *buf) {
  ssize_t res = recv(cfd, buf, BUFSIZE, 0);
  enum http_method method;

  if (buf[0] == 'G'){
    method = GET;
  } else {
    method = OTHER;
  }

  memset(buf, 0, BUFSIZE);

  switch (method){
  case GET:
    int response_size = build_http_response(buf);

    res = send(cfd, buf, response_size, 0);

    if (res != response_size)
      perror("response not completely sent\n");

    memset(buf, 0, BUFSIZE);

    close(cfd);

    break;

  default:
    break;
  }

}

int init_server(struct sockaddr_in *addr) {

  int sfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC, 0);
  if (sfd == -1) {
    perror("socket failiure\n");
    return -1;
  }


  if (bind(sfd, (struct sockaddr*) addr, sizeof(addr)) == -1)
    perror("bind failiure\n");
    return -1;

  if (listen(sfd, BACKLOG) == -1)
    perror("listen failiure\n"); 
    return -1;

  return sfd;
}

int  main() {
  char buf[BUFSIZE];
  struct sockaddr_in addr;
  socklen_t peer_addr_size;

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = inet_addr(LOCALHOST); 

  int sfd = init_server(&addr);

  while(1) {
    int cfd = accept(sfd, (struct sockaddr *) &addr, &peer_addr_size);

    if (cfd == -1)
      perror("accept error\n");

    handle_connection(cfd, buf);
  }

  close(sfd);

  return 0;
}
