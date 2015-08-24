/* Author: Hemanth Kumar Veeranki */
/* Nick: Harry7 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <proccache.h>
#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_LENGTH 1024
#define DELIMS " \t\r\n"
void storeproc(int pid, char cache[][20]){
	char tmp1[1024]={'\0'},tmp2[1024]={'\0'};
	int i=0;
	char c;
	//printf("PID : %d\n",pid);
	sprintf(tmp1,"/proc/%d/cmdline",pid);
	FILE *filep=fopen(tmp1,"r");
	long long int lag=1000000;
	while(lag--);
	while((c=fgetc(filep))!=EOF){
		tmp2[i++]=c;
	}
	strcpy(cache[pid],tmp2);
	return ;
}
