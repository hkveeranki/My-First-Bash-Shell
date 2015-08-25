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
	//Temporary variables 	
	char *tmp1=(char*)malloc(MAX_LENGTH*sizeof(char));
	char *tmp2=(char*)malloc(MAX_LENGTH*sizeof(char));
	int i=0;
	char c;
	//Opening file in proc folder
	sprintf(tmp1,"/proc/%d/cmdline",pid);
	
	FILE *filep=fopen(tmp1,"r");
	//giving it a sleep to get correct buffer pid
	int lag=10000000;
	while(lag--);
	//storing name into cacahe
	while((c=fgetc(filep))!=EOF){
		tmp2[i++]=c;
	}
	strcpy(cache[pid],tmp2);
	return ;
}
