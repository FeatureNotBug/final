#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include "deck.h"

int judge_handshake(int* from_server){
  int to_server;
  char buffer[100];

  sprintf(buffer,"%d",getpid());
  mkfifo(buffer,0644);

  to_server = open("pip",O_WRONLY);
  write(to_server,buffer,sizeof(buffer));

  *from_server = open(buffer,O_RDONLY);
  remove(buffer);

  read(*from_server,buffer,sizeof(buffer));
  printf("Judge connected: %s\n",buffer);

  return to_server;
  
}

void receivecards(int* from_server,card *received[8]){
  char buffer[100];
  int counter = 0;
  while(counter < 8){
    read(*from_server,buffer,sizeof(buffer));
    (*received)[counter] = *makecard(buffer,"red");
    printf("%s\n",((*received)[counter]).content);
    counter ++;
  }
}

int main(){
  
  card received[8];
  int to_server;
  int from_server;
  char buffer[100];
  int winner;
  
  to_server = judge_handshake(&from_server);
  while(1){
    printf("select a card as the winner: \n");
    int counter = 0;
    while(counter < 8 && received[counter].content){
      printf("%d.%s\n",counter,received[counter].content);
    }
    fgets(buffer,sizeof(buffer),stdin);
    *strchr(buffer,'\n') = 0;
    int winner = atoi(buffer);
    while(winner < 0 || winner > 7 || !(received[winner].content)){
      printf("please type in a valid number: \n");
      while(counter < 8 && received[counter].content){
	printf("%d.%s\n",counter,received[counter].content);
      }
      fgets(buffer,sizeof(buffer),stdin);
      *strchr(buffer,'\n') = 0;
      int winner = atoi(buffer);
    }
    write(to_server,received[winner].content,sizeof(received[winner].content));    
  }
  
  close(to_server);
  close(from_server);
  return 0;
}
