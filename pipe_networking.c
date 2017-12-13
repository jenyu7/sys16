#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  if ( mkfifo("wellknown_pipe", 0644) == -1 ) {
    // if mkfifo gives -1, an error happened
    printf("SERVER: %d %s", errno, strerror(errno));
    exit(1);
  }
  char clients_message[256];
  int *from_client = open("wellknown_pipe", O_RDONLY);
  if ( *from_client == -1 ) {
    // if open gives -1, an error happened
    printf("SERVER: %d %s", errno, strerror(errno));
    exit(1);
  }
  memset(clients_message, 0, 256); // set everything in clients_message to 0
  read(*from_client, clients_message, 256); // client's secret pipe name
  close(*from_client);
  *to_client = open(clients_message, O_WRONLY);
  if ( *to_client == -1 ) {
    // if open gives -1, an error happened
    printf("SERVER: %d %s", errno, strerror(errno));
    exit(1);
  }
  write(*to_client, "got it, respond back pls", 256);
  memset(clients_message, 0, 256); // set everything in clients_message to 0
  read(*from_client, clients_message, 256);
  if ( strcmp(clients_message,"got it, here's my response") != 0 ) {
    // unexpected message
    printf("SERVER: unexpected message from client for third handshake");
    exit(1);
  }
  
  return *from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int val = mkfifo("client_pipe", 0644);
  if (val == -1){printf("Error: %s\n", strerror(errno));exit(0);}
  *to_server = open("client_pipe", O_RDWR);
  close(*to_server);
  return *to_server;
}

int main(){
  int * in;
  printf("%d\n", client_handshake(in));
}
