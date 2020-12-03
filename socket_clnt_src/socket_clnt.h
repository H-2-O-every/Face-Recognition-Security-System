#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

void in_notice(int argc);
void capt_img(char image_name[64]);
void sockInit(int* sock, struct sockaddr_in* serv_addr, int port);
void sockConnect(int sock, struct sockaddr_in serv_addr);
void fileAndSockProc(int sock, char image_name[64]);
void error_handling( char* message);
