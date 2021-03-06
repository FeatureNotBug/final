// A universal key listener: works on Windows, Linux, and (presumably) OSX 
//
//
// Adapted from code by user 'Thanatos' on the C Programming forums
// posted at http://cboard.cprogramming.com/linux-programming/51531-faq-cached-input-mygetch.html

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
 
void changemode(int);
int  kbhit(void);

char get_letter()
{
  int ch;
  changemode(1);
  while ( !kbhit() )
  {
    //waits
  }
  ch = getchar();
  changemode(0);
  return ch;
}

// For testing the functionality of this file on its own:
// Loops until user exits, printing any key that is pressed.
//
//int main(void)
//{
//  int ch;
//  changemode(1);
//  while(1){
//  while ( !kbhit() )
//  {
//    //waits
//  }
// 
//  ch = getchar();
// 
//  printf("\nGot %c\n", ch);
//  }
//  changemode(0);
//  return 0;
//}
 
void changemode(int dir)
{
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}
 
int kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
 
}