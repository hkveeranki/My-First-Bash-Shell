/* Author: Hemanth Kumar Veeranki */
/* Nick: Harry7 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <implement.h>

#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_LENGTH 1024

void implement_command(char *a[101],char *token){
	int tmp=0;
	while(token!=NULL){
		a[tmp]=(char *)malloc(strlen(token)*sizeof(char));
		strcpy(a[tmp++],token);
		token=(char *)strtok(NULL," \n");
	}
	a[tmp]=NULL;
	int err=execvp(a[0],a);
	if (err==-1 && errno==2 )fprintf(stderr,"%s: command not found\n",a[0]);
}

