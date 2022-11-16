#include<sys/types.h>  
#include<sys/socket.h>  
#include<sys/time.h>    
#include<time.h>         
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define  SERVER_PORT   5001   /* 着信側のデフォルトの待ち受けポート番号 */
#define  MAXPKTLEN     1004   /* 最大パケット長(バイト)     */

int GetFileSize(FILE *fp);

int main(int argc, char **argv)
{
	int    sockfd;
	struct sockaddr_in	servaddr;
	struct timeval tv;      /* 受信タイムアウトの待ち時間を設定する変数 */

	FILE   *fp;              /* 読み込むファイルの識別子   */
	long   filesize;         /* ファイルサイズ             */

	int  num_bytes_data = 0; /* 送信データの長さ */
	int  send_seq_num  = 0;  /* 送信パケットの順序番号         */
	int  num_bytes_pkt = 0;	 /* 送信するパケットの長さ */
       	
	char sendpkt[MAXPKTLEN];    /* 送信パケット全体 */
	char senddata[MAXPKTLEN-4]; /* パケットのデータ部 */
	char filename[50];
        
	char recvpkt[MAXPKTLEN]; /* 相手から受信するACKパケット全体 */
	int  num_bytes_recv;     /* 相手から受信するACKパケットの長さ */
	
	int port_number = SERVER_PORT;

	int n;		/* 送る写真の数 */
	int i;		/* ループ用 */
	
	if (argc > 3 || argc == 1)
	{
		printf("usage:  <IPaddress> <PORT NUMBER>\n");
		return -1;
	}
	
	if (argc == 3)
    	port_number = atoi(argv[2]);

	/* 送るファイルの数を指定 */
	printf("送るファイル数を入力：");
	scanf("%d",&n);

	/* コマンド実行時に引数として与えるIPアドレスを相手のIPアドレスとして設定 */

	memset(&servaddr, 0, sizeof(servaddr));
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port_number);

	/* UDPのソケットを設定 */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0)
	{
		perror("Socket open error:\n");
		return -1;
	}



	for(i=1; i<=n; i++){


		/* 送るファイル名を指定 */
		printf("送るファイル名を入力：");
		scanf("%s",filename);

		/* 相手に送信する画像ファイルをオープン */
		if (NULL == (fp = fopen(filename,"rb")))
		{
			perror("File open error:");
			return -1;
    	}

		/* 受信タイムアウト時間の設定 (課題2で用いる．課題1では実質使われない) */
 		tv.tv_sec  = 0;      /* 秒単位 */
		tv.tv_usec = 750000; /* マイクロ秒単位 750μs*/
		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

		/* パケットの順序番号を -2と設定 */
		send_seq_num = -2;

		num_bytes_data = strlen(filename)+1;

		/* sendpktの先頭4バイトに順序番号の変数 send_seq_num(4バイト)の内容をコピー */
		memcpy(sendpkt,(char *)&send_seq_num,4);

		/* sendpktの先頭から5バイト以降に，filenameの内容を num_bytes_dataの長さだけコピー */
		memcpy(sendpkt+4,filename,num_bytes_data);

		/* パケットの長さを num_bytes_pktに設定 */
		num_bytes_pkt = num_bytes_data + 4;

		/* ファイル名を送信 */
		/* sendataの先頭から num_bytes_pkt　バイトをUDPで相手に送信 */
		sendto(sockfd, sendpkt, num_bytes_pkt, 0, 
				(struct sockaddr *)&servaddr, sizeof(servaddr));
		printf("Sending data of %d bytes, seq_num=%d....",num_bytes_data,send_seq_num);
		
		/* 相手から ACK パケットを受信 */
		num_bytes_recv = recvfrom(sockfd, recvpkt,MAXPKTLEN,0,NULL,NULL);

		/* ファイルサイズ (バイト数) を取得 */
		filesize = GetFileSize(fp);
		printf("File size is %ld bytes\n",filesize);

		/* ファイルサイズが1000バイトより大きければ1000バイトを，そうでなければ */
		/* ファイルサイズに等しい長さのデータを相手に送ると設定       */
		/* この数を変数 num_bytes_data に格納する                     */
	
		/* パケットの順序番号を 1と設定 */
		send_seq_num = 1;
	

		while(1){
	
			if (filesize > 1000) {
				num_bytes_data = 1000;
				filesize -= 1000;
			}else{ 
				num_bytes_data = filesize;
				send_seq_num = -1;
				if(i == n){
					send_seq_num = 0;
				}
			}
	
			/* num_bytes_dataの長さのデータをファイルから読み込んで */
    		/* senddataに格納                                       */
			fread(senddata, num_bytes_data,1,fp);
		
			/* sendpktの先頭4バイトに順序番号の変数 send_seq_num(4バイト)の内容をコピー */
			memcpy(sendpkt,(char *)&send_seq_num,4);
		
			/* sendpktの先頭から5バイト以降に，senddataの内容を num_bytes_dataの長さだけコピー */
			memcpy(sendpkt+4,senddata,num_bytes_data);
		
			/* パケットの長さを num_bytes_pktに設定 */
			num_bytes_pkt = num_bytes_data + 4;
		
			/* sendataの先頭から num_bytes_pkt　バイトをUDPで相手に送信 */
			sendto(sockfd, sendpkt, num_bytes_pkt, 0, 
				(struct sockaddr *)&servaddr, sizeof(servaddr));
			printf("Sending data of %d bytes, seq_num=%d....",num_bytes_data,send_seq_num);

			/* 相手から ACK パケットを受信 */
			num_bytes_recv = recvfrom(sockfd, recvpkt,MAXPKTLEN,0,NULL,NULL);

			if (num_bytes_recv > 0)    /* num_bytes_recvが0以上であった場合，正常に受信している*/         
				printf("ACK received.\n");
			else if (num_bytes_recv < 0)  /* num_bytes_recvが0以下であった場合，ACKタイムアウト*/
				printf("ACK timeout.\n");
			else			  /* num_bytes_recv = 0の場合はタイムアウト以外のエラー*/
				printf("ACK error.\n");
		
			if (send_seq_num == -1){
				break;
			}else if(send_seq_num == 0){
				break;
			}

			send_seq_num++;
		}
	}

	fclose(fp);
    close(sockfd);
	return 0;
}


int GetFileSize(FILE *fp)    /* 引数で与えるファイルのサイズを出力する関数 */
{
  int filesize;

  fseek(fp,0L,SEEK_END); 
  filesize = ftell(fp);
  rewind(fp);
    
  return filesize;
}
