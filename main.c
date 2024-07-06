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

enum http_method { GET, OTHER };

int build_http_response(char *buf) {
  int response_size = 0;

  char file_buf[BUFSIZE];
  int fd = open("./root/index.html", O_RDONLY);
  ssize_t file_size = read(fd, file_buf, BUFSIZE);

  printf("File size: %d\n", (int)file_size);

  strcpy(buf, OK);
  strcpy(&buf[17], CONTENT_TYPE);
  response_size = (int)file_size + strlen(buf) + 3;
  sprintf(&buf[43], CONTENT_LENGTH, response_size);
  strcat(buf, "\r\n");
  strcat(buf, file_buf);
  strcat(buf, "\r\n");

  close(fd);

  return response_size;
}

int  main() {
  struct sockaddr_in addr;
  socklen_t peer_addr_size;
  enum http_method method;
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

  while(1) {
    int cfd = accept(sfd, (struct sockaddr *) &addr, &peer_addr_size);
    if (cfd == -1)
      perror("accept error\n");
    ssize_t n = recv(cfd, buf, BUFSIZE, 0);
    printf("data: %s\n", buf);

    if (buf[0] == 'G'){
      method = GET;
    } else {
      method = OTHER;
    }

    // clear buffer
    memset(buf, 0, BUFSIZE);

    switch (method){
    case GET:
      /* code */
      // send bytes from file to response
      int response_size = build_http_response(buf);

      int n = send(cfd, buf, response_size, 0);

      if (n != response_size)
        perror("response not completely sent\n");

      memset(buf, 0, BUFSIZE);

      close(cfd);

      break;
    
    default:
      break;
    }
  }

  return 0;
}

