/* Author: Hemanth Kumar Veeranki */
/* Nick: Harry7 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <signal.h>
#include <proccache.h>
#include <cd.h>
#include <implement.h>
#include <echo.h>

#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_LENGTH 1024
#define proc_range 32768

char cache[proc_range+10][20];

typedef void (*sighandler_t)(int);

char *usrname,*token,*homedir,*hostname,*a[102],*mypath,*origin;
char delim[2]=" ";
char cmd[MAX_LENGTH],cmd_given[MAX_LENGTH],curdir[MAX_LENGTH];

void handle_signal(int signo){
	printf("\n%s @ %s: ",usrname,hostname);
	if ( strlen(homedir)>strlen(curdir)) printf("%s $ ",curdir);
	else  printf("~%s $ ",curdir+strlen(homedir));
	fflush(stdout);
}

int main(int argc,char* argv[],char* envp[]){

	/* Variable and stuff */

	int bgflag=0,index,i,mysignal,cmd_len,c;
	pid_t pid;
	char tmp1[100],tmp2[100];

	/* Doing Malloc */

	origin=(char *)malloc(MAX_LENGTH*sizeof(char));
	mypath=(char *)malloc(MAX_LENGTH*sizeof(char));
	usrname=(char*)malloc(MAX_LENGTH*sizeof(char));
	token=(char*)malloc(MAX_LENGTH*sizeof(char));
	homedir=(char*)malloc(MAX_LENGTH*sizeof(char));
	hostname=(char *)malloc(MAX_LENGTH*sizeof(char));

	/* Gettting env variable */

	getcwd(homedir,MAX_LENGTH);

	usrname=getenv("USER");
	gethostname(hostname,MAX_LENGTH);

	signal(SIGINT, SIG_IGN);
	signal(SIGINT, handle_signal);

	while(1){

		bgflag=0;
		getcwd(curdir,MAX_LENGTH);
		bzero(cmd,MAX_LENGTH);

		while( (pid=waitpid(-1,&mysignal,WNOHANG))>0){
			printf("Process %s with PID :",cache[pid]);
			printf(" %d ",pid);
			puts("Exitted\n");
		}


		printf("%s @ %s: ",usrname,hostname);

		if ( strlen(homedir)>strlen(curdir)) printf("%s $ ",curdir);

		else  printf("~%s $ ",curdir+strlen(homedir));


		if(!fgets(cmd_given,MAX_LENGTH,stdin))break;

		index=0;

		while(cmd_given[index]!='\0'){

			i=0;

			while(cmd_given[index]!=';' && cmd_given[index]!='\n') {
				cmd[i]=cmd_given[index];
				i++;
				index++;
			}
			bzero(mypath,MAX_LENGTH);
			index++;
			cmd[i++]='\n';
			cmd[i++]='\0';

			cmd_len=strlen(cmd);

			if (cmd[cmd_len-2]=='&'){
				bgflag=1;
				cmd[cmd_len-2]='\0';
			}

			token= (char *)strtok(cmd,delim);

			if (token[strlen(token)-1]=='\n')
				token[strlen(token)-1]='\0';

			if (strcmp(cmd,"echo")==0){
				implement_echo(cmd+strlen(token)+1);
			}

			else if ((strcmp(cmd,"cd"))==0){
				token=strtok(NULL," \n");
				if(token!=NULL)
					implement_cd(token,homedir,argv[0],cmd,curdir);
				else chdir(homedir);
			}
			else if (strcmp(cmd,"exit")==0 ){
				return 0;
			}
			else{
				pid=fork();
				if (pid==0){
					//Child Process
					implement_command(a,token);
				}
				else{
					//Parent Process
					storeproc(pid,cache);
					if (bgflag==0)
						waitpid(pid,&mysignal,0);	
				}
			}
		}
	}
	printf("exit\n");
	return 0;
}
