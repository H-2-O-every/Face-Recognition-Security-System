#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>     // socket header
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

#define BSIZE 16384


void error_handling( char* message );

int main( int argc, char* argv[] ) {
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if( argc != 3 ) {
		printf( "Usage : %s <IP> <port>\n", argv[0] );
		exit(1);
	}

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char image_name[64];
	sprintf(image_name, "%d%d%d%d%d%d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	char inst[128] = "raspistill -t 1 -o ";
	strcat(inst, image_name);

//	system(inst);

	sock = socket( PF_INET, SOCK_STREAM, 0 );
	if( sock == -1 )
		error_handling( "socket() error" );

	memset( &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr( argv[1] );
	serv_addr.sin_port = htons( atoi(argv[2]) );

	if( connect( sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1 ){
		error_handling( "connect() error!" );
	}
	strcpy(message, "hello");
//	printf("to server : %x\n", message);
//	write(sock, message, sizeof(message));

	FILE* fp = fopen("testImage.jpg", "r");
//	FILE* fp = fopen(image_name, "r");
	FILE* fp_out = fopen("copyImage.jpg", "w");

	char buf[BSIZE];

	fseek(fp, 0, SEEK_END);
	int fsize = ftell(fp);
		
	printf("file size : %d\n", fsize);
	fseek(fp, 0, SEEK_SET);


	int remain = fsize;
	int b_num = (fsize / BSIZE) + 1;
	write(sock, &fsize, sizeof(int));
//	write(sock, &b_num, sizeof(int));	
	printf("block number : %d\n", b_num);

	int a=0;
/*	
	for(int i=0; i<b_num; i++){
		usleep(2000);
		if(remain < BSIZE){
			fread(buf, remain, 1, fp);
//			write(sock, buf, remain);
			fwrite(buf, remain, 1, fp_out);
		}
		else{
			fread(buf, BSIZE, 1, fp);
//			write(sock, buf, BSIZE);
			fwrite(buf, BSIZE, 1, fp_out);
		}
		remain -= BSIZE;
//		printf("remain : %d\n", remain);
		printf("%d	", ++a);
	}	
*/

	for(int i=0; i<fsize; i++){
		fread(buf, 1, 1, fp);
//		usleep(10);
		write(sock, buf, 1);
	}


	str_len = read( sock, message, sizeof(message)-1 );
	if( str_len == -1 )
		error_handling( "read() error!" );

	printf( "Message from server : %s\n", message );
	close( sock );
	fclose(fp);
	fclose(fp_out);
	return 0;
}

void error_handling( char* message ) {
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit(1);
}
