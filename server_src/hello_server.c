#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>


void error_handling( char* message );

int main( int argc, char* argv[] ) {
	int serv_sock;
	int clnt_sock;
	int ser_len, cli_len;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[30];

	if( argc != 2 ) {
		printf( "Usage : %s <port>\n", argv[0] );
		exit(1);
	}

	serv_sock = socket( PF_INET, SOCK_STREAM, 0 );
	if( serv_sock == -1 )
		error_handling( "socket() error" );

	memset( &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	serv_addr.sin_port = htons( atoi(argv[1]) );

	if( bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr) ) == -1 )
		error_handling( "bind() error");

	if( listen( serv_sock, 5 ) == -1 )
		error_handling( "listen() error" );

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept( serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size );
	if( clnt_sock == -1 )
		error_handling( "accept() error" );

//	int str_len = read(clnt_sock, message, sizeof(message));
	char buf[BUFSIZ];
	int b_num;
	int str_len = read(clnt_sock, &b_num, sizeof(int));
	if( str_len == -1){
		error_handling( "read() error!" );
	}
	printf("block number : %d\n", b_num);


	FILE* fp;
	fp = fopen("testImage.jpg", "w");
//	fp = fopen("test.txt", "w");

	for(int i=0; i< b_num; i++){
		str_len = read(clnt_sock, buf, sizeof(buf));
		fwrite(buf, str_len, 1, fp);
	}
	printf("from client : %s\n", message);
//	printf("clnt to ser : %s\n", message);
	strcpy(message, "bye");
	write( clnt_sock, message, sizeof(message) );
	close( clnt_sock );
	close( serv_sock );

	fclose(fp);

	return 0;
}

void error_handling( char* message ) {
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit(1);
}
