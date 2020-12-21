#include "web.h"

void webdata(char name[STRING_LENGTH], char time[STRING_LENGTH], char sz_data[STRING_LENGTH]){
	printf("Webdata() Start\n");
	char *ptr;
	ptr = strtok(time, ".");
	//printf("ptr = %s\n", ptr);
	char data[STRING_LENGTH];
	sprintf(data, "<pre>%s	%s <br></pre>", ptr+14, name);
	printf("data = %s\n", data);
	strcpy(sz_data, data);
	printf("sz_data = %s\n", sz_data);
}

void webupdate(char sz_data[STRING_LENGTH]){
	FILE *fp = fopen(HTML_FILENAME, "r+");
	if(fp == NULL){
		printf("we can't open the HTML FILE %s\n", HTML_FILENAME);
	}
	char buf[BUFSIZ];
	fgets(buf, sizeof(buf), fp);
	while(strcmp(buf, "</body>\n") != 0){
		fgets(buf, BUFSIZ, fp);
	}
	fseek(fp, -8, SEEK_CUR);
	fputs(sz_data, fp);
	fputs("\n</body>\n", fp);
	fputs("</html>\n", fp);
	fclose(fp);
	system("sudo cp test.html /var/www/html/test.html");
}
