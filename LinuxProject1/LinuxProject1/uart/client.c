#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 		5000         		// The port which is communicate with server
#define LENGTH 	2          		// Buffer length

int main(int argc, char *argv[])
{
  //  int sockfd;                        	// Socket file descriptor
    int num;              		// Counter of received bytes 
	int i;
	int id; 
	int len;
	id=uart_init();
	char revbuf[LENGTH];
	revbuf[0] ='a' ;
	revbuf[1] = 'b';
	num = 1;
	while(1)
	{
		len=read(id, revbuf, num);
		if (len > 0)
		{
		
				printf("%c", revbuf[i]);
				write(id, revbuf, num);
			
		}
	
		
		//printf("num is %d\n",num);
	}
		
    return (0);
}

