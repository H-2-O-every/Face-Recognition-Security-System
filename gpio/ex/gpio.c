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


fd = open("/dev/LED_IRQ", O_RDWR | O_NDELAY);

if(fd >= 0)
{
printf("start..\n");
buf[0] = '1';
write(fd, buf, 1, NULL);

printf("wait.. input\n");

while(1)
{
memset(intbuffer, 0, sizeof(intbuffer));
intcount = read(fd, intbuffer, sizeof(R_INT_INFO), NULL) / sizeof(R_INT_INFO);
if(intcount)
{
break;
}
}

printf("input ok..\n");

sleep(1);

memset(intbuffer, 0, sizeof(intbuffer));

printf("read interrupt time\n");
intcount = read(fd, intbuffer,sizeof(intbuffer),NULL) / sizeof(R_INT_INFO);

for(loop = 0; loop < intcount; loop++)
{
printf("index = %d time = %1d\n", loop, intbuffer[loop].time);
}

printf("led flashing..\n");

for(loop = 0; loop < 5; loop++)
{
buf[0] = '1';
write(fd, buf, 1, NULL);
sleep(1);
buf[0] = '0';
write(fd, buf, 1, NULL);
sleep(1);
}


close(fd);
}

return 0;
}