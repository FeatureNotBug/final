#ifndef values
#define values

//Note on semaphores. Semaphore is array of size 2. 
//Index [0] controls userlist. Index [1] controls log

/*Input formats:
  ? - displays command formats
  msg [name] [msg] - Sends message to [name]
  file [name] [filename] - Sends file to [name]
  com [name] [command] - Sends command to [name]
  old [name] - retrieve old messages from [name]
*/

//Only needed for linux systems
#if defined(unix) || defined(__unix__) || defined(__unix)
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short *array;
  struct seminfo *_buf; 
};
#endif

char * file_path="root";
int sem_id = 42;
//help ultimately not used
char * help="You have the following commands:\n\t1 - Send a message\n\t2 - Send a file\n\t3- Send a command";
int kill_num=10000000;

#define MAXRCVLEN 500
#define PORTNUM 2300
#define NAME_LEN 21
#define PASS_LEN 21
#define MAX_MSG 501

#endif
