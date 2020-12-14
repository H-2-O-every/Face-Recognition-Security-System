#include "socket_clnt.h"

#define GPIO_LED 24

int main( int argc, char* argv[] ) {
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;
	
	in_notice(argc, argv[0]);
	
	printf("main : init start\n");
	sockInit(&sock, &serv_addr, argv);
	printf("main : init end\n");

	printf("main : connect start\n");
	sockConnect(sock, serv_addr);
	printf("main : connect end\n");
//while(1){	
	char image_name[100] = {"\0"};

	capt_img(image_name);
	
//	sockInit(&sock, &serv_addr, argv);
	
//	sockConnect(sock, serv_addr);
	
	fileAndSockProc(sock, image_name);
//}
	close(sock);
	return 0;
}
