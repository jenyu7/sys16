#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char buf[HANDSHAKE_BUFFER_SIZE];
  if ( mkfifo("wellknown_pipe", 0644) == -1 ) {
    // if mkfifo gives -1, an error happened
    printf("SERVER: %d %s", errno, strerror(errno));
    exit(1);
  }
  printf("Server pipe created.\n");
  
  int from_client = open("wellknown_pipe", O_RDONLY);
  printf("Connecting to client...\n");
  if ( from_client == -1 ) {
    // if open gives -1, an error happened
    printf("SERVER: %d %s", errno, strerror(errno));
    exit(1);
  }

  printf("Connecting to pipe %s...\n", buf);
  read(from_client, buf, sizeof(buf)); // client's secret pipe name

  *to_client = open(buf, O_WRONLY);
  if (*to_client == -1 ) {
    // if open gives -1, an error happened
    printf("SERVER: %d %s", errno, strerror(errno));
    exit(1);
  }
  
  strcpy(buf, "hello");
  write(*to_client, buf, sizeof(buf));
  //memset(clients_message, 0, 256); // set everything in clients_message to 0
  
  read(from_client, buf, sizeof(buf));
  
  if ( strcmp(buf,"Received.") != 0 ) {
    // unexpected message
    printf("SERVER: unexpected message from client for third handshake");
    exit(1);
  }

  printf("Connection established.\n");
  remove("wellknown_pipe");
  
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char buf[HANDSHAKE_BUFFER_SIZE];
  char pname[128];
  sprintf(pname,"%d",getpid());
  int val = mkfifo(pname, 0644);
  strcpy(buf,pname);
  if (val == -1){printf("Error: %s\n", strerror(errno));exit(0);}
  printf("Pipe created: %s\n", pname);
  
  *to_server = open("wellknown_pipe", O_WRONLY);
  write(*to_server, buf, sizeof(buf));
  printf("Connection made to server.\n");

  int from_server = open(pname, O_RDONLY);
  read(from_server, buf, sizeof(buf));
  printf("Received from server: %s\n", buf);

  if(strcmp(buf,"hello")==0){
    printf("Successfully connected.\n");
  }
  else{
    printf("Wrong message received. Disconnecting...\n");
    exit(0);
  }
  remove(pname);

  strcpy(buf, "Received.");
  write(*to_server, buf, sizeof(buf));
  
  return from_server;
}

/*int main(){
  int * in;
  printf("%d\n", client_handshake(in));
  }*/
