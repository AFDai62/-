#include        <sys/types.h>   
#include        <sys/socket.h>  
#include        <sys/time.h>    
#include        <time.h>          
#include        <netinet/in.h>  
#include        <arpa/inet.h>   
#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include 	<unistd.h>

#define  SERVER_PORT     5001   /* ���M���̑҂��󂯃|�[�g�ԍ� */
#define  MAXPKTLEN       1004   /* �ő�p�P�b�g��(�o�C�g)     */

int main(int argc, char **argv)
{
	int			sockfd;            /*  �\�P�b�g�ݒ��  */
	struct sockaddr_in	servaddr, cliaddr; /*  �K�v�ȕϐ�    �@*/
	socklen_t len;
	int port_number;
	
	char recvpkt[MAXPKTLEN];    /* ��M�p�P�b�g�S�̂��i�[����z��ϐ�        */
	char recvdata[MAXPKTLEN-4]; /* ��M�p�P�b�g�̃f�[�^�����i�[����z��ϐ�   */
	char sendpkt[4];             /* ACK �p�P�b�g�S�̂��i�[����4�o�C�g�̔z��ϐ� */ 
	int seq_num = 0;               /* ��M�p�P�b�g�̏����ԍ����i�[����ϐ�       */
	int num_bytes_pkt = 0;         /* ��M�p�P�b�g��                             */
	int num_bytes_data = 0;        /* ��M�p�P�b�g�̃f�[�^���̒���               */
	int total_num_bytes_data = 0;

	FILE *fp;
	char filename[50];

	/* UDP�̃\�P�b�g��ݒ� */

	port_number = SERVER_PORT;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(port_number);
	len			 = sizeof(cliaddr);

	while (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	 servaddr.sin_port = htons(++port_number);
	
	printf("PORT NUMBER : %d\n", port_number);
	
	
	while(1){

		/* ���肩��p�P�b�g���P��M���đS�̂�recvpkt�Ɋi�[ */
		/* ��M�p�P�b�g���� num_bytes_pkt�Ɋi�[�����       */
		num_bytes_pkt = recvfrom(sockfd, recvpkt, MAXPKTLEN, 0, 
										(struct sockaddr *)&cliaddr, &len);
		
		/* ��M�p�P�b�g�� - 4 �� �p�P�b�g�̃f�[�^���̒��� */
		num_bytes_data = num_bytes_pkt - 4;

		/* ��M�p�P�b�g�̐擪4�o�C�g(�w�b�_��)��seq_num�ɃR�s�[ */
		memcpy((char *)&seq_num,recvpkt,4); 

		/* ��M�p�P�b�g�̐擪����5�o�C�g�ڈȍ~�����܂�(�f�[�^��)��  */
		/* filename�ɃR�s�[                                         */
		memcpy(filename,recvpkt+4,num_bytes_data); 
		printf("Receive : %d bytes, seq_num=%d\n",num_bytes_data,seq_num);
		
		/* sendpkt(����4�o�C�g)��seq_num(����4�o�C�g)�̓��e���R�s�[ */
		memcpy(sendpkt,(char *)&seq_num,4);

		/* sendpkt�̓��e��ACK�p�P�b�g�Ƃ��đ���ɑ��M */
    	sendto(sockfd, sendpkt, 4, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		printf("ACK with seq_num %d is sent : \n",seq_num);
				
		/* ��M�����t�@�C�����Ńp�P�b�g�̃f�[�^���������o���t�@�C�����I�[�v�� */
		fp = fopen(filename,"wb");

		if (fp == NULL){
			printf("File open error: test.jpg\n");
			return -1;
		}

		while(1){
			
			/* ���肩��p�P�b�g���P��M���đS�̂�recvpkt�Ɋi�[ */
			/* ��M�p�P�b�g���� num_bytes_pkt�Ɋi�[�����       */
			num_bytes_pkt = recvfrom(sockfd, recvpkt, MAXPKTLEN, 0, 
										(struct sockaddr *)&cliaddr, &len);

			/* ��M�p�P�b�g�� - 4 �� �p�P�b�g�̃f�[�^���̒��� */
			num_bytes_data = num_bytes_pkt - 4;
			total_num_bytes_data += num_bytes_data;

			/* ��M�p�P�b�g�̐擪4�o�C�g(�w�b�_��)��seq_num�ɃR�s�[ */
			memcpy((char *)&seq_num,recvpkt,4); 

			/* ��M�p�P�b�g�̐擪����5�o�C�g�ڈȍ~�����܂�(�f�[�^��)��  */
			/* recvdata�ɃR�s�[                                         */
			memcpy(recvdata,recvpkt+4,num_bytes_data); 
			printf("Receive : %d bytes, seq_num=%d\n",num_bytes_data,seq_num);

			/* sendpkt(����4�o�C�g)��seq_num(����4�o�C�g)�̓��e���R�s�[ */
			memcpy(sendpkt,(char *)&seq_num,4);

			/* sendpkt�̓��e��ACK�p�P�b�g�Ƃ��đ���ɑ��M */
    		sendto(sockfd, sendpkt, 4, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
			printf("ACK with seq_num %d is sent : \n",seq_num);

			/* recvdata �̓��e�� �t�@�C���ɏ����o�� */
			fwrite(recvdata,num_bytes_data,1,fp);
			
			if(seq_num == -1){
				fclose(fp);
				break;
			}else if(seq_num == 0){
				fclose(fp);
				break;
			}
		}
		if(seq_num == 0){
			break;
		}
	}

	close(sockfd);

	printf("Total amount of received data : \
				%d bytes\n",total_num_bytes_data);

	return 0;
}
