#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 1024 * 50

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
  printf("data: %s", buf);
}

