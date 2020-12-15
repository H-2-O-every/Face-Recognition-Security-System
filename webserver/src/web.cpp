#include "web.h"

void data(char name[100], char time[100], char sz_data[100]){
	char* ptr;
	ptr = strtok(time, ".");
	char data[100];
	sprintf(data, "<pre>%s	%s <br></pre>", ptr, name);
	strcpy(sz_data, data);
}

void update(char sz_data[100]){
	FILE* fp = fopen("test.html", "r+");
	char buf[64];
	fgets(buf, sizeof(buf), fp);
	while(strcmp(buf, "</body>\n")!=0){
		fgets(buf, 64, fp);
	}
	fseek(fp, -8, SEEK_CUR);
	fputs(sz_data, fp);
	fputs("\n</body>\n", fp);
	fputs("</html>\n", fp);
	fclose(fp);
}

