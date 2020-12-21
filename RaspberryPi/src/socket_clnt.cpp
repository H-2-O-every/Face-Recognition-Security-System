#include "socket_clnt.h"
#include<unistd.h>
#define BSIZE 1350
#define ID 1

void in_notice(int argc, char* argv){
	if( argc != 3 ) {
		printf( "Usage : %s <IP> <port>\n", argv );
		exit(1);
	}
}

void rm_img_dir(char path[100]){// you must be careful because this func_ use [rm] with option [-rf]
	char inst[120];
	sprintf(inst, "rm -rf %s", path);
	system(inst);
}

void capt_img(char image_name[100]){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	sprintf(image_name, "%d-%d-%d-%d-%d-%d-%d.jpg", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, ID);

	char inst[256] = "raspistill -t 1 -w 2400 -q 20 -o ";
	char path[32] = "./capture_image/";
	strcat(inst, path);
	strcat(inst, image_name);

	system(inst);

	char inst2[256];
	sprintf(inst2, "convert -resize \"20%\" ./capture_image/%s ./capture_image/%s", image_name, image_name);
	system(inst2);
	printf("%s\n", image_name);
}

void sockInit(int* sock, struct sockaddr_in* serv_addr, char* argv[]){
	*sock = socket( PF_INET, SOCK_STREAM, 0 );
	if( *sock == -1 )
		error_handling( "socket() error" );

	memset( serv_addr, 0, sizeof(serv_addr) );
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_addr.s_addr = inet_addr( argv[1] );
	serv_addr->sin_port = htons( atoi(argv[2])+ID-1 );
}

void sockConnect(int sock, struct sockaddr_in serv_addr){
	if( connect( sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) ) == -1 ){
		error_handling( "connect() error!" );
	}
}

void fileAndSockProc(int sock, char image_name[100]){
	char path[100] = "./capture_image/";
	strcat(path, image_name);
	printf("file open : %s\n", path);
	FILE* fp = fopen(path, "r");

	char buf[BSIZE];

	fseek(fp, 0, SEEK_END);
	int fsize = ftell(fp);
		
	printf("file size : %d\n", fsize);
	fseek(fp, 0, SEEK_SET);


	int remain = fsize;
	int b_num = (fsize / BSIZE) + 1;
	char* ptr;
	printf("%s\n", image_name);
	write(sock, image_name, sizeof(char)*100);
	write(sock, &b_num, sizeof(int));
	printf("block number : %d\n", b_num);

	int charity=1;
	int block_siz_check;
/*	
	for(int i=0; i<b_num; i++){
		if(remain > BSIZE){
			if(charity == 1){
				fread(buf, BSIZE, 1, fp);
			}
			block_siz_check = BSIZE;
			write(sock, &block_siz_check, sizeof(int));
			write(sock, buf, BSIZE);
			read(sock, &charity, sizeof(int));
			printf("charity %d : %d\n",i, charity);
			if(charity == 0){
				i--;
			}
			else if(charity == 1){
				remain -= BSIZE;
			}
			charity = 0;
		}
		else{
			if(charity == 1){
				fread(buf, remain, 1, fp);
			}
			block_siz_check = remain;
			write(sock, &block_siz_check, sizeof(int));
			write(sock, buf, remain);
			read(sock, &charity, sizeof(int));
			printf("charity %d : %d\n", i, charity);
			if(charity == 0){
				i--;
			}
			else if(charity == 1){
				remain -= remain;
			}
		}
	}
*/
	for(int i=0; i<b_num; i++){
		if(remain > BSIZE){
			fread(buf, BSIZE, 1, fp);
			block_siz_check = BSIZE;
			write(sock, &block_siz_check, sizeof(int));
LOOP1:
			write(sock, buf, BSIZE);
			read(sock, &charity, sizeof(int));
		//	printf("charity %d : %d\n",i, charity);
			if(charity == 0){
//				fseek(fp, -BSIZE, SEEK_CUR);
				goto LOOP1;
			}
			charity = 0;
//			fwrite(buf, BSIZE, 1, fp_out);
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
//				fseek(fp, -remain, SEEK_CUR);
				goto LOOP2;
			}
			charity = 0;
//			printf("remain\n");
		}
	}	

//	close(sock);
	fclose(fp);	
}

void get_retVal(int sock){
	int retVal;
	read(sock, &retVal, sizeof(int));
	printf("retVal : %d\n", retVal);
}

void error_handling( char* message ) {
	fputs( message, stderr );
	fputc( '\n', stderr );
	exit(1);
}
