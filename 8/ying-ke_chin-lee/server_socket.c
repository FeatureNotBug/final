/* 
	CODE ADAPTED FROM http://www.linuxhowtos.org/C_C++/socket.htm
	"A TCP server in the internet domain
	The port number is passed as an argument 
	This version runs forever, forking off a separate 
	process for each connection"
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

#include "writing.h"

#define SIZEBUFF 256

/* potential ending condition: all bidders leave */
int num_bidders = 0;
int auction_started = 0;

void dostuff(int); /* function prototype */
void write_bid(char *, char *); // check if you can write the new bid, and then do the sem/shmem stuff
void create_bid_history(); // puts together the bid history in a "nicer" way

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

static void sighandler(int signo) {
	if (signo == SIGINT) {
		printf("ctrl c was hit\n");
		create_bid_history();
		exit(0);
	}
}

void end_auction() {	/* Parent process */
	printf("The auction has ended.\nAt this time a file of bidding history will be created.\n");
	create_bid_history();
	exit(0);
}

void quitter() {	/* Parent process */
	num_bidders--;
	if(num_bidders == 1) printf("1 bidder remaining\n");
	else printf("%d bidders remaining\n", num_bidders);
	if(num_bidders < 1) end_auction();
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sighandler);
	signal(SIGCHLD, quitter);

	auction_started = 1;

	int sockfd, newsockfd, portno, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			  sizeof(serv_addr)) < 0) 
			  error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	while (1) {
		newsockfd = accept(sockfd, 
				(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
		pid = fork();
		if (pid < 0)
			error("ERROR on fork");
		if (pid == 0)  {
			close(sockfd);
			ppid = getppid();
			cpid = getpid();
			printf("\nA new user has connected to the auction.\n");
			while (1) dostuff(newsockfd);
			exit(0);
		} else {
			close(newsockfd);
			if (num_bidders < 0) num_bidders = 1;
			num_bidders++;
		}
	} /* end of while */
	close(sockfd);
	return 0; /* we never get here */
}

/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It deals with the client's
 request, whether for bidding, viewing data, or
 quitting.
 *****************************************/
void dostuff (int sock)
{
	char *paddleno;
	int n;
	int has_msg = 0;
	char buffer[SIZEBUFF];
	char msg_out[SIZEBUFF]; //should be enough space...
	 
	bzero(buffer,SIZEBUFF);
	n = read(sock,buffer,SIZEBUFF-1);
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);

	if (strcmp(buffer, "2") == 0) {		// next three lines newly added
		has_msg = 1;
		printf("in req info mode\n");
		// retrieve the necessary info
		
		// okay, this is yucky but is just the copy/paste of code from writing.h
		char last_bid[SIZEBUFF]; //hopefully enough space
		char new_char;
		int index = 0;

		FILE *fp;
		fp = fopen(bidfile, "r");
		fseek(fp, 0, SEEK_END);
		new_char = fgetc(fp);
		while (new_char != '\n') {
			last_bid[index] = new_char;
			index++;

			fseek(fp, -1*index, SEEK_END);
			new_char = fgetc(fp);
		}
		fclose(fp);
		last_bid[index] = '\0';
		printf("last_bid (backwards) is %s\n", last_bid);
	
		/* FLIP THE STRING AAAAACK *flips table* */
		reverse(last_bid);
		printf("last_bid (forwards?) is %s\n", last_bid);
		sprintf(msg_out, "%s", last_bid);

	} else if (strcmp(buffer, "3") == 0) {
		printf("in quit mode; a user has left the bidding\n");
	//	num_bidders--;
	//	printf("remaining bidders: %d\n", num_bidders);
		// here keep track of users still around, for end-of-auction condition.

	} else {
		strcpy(paddleno, buffer);
		//retrieve new bid
		bzero(buffer,SIZEBUFF);
		n = read(sock, buffer, SIZEBUFF-1); // works based on print statement
		printf("should contain new bid: %s\n", buffer);
		if (n < 0) error("ERROR reading from socket");

		write_bid(buffer, paddleno);
		printf("success_write = %d\n", success_write);

		if (success_write) 
			sprintf(msg_out, "BID SUCCESSFUL.");
		else 
			sprintf(msg_out, "UNSUCCESSFUL: You have already been outbid.");
		has_msg = 1;
	
	} //newly added, same goes for below
	printf("msg_out = %s\n", msg_out);
	if (has_msg) write(sock, msg_out, SIZEBUFF-1);

	bzero(msg_out, sizeof(msg_out));
	success_write = 1;
}

void write_bid(char *offer, char* pno) {
	signal(SIGCHLD, SIG_IGN);
	int status;
	int fd;
	// now start writing.
	//open bid
	int f = fork();
	if (f == 0) {
		char *creat[3] = {"./control", "-c", NULL};
		int f0 = fork();
		if (f0 == 0) {
			execvp(creat[0], creat);
			exit(0);
		}
		else {
			wait(&status);
		}
		// write the bids
		file_write(offer, pno);

		// close bid info
		char *remv[3] = {"./control", "-r", NULL};
		int f2 = fork();
		if (f2 == 0) {
			execvp(remv[0], remv);
			exit(0);
		} else {
			wait(&status);
		}
	} else {
		wait(&status);
	} 
	signal(SIGCHLD, quitter);
}

void create_bid_history() {
	char line1[256];
	char line2[256];
	char line3[256];

	FILE *bh_p = fopen("bid_history.txt", "a");
	FILE *bp = fopen(bidderfile, "r");
	FILE *cbp = fopen(bidfile, "r");

	// read 0
	fscanf(cbp, "%s", line2);
	
	while (fscanf(bp, "%s", line1) == 1) {
		fscanf(cbp, "%s", line2);
		fprintf(bh_p, "%s: %s\n", line1, line2);
	}

	fclose(bh_p);
	fclose(bp);
	fclose(cbp);
}
