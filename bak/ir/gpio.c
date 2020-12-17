#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define INT_BUFF_MAX 64

typedef struct
{
unsigned long time;
} __attribute__ ((packed)) R_INT_INFO;


int main(int argc, char** argv)
{
char buf[5];
int loop;
int fd = -1;

R_INT_INFO intbuffer[INT_BUFF_MAX];
int intcount;


//fd = open("/dev/LED_IRQ", O_RDWR | O_NDELAY);
fd = open("/dev/gpioled", O_RDWR | O_NDELAY);

if(fd >= 0)
{
printf("start..\n");
buf[0] = '1';
write(fd, buf, 1, NULL);
FILE* fp;
fp = fopen("IRflag","w");
fprintf(fp, "0");
fclose(fp);

while(1){
	memset(intbuffer, 0, sizeof(intbuffer));
	intcount = read(fd, intbuffer, sizeof(R_INT_INFO), NULL) / sizeof(R_INT_INFO);
	if(intcount){
		fp = fopen("IRflag", "w");
		fprintf(fp, "%d", 1);
		fclose(fp);
		printf("button in\n");
		intcount = 0;
		sleep(3);
		fp = fopen("IRflag", "w");
		fprintf(fp,"0");
		fclose(fp);
		buf[0]=0;
		write(fd, buf, 1, NULL); 
	}
	intcount=0;
}




close(fd);
}

return 0;
}
