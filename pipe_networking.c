#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  if ( mkfifo("wellknown_pipe", 0644); ) {
    // if mkfifo gives -1, an error happened
    printf("SERVER: fifo creation failed");
    return 1;
  }
  return 0;
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
