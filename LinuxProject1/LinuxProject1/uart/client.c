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
#include <pthread.h>

#define PORT 		5000         		// The port which is communicate with server
#define LENGTH 	20          		// Buffer length
typedef unsigned char u8;
typedef unsigned int u16;

/******************************CRC_CCITT��ʽ��*********************************/
unsigned int crc_tab[16] =  {
	0x0000,
	 0x1021,
	 0x2042, 
	0x3063,
	 0x4084,
	 0x50a5,
	 0x60c6, 
	0x70e7,
	0x8108,
	 0x9129,
	 0xa14a,
	 0xb16b,
	 0xc18c,
	 0xd1ad,
	 0xe1ce,
	 0xf1ef,
};

unsigned char card_id[4];
unsigned int crc_get;

pthread_t pthreadid_read_card;
unsigned int cal_CRC(unsigned char *ptr, unsigned char len);//crcУ��
void * pthread_read_card(void *arg);

  //  int sockfd;                        	// Socket file descriptor
int num=10;              		// Counter of received bytes 
int i;
int id; 
int len;
char revbuf[LENGTH];

int main(int argc, char *argv[])
{
	int err;
	id = uart_init();
	err=pthread_create(&pthreadid_read_card, NULL, pthread_read_card,NULL);	
	if (err != 0)
	{
		printf("can's create thread for read card");
	}
	while (1)
		;
    return (0);
}

/**********************************************************
������uint16_t cal_CRC(uint8_t *ptr,uint8_t len)
�βΣ�ptr:�β�����:unsigned char*���βι��ܣ���У�����ݵ��׵�ַ
      len:�β�����:unsigned char;  �βι��ܣ���У�����ݵĳ���
���أ��������ͣ�unsigned int;
���ܣ�CRC_CCITT����У�麯��
***********************************************************/
unsigned int cal_CRC(unsigned char *ptr, unsigned char len)
{
	unsigned int crc;
	unsigned char  dat;   
	crc = 0x00;
	while (len-- != 0)
	{
		dat   = ((unsigned char)((crc / 256)) / 16 & 0x000f);  
		crc   = (unsigned int)((crc << 4) & 0xfff0);         
		crc  ^= crc_tab[dat ^ (*ptr / 16)];             
		dat   = ((unsigned char)((crc / 256)) / 16 & 0x000f);  
		crc   = (unsigned int)((crc << 4) & 0xfff0);         
		crc  ^= crc_tab[dat ^ (*ptr & 0x0f)];            
		ptr++;
	}
	return (crc);
}

void *
pthread_read_card(void *arg)
{
	while (1)
	{
		
		len = read(id, revbuf, num);
		if (len > 0)
		{
			for (i = 0;i < len;i++)
			{
				printf("%x", revbuf[i]);//��16���ƴ�ӡ����
			}
			card_id[0] = revbuf[2];
			card_id[1] = revbuf[3];
			card_id[2] = revbuf[4];
			card_id[3] = revbuf[5];
			for (i = 0;i < 4;i++)
			{
				printf("  %x  ", card_id[i]);//��16���ƴ�ӡ����
			}
			crc_get = cal_CRC(card_id, 4);//crcУ��
			if (
			    (revbuf[0] == 0x55)
				 &&(revbuf[1] = 0xaa)
				 &&(revbuf[8] = 0xaa)
				 &&(revbuf[9] = 0x55)
			     &&(((u8)(crc_get >> 8)) == revbuf[6])
			     &&(((u8)crc_get) == revbuf[7]))
			{
				printf(" ok ");
				
			}
			printf("\n");        //printf���ڻ�����������һ�����Ŵ�ӡ�������������з���ֱ�Ӵ�ӡ
			write(id, revbuf, num);		
		}
	    
		
	}
}