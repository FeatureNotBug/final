#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

void server_function(int from_client, int to_client) {
  char line[100];
  int dx = -1;
  int dy = 0;
  struct drawing_information {
    int paddle_one_y;
    int paddle_two_y;
    int ball_x;
    int ball_y;
    int score_one;
    int score_two;
  } drawing_information;
  
  drawing_information.ball_x = 360;
  drawing_information.ball_y = 240;
  drawing_information.paddle_one_y = 0;
  drawing_information.paddle_one_y = 0;
  drawing_information.paddle_two_y = 0;
  drawing_information.score_one = 0;
  drawing_information.score_two = 0;
  
  while (1) {
    char line_to_write[100];
    printf(line_to_write, "%d,%d,%d,%d,%d,%d", drawing_information.ball_x, drawing_information.ball_y, drawing_information.paddle_one_y, drawing_information.paddle_two_y, drawing_information.score_one, drawing_information.score_two);
    write(to_client, line_to_write, sizeof(line_to_write));
 
    read(from_client, line, sizeof(line));

    drawing_information.ball_x += dx;
    drawing_information.ball_y += dy;
  }
}

void server_connection_protocol(int *from_client) {
  int to_client;
  char line[100];
  
  // Loop to handle the client connections
  
  while (1) {
    // Create a Well Known Pipe
    mkfifo("mario", 0644);
    
    // Waits for a connection
    *from_client = open("mario", O_RDONLY);
    
    // Recieve message from client
    read(*from_client, line, sizeof(line));
    printf("<Server> Got connection request: [%s]\n", line);
    
    // Remove well known pipe
    remove("mario");
    
    // Fork a process to deal with this client
    int forkin = fork();
    
    if (forkin == 0) {
      // Connect to the client pipe
      to_client = open(line, O_WRONLY);				
      
      // Send acknowledgement message
      strncpy(line, "it's-a-me, mario!", sizeof(line));
      write(to_client, line, sizeof(line));
      
      // Call the server_function to handle the client
      server_function(*from_client, to_client);
      exit(1);
    }
  }
}

void sighandler(int signum) {
  printf("Closing nicely...\n");
  remove("mario");
  exit(0);
}

int main() {
  signal(2, sighandler);

  int to_client;
  int from_client;
  
  char line[100];
  
  server_connection_protocol(&from_client);
}
