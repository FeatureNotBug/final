#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define USER_LEN 20
#define PSWD_LEN 20

void make_profile(char *username) {

}

void find_username(char *username) {



}

int main() {
  char user[USER_LEN]; char pswd[PSWD_LEN];
  char underscore[1] = "_";
  //char *newline;
  char *username;// char *password;
  char yes_no;
  //newline = (char *)malloc(sizeof(char *));
  //newline = "\n";

  printf("=======WELCOME TO MARY'S PROM DATE SERVER=======\nYou want to take Mary out to PROM because she is an awesome person and if you don't want to then you have bad taste in women.\n\nPlease type in 1 if you have an account or 2 if you'd like to make one.\n");
  fgets(&yes_no, 3, stdin);
  
  printf("yes_no value:%c\n", yes_no);
  
  if (yes_no == '1') {
    FILE* fd1 = fopen("username.txt", "r");
    printf("Please type in your username:\n");
    fgets(user, USER_LEN, stdin);

    printf("Please type in your password:\n");
    fgets(pswd, PSWD_LEN, stdin);

    printf("user: %s\n", user);
    printf("pswd: %s\n", pswd);

    char *buffer = (char *)malloc(100*sizeof(char));
    //printf("sizeof(buffer) = %lu\n", sizeof(buffer));
    fread(buffer, sizeof(char), 100, fd1);
    printf("buffer: %s\n", buffer);
    buffer = "";
    printf("buffer after fread/fwrite: %s\n", buffer);    
  }

  else if (yes_no == '2'){
    FILE* fd1 = fopen("username.txt", "a+");
    printf("Please create a username:\n");
    fgets(user, USER_LEN, stdin);
    
    printf("Please create a password:\n");
    fgets(pswd, PSWD_LEN, stdin);
    //printf("username array: %s\n", user);
    //printf("pswd array: %s\n", pswd);
    username = calloc(strlen(user) + strlen(pswd) + 1 + 1, sizeof(char));//1 is for the underscore and the other is for the null char
    //password = calloc(strlen(pswd) + 1, sizeof(char));

    strcat(username, user);
    strsep(&username, "\n");
    printf("sizeof(username) = %lu\n", strlen(username));
    printf("username: %s\n", username);

    strcat(username, underscore);
    printf("username: %s\n", username);
    //strcat(password, pswd);
    strcat(username, pswd);
    fwrite(username, sizeof(char), strlen(username), fd1);

        
    fclose(fd1);
    return 0;
  }
  else
    printf("STOP SABOTAGING THIS PROGRAM AND GIVE US EITHER 1 OR 2 AS YOUR ANSWER. SMH PEOPLE THESE DAYS\n");
}

/*
  int fd1 = open("username.txt", O_RDONLY | O_WRONLY | O_APPEND);
  if (fd1 == -1)
    printf("fd1 is broken: %s\n", strerror(errno));
  
  int fd2 = write(fd1, usrname, sizeof(usrname));
  if (fd2 == -1)
    printf("fd2 is broken: %s\n", strerror(errno));

  
  char *buffer = (char *)malloc(1024);
  
  int readfile = scanf(fd1, buffer, sizeof(buffer));
  printf("buffer: %s\n", buffer);
*/
