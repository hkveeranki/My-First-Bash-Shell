/* Author: Hemanth Kumar Veeranki */
/* Nick: Harry7 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <cd.h>

#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_LENGTH 1024
void implement_cd(char *path,char *home,char *exe,char *cmd,char *origin){
	char *token=(char*)malloc(MAX_LENGTH*sizeof(char));
	char *mypath=(char*)malloc(MAX_LENGTH*sizeof(char));
	
	if(path[0]=='/'){
		chdir("/");
		strcpy(mypath,"/");
	}
	token=strtok(path,"/");
	while(token!=NULL){
		strcat(mypath,token);
		int err=chdir(token);
		if (err==-1){
			printf("%s: %s: %s: %s\n",exe,cmd,mypath,strerror(errno));
			chdir(origin);
			return ;
		}
		token=strtok(NULL,"/");
	}	
	
}

