#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

void in_notice(int argc, char* argv);
void capt_img(char image_name[100]);
void sockInit(int* sock, struct sockaddr_in* serv_addr, char* argv[]);
void sockConnect(int sock, struct sockaddr_in serv_addr);
void fileAndSockProc(int sock, char image_name[100]);
void error_handling( char* message);
void rm_img_dir(char path[100]);//you must be careful because this func_ use [rm] with option [-rf]
