#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {

  int socket_id;
  char buffer[256];
  int i;
  


  //create the socket
  socket_id = socket( AF_INET, SOCK_STREAM, 0 );
  
  //bind to port/address
  struct sockaddr_in sock;
  sock.sin_family = AF_INET;   
  sock.sin_port = htons(50123);
  //Set the IP address to connect to
  //127.0.0.1 is the "loopback" address of any machine
  inet_aton( "127.0.0.1", &(sock.sin_addr) );
  bind( socket_id, (struct sockaddr *)&sock, sizeof(sock));
  
  //attempt a connection
  i = connect(socket_id, (struct sockaddr *)&sock, sizeof(sock));
  printf("<client> connect returned: %d\n", i);

  read( socket_id, buffer, sizeof(buffer));
  printf("<client> received: [%s]\n", buffer );
  buffer[0] = 0; //Clearing buffer so we can write to server
  

  while( 1 ){ //While loop that allows communication
    printf("Input: ");
    fgets(buffer, sizeof(buffer), stdin); //Gets user input

    
    write( socket_id, buffer, sizeof(buffer)); 
    buffer[0] = 0; //Clearing buffer so we can read from server again
    
    read( socket_id, buffer, sizeof(buffer) );
    
    printf("<client> recieved: [%s]\n", buffer);
  }
  
  return 0;
}
 
