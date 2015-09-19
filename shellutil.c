/*Author: Hemanth Kumar Veeranki*/
/*Handle : harry7 */

#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cd.h>
#include <echo.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <shellutil.h>

void print_prompt(char* usrname,char* hostname,char* homedir,char* curdir){

	printf("%s @ %s: ",usrname,hostname);
	if ( strlen(homedir)>strlen(curdir)) printf("%s $ ",curdir);
	else  printf("~%s $ ",curdir+strlen(homedir));

}

void print_command(pid_t pid,char tmp1[],unsigned char buffer[]){
	const int BUFSIZE = 4096; 
	sprintf(tmp1,"/proc/%d/cmdline",pid);            
	int fd = open(tmp1, O_RDONLY);
	int nbytesread = read(fd, buffer, BUFSIZE);
	unsigned char *end = buffer + nbytesread;
	unsigned char *p ;
	for (p = buffer; p < end;)
	{
		printf("%s ",p);
		while (*p++); 
	}
	close(fd);
	printf("\n");
}

void cache_store(char temp[],pid_t pid,char cache[32778][20]){
	char ch;
	char *tmp1=(char *)malloc(1024*(sizeof(char)));
	sprintf(tmp1,"/proc/%d/cmdline",pid);
	FILE *fp = fopen(tmp1,"r");
	usleep(10000);
	int m=0;          
	while( ( ch = fgetc(fp) ) != EOF )  temp[m++]=ch;
	temp[m++]='\0';
	strcpy(cache[pid],temp);
	free(tmp1);
}
