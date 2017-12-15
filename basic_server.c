#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;
  char buf[BUFFER_SIZE];


  memset(buf, 0, BUFFER_SIZE);

  while ( 1 ) {
    from_client = server_handshake( &to_client );
    while( read(from_client, buf, sizeof(buf)) ){
      printf("Received from client: %s\n", buf);
      int i;
      for ( i=0 ; buf[i] ; i++ ) {
        buf[i] = toupper(buf[i]);
      }
      write(to_client, buf, sizeof(buf));
    }
  }

  return 0;
}
