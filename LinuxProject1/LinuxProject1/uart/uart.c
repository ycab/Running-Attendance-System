#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <limits.h>
#define DEV_NAME        "/dev/ttySP0" 
int uart_init(void) 
{ 
	int iFd;
	int len;
	unsigned char ucBuf[1000];
	struct termios opt;

	iFd=open(DEV_NAME,O_RDWR|O_NOCTTY);
	if(iFd<0)
	{
		perror(DEV_NAME);
		return -1;
	}
	tcgetattr(iFd, &opt);
        if(tcgetattr(iFd,&opt)<0)
	{
		return -1;
	};
	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);
	opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG); 
	opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON); 
	opt.c_oflag &= ~(OPOST); 
	opt.c_cflag &= ~(CSIZE | PARENB); 
	opt.c_cflag |= CS8; opt.c_cc[VMIN] = 255; 
	opt.c_cc[VTIME] = 150;
	if (tcsetattr(iFd, TCSANOW, &opt)<0) 
	{
		 return -1; 
	}
	tcflush(iFd,TCIOFLUSH); 
/*	for (i = 0; i < 1000; i++)
	{ 
		ucBuf[i] = 0xff - i; 
	} 
	write(iFd, ucBuf, 0xff); 
	len = read(iFd, ucBuf, 0xff); 
	printf("get date: %d \n", len); 
	for (i = 0; i < len; i++)
	{ 
		printf(" %x", ucBuf[i]); 
	}
	printf("\n"); 
	close(iFd); */
	printf("uart has been opened \n");
	return iFd;
}
