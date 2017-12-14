#include "pipe_networking.h"


int main() {

  int to_server;
  int from_server;
  char buf[BUFFER_SIZE];
  from_server = client_handshake( &to_server );

  while(1){
    printf("Send message to the server: \n");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] == '\n'){printf("No text entered.\n");exit(0);}
    write(to_server, buf, sizeof(buf));
    read(from_server, buf, sizeof(buf));
    printf("Received from server: %s\n", buf);
  }
  return 0;
}
