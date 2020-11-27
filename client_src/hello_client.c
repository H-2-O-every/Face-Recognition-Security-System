#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>     // socket header
#include <sys/types.h>
#include <netinet/in.h>


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

	char buf[BUFSIZ];

	fseek(fp, 0, SEEK_END);
	int fsize = ftell(fp);
		
	printf("file size : %d\n", fsize);
	fseek(fp, 0, SEEK_SET);


	int remain = fsize;
	int b_num = (fsize / BUFSIZ) + 1;
	write(sock, &b_num, sizeof(int));	
	printf("block number : %d\n", b_num);

	for(int i=0; i<b_num; i++){
		if(remain < BUFSIZ){
			fread(buf, remain, 1, fp);
			write(sock, buf, remain);
		}
		else{
			fread(buf, BUFSIZ, 1, fp);
			write(sock, buf, BUFSIZ);
		}
		remain -= BUFSIZ;
	}	
			

	str_len = read( sock, message, sizeof(message)-1 );
	if( str_len == -1 )
		error_handling( "read() error!" );

	printf( "Message from server : %s\n", message );
	close( sock );
	fclose(fp);
	return 0;
}

void error_handling( char* message ) {
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit(1);
}
