#include "web.h"
#include <time.h>

int main(){
	char sz_name[100] = "kim_dongwon";
	char sz_time[100];
	char sz_data[100];

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(sz_time,"%d-%02d-%02d-%02d-%02d-%02d-%d.jpg", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, 1);

	data(sz_name, sz_time, sz_data);

	update(sz_data);
	system("sudo cp test.html /var/www/html/test/test.html");

	return 0;
}
