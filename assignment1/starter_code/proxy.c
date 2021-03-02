#include "proxy_parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <netinet/tcp.h>
#define BUFFER_SIZE 1024
#define SMALL_BUF 100

/* proxy steps
 * Establish a socket connection to listen for incoming connections.
 * Accept each client request in a new process.
 * Parse header of request and get requested URL.
 * Get data from requested remote server.
 * Send data to the client
 * Return 0 on success, non-zero on failure
*/

//TODO: send and receive data to remote Server
void communicateWtihRemote() {
    
}

// to parse into the proxy and Send to Server
void parseInServer(int clnt_sock) {
  int len = 1;
  char buffer[BUFFER_SIZE];
  struct ParsedRequest *req = malloc(sizeof(struct ParsedRequest));
  while (len > 1) {
    len = read(clnt_sock, buffer, BUFFER_SIZE);
  }
  printf("The current buffer is %s\n", buffer);
  if (ParsedRequest_parse(req, buffer, len) < 0) {
    printf("parse failed\n");
    return -1;
  }
  printf("Host:%s\n", req->host);
  //Get data from requested remote server.
  //TODO: check the parse is correct?
  
  //get complete URL
  char* url = char[SMALL_BUF];;    
  strcat(url, req->host);
  strcat(url, req->port);
  strcat(url, req->path);
  //TODO: communicate and send data back to client
  communicateWithRemote();

}

int proxy(char *proxy_port) {
  printf("entering proxy");
  int serv_sock,clnt_sock;
  struct sockaddr_in serv_addr,clnt_addr;
  socklen_t clt_sz = sizeof(clnt_addr);
  char buf[BUFFER_SIZE];
  printf("Creating socket");
  serv_sock = socket(PF_INET,SOCK_STREAM,0);
  memset(&serv_addr,0,sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(proxy_port));
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1) {
    perror("bind()");
    exit(1);
  }

  printf("Binded");
  if(listen(serv_sock,20) == -1) {
    perror("listen()");
    exit(1);
  }
  printf("Connecting");
  int processCnt = 0;
  while (1) {
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clt_sz);
    printf("connection Request: %s:%d\n",inet_ntoa(clnt_addr.sin_addr),ntohs(clnt_addr.sin_port));
    // Todo: Fork a new process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        printf("Child process Created\n");
        _exit(EXIT_SUCCESS);
    }
    else {
        int status;
        (void)waitpid(pid, &status, 0);
    }
    return EXIT_SUCCESS;
    // parse header of request and get requested URL
      
    close(clnt_sock);
  }
}


int main(int argc, char * argv[]) { 
  char *proxy_port;
  if (argc != 2) {
    fprintf(stderr, "Usage: ./proxy <port>\n");
    exit(EXIT_FAILURE);
  }

  proxy_port = argv[1];
  printf("Asking proxy function");
  return proxy(proxy_port);
}
