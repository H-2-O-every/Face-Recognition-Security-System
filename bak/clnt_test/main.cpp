#include "socket_clnt.h"

#define BSIZ 64
#define IMGLEN 100

int main( int argc, char* argv[] ) {
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;
	int fd = -1;
	FILE* IR;
	IR = fopen("IRflag", "w");
	fprintf(IR, "0");
	fclose(IR);

//	fd = open("/dev/LED_IRQ", O_RDWR);	
	in_notice(argc, argv[0]);
	
	printf("main : init start\n");
//	sockInit(&sock, &serv_addr, argv);
//	printf("main : init end\n");

	printf("main : connect start\n");
//	sockConnect(sock, serv_addr);
//	printf("main : connect end\n");

	char image_name[IMGLEN] = {"\0"};
	int intcount;
	char buf[BSIZ];
	while(1){
		strcpy(image_name, "\0");

//		intcount = read(fd, buf, BSIZ, NULL);
//		getchar();

		IR = fopen("IRflag", "r+");
		fscanf(IR, "%d", &intcount);
//		printf("intcount : %d\n", intcount);
		if(intcount==1){
			printf("capture\n");
			capt_img(image_name);
//			fileAndSockProc(sock, image_name);
			intcount = 0;
			fseek(IR, 0, SEEK_SET);
			fprintf(IR, "0");
		}
		fclose(IR);
	}
	close(sock);
	return 0;
}
