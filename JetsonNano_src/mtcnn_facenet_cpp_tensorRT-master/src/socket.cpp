#include "socket.h"
#include <string.h>

#define BSIZ 16384


void error_handling( const char *message ){
	fputs(message, stderr);
	exit(1);
}

void socketInit(int *serv_socket, struct sockaddr_in *sck_addr, int port){
	*serv_socket = socket( PF_INET, SOCK_STREAM, 0);
	if( *serv_socket == -1){
		error_handling( "Error : socket()\n");
		return;
	}

	memset(sck_addr, 0, sizeof(sck_addr) );
	sck_addr->sin_family	  = AF_INET;
	sck_addr->sin_addr.s_addr = htonl( INADDR_ANY );
	sck_addr->sin_port		  = htons( port );
}

void bind_and_listen(int serv_sock, struct sockaddr_in sock_addr){
	if( bind(serv_sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr) ) == -1 ){
		error_handling( "Error : bind()\n");
		return;
	}
	if( listen(serv_sock, 10) == -1 ){
		error_handling( "Error : listen()\n");
		return;
	}
}

void client_socket_process(int *clnt_sock, int serv_sock, socklen_t *clnt_addr_size, struct sockaddr_in *clnt_addr){
	*clnt_addr_size = sizeof(clnt_addr);
	*clnt_sock = accept( serv_sock, (struct sockaddr*)clnt_addr, clnt_addr_size);
	if(*clnt_sock == -1)
		error_handling( "Error : accept()\n");
}

void socket_process(int clnt_sock, int serv_sock, const char* file_abslocation, char framefilename[]){
	char buf[BSIZ];
	int b_num;
	char filepath[MAX_FILENAME*2];
	char filename[MAX_FILENAME];
	int str_len = 0;

	printf("Read filename\n");
	read(clnt_sock, filename, sizeof(char)*MAX_FILENAME);

	printf("read block number\n");
	read(clnt_sock, &b_num, sizeof(int));
	printf("b_num = %d\n", b_num);
	if(str_len == -1){
		error_handling( "Error : read()\n");
	}
	printf("make the filepath\n");
	//sprintf(filepath, "%s/%s", file_abslocation, filename);
	sprintf(framefilename, "%s/%s", file_abslocation, filename);
	FILE *fp;
	//printf("%s file open\n", filepath);
	printf("%s file open\n", framefilename);
	//fp = fopen(filepath, "wb");
	fp = fopen(framefilename, "wb");

	int block_siz_check;
	int recv_flag = 1;
	int charity = 0;
	int error_cnt = 0;

	printf("start recevie\n");
	for(int i=0; i<b_num; i++){
		read(clnt_sock, &block_siz_check, sizeof(int));
RETRY:
		str_len = read(clnt_sock, buf, sizeof(buf));
		if(str_len == block_siz_check){
			fwrite(buf, str_len, 1, fp);
			charity = 1;
			write(clnt_sock, &charity, sizeof(int));
		} else {
			charity = 0;
			write(clnt_sock, &charity, sizeof(int));
			//i--;
			goto RETRY;
		}
		//printf("i = \n", i);
	}
	printf("end receive\n");
	//close( clnt_sock );
	//close( serv_sock );
	fclose( fp );
}
