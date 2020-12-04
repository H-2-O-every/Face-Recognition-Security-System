#include "socket_clnt.h"

#define BSIZE 1350

void in_notice(int argc){
	if( argc != 3 ) {
		printf( "Usage : %s <IP> <port>\n", argv[0] );
		exit(1);
	}
}

void capt_img(char image_name[100]){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char image_name[100];
	sprintf(image_name, "capture_image/%d%d%d%d%d%d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	char inst[128] = "raspistill -t 1 -w 2400 -q 7 -o ";
	strcat(inst, image_name);

	system(inst);

	char inst2[128];
	sprintf(inst2, "convert -resize \"20%\" %s %s", image_name, image_name);
	system(inst2);
}

void sockInit(int* sock, struct sockaddr_in* serv_addr, int port){
	*sock = socket( PF_INET, SOCK_STREAM, 0 );
	if( sock == -1 )
		error_handling( "socket() error" );

	memset( serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr( argv[1] );
	serv_addr.sin_port = htons( atoi(argv[2]) );
}

void sockConnect(int sock, struct sockaddr_in serv_addr){
	if( connect( sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1 ){
		error_handling( "connect() error!" );
	}
}

void fileAndSockProc(int sock, char image_name[100]){
	FILE* fp = fopen(image_name, "r");

	char buf[BSIZE];

	fseek(fp, 0, SEEK_END);
	int fsize = ftell(fp);
		
	printf("file size : %d\n", fsize);
	fseek(fp, 0, SEEK_SET);


	int remain = fsize;
	int b_num = (fsize / BSIZE) + 1;
	write(sock, image_name, sizeof(char)*100);
	write(sock, &b_num, sizeof(int));	
	printf("block number : %d\n", b_num);

	int charity=0;
	int block_siz_check;
	
	for(int i=0; i<b_num; i++){
		if(remain > BSIZE){
			fread(buf, BSIZE, 1, fp);
			block_siz_check = BSIZE;
			write(sock, &block_siz_check, sizeof(int));
	LOOP1:
			write(sock, buf, BSIZE);
			read(sock, &charity, sizeof(int));
//			printf("charity %d : %d\n",i, charity);
			if(charity == 0){
				goto LOOP1;
			}
			charity = 0;
			remain -= BSIZE;
		}
		else{
			fread(buf, remain, 1, fp);
			block_siz_check = remain;
			write(sock, &block_siz_check, sizeof(int));
	LOOP2:
			write(sock, buf, remain);
			read(sock, &charity, sizeof(int));
			if(charity == 0){
				goto LOOP2;
			}
			charity = 0;
		}
	}

	close(sock);
	fclose(fp);	
}

void error_handling( char* message ) {
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit(1);
}

