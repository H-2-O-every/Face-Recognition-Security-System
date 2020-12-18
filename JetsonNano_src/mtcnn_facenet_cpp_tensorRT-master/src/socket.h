#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

#define MAX_FILENAME 100
#define PORT		 3390

void error_handling(const char *message);
void socketInit(int *serv_sock, struct sockaddr_in *sck_addr, int port);
void bind_and_listen(int serv_sock, struct sockaddr_in sck_addr);
void client_socket_process(int *clnt_sock, int serv_sock, socklen_t *clnt_addr_size, struct sockaddr_in *clnt_addr);
void socket_process(int clnt_sock, int serv_sock, const char *file_abslocation, char framefilename[]);
