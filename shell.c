/* Author: Hemanth Kumar Veeranki */
/* Nick: Harry7 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>
#include <fcntl.h>
#include <signal.h>
#include <cd.h>
#include <echo.h>

#include <unistd.h>

#define MAX_LENGTH 1024
#define proc_range 32768

char cache[proc_range+10][20];

typedef void (*sighandler_t)(int);

char *usrname,*token,*homedir,*hostname,*a[102],*mypath,*origin;
char delim[4]=" \t\n";
char cmd[MAX_LENGTH],cmd_given[MAX_LENGTH],curdir[MAX_LENGTH];
pid_t shell_pid;

int main(int argc,char* argv[],char* envp[]){

	/* Variable and stuff */

	int bgflag=0,index,i,mysignal,cmd_len,c;
	pid_t pid;
	char tmp1[100],tmp2[100];
	int in,out,out2;
	/* Doing Malloc */

	origin=(char *)malloc(MAX_LENGTH*sizeof(char)); 
	mypath=(char *)malloc(MAX_LENGTH*sizeof(char));
	usrname=(char*)malloc(MAX_LENGTH*sizeof(char));//To store the user name 
	token=(char*)malloc(MAX_LENGTH*sizeof(char)); //To store the tokens
	homedir=(char*)malloc(MAX_LENGTH*sizeof(char));//To store homedirectory
	hostname=(char *)malloc(MAX_LENGTH*sizeof(char));//To store the hostname
	shell_pid=getpid();
	/* Gettting env variable */

	//To get home directory
	getcwd(homedir,MAX_LENGTH);

	usrname=getenv("USER");
	gethostname(hostname,MAX_LENGTH);

	while(1){

		//handling background not yet converted

		bgflag=0;
		//getting current working directory to display prompt

		getcwd(curdir,MAX_LENGTH);
		//Initialisation
		bzero(cmd,MAX_LENGTH);

		//Vague implementation of '&' 
		while( (pid=waitpid(-1,&mysignal,WNOHANG))>0){
			printf("Process %s with PID :",cache[pid]);
			printf(" %d ",pid);
			puts("Exitted\n");
		}

		//Printing Prompt 

		printf("%s @ %s: ",usrname,hostname);

		if ( strlen(homedir)>strlen(curdir)) printf("%s $ ",curdir);

		else  printf("~%s $ ",curdir+strlen(homedir));

		//Taking input and checking EOF condition

		if(!fgets(cmd_given,MAX_LENGTH,stdin)){
			//Quitting
			puts("exit");
			_exit(0);
		}
		if(strcmp(cmd_given,"\n")==0)continue;//Only enter is pressed
		index=0;

		while(cmd_given[index]!='\0'){

			i=0;
			//Splitting command with respect to ;

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


			//Tokenising

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
			else if (strcmp(cmd,"pwd")==0){
				printf("%s\n",curdir);
			}
			else if (strcmp(cmd,"exit")==0 ){
				//Quitting
				_exit(0);
			}
			else{
				if (cmd[strlen(cmd)-2]=='&'){
					bgflag=1;
					cmd[cmd_len-2]='\0';
				}
				pid=fork();
				if (pid==0){
					in=out=out2=0;
					//Child Process
					int tmp=0;
					//Storing the arguments given in an array of strings 
					while(token!=NULL){
						a[tmp]=(char *)malloc(strlen(token)*sizeof(char));
						if (strcmp(token,"<")==0){
							in=1;
						}
						else if (in==1 && (strcmp(token,"<")==0 || strcmp(token,">")==0||strcmp(token,">>")==0)){
							printf("syntax near unexpected token \'<\'\n");
						}
						else if ((out==1 || out2==1 ) && (strcmp(">",token)==0 ||strcmp("<",token)==0||strcmp(">>",token)==0)){
							printf("syntax near unexpected token \'>\'\n");
						}
						else if (in!=1 && out2!=1 && strcmp(token,">")==0){
							out=1;
						}
						else if (in!=1 && out!=1 && out2!=1 && strcmp(token,">>")==0){
							out2=1;
						}
						else if (in==1){
							int fd=open(token,O_RDONLY,0);
							dup2(fd,0);
							close(fd);
							in=0;
						}
						else if (out==1){
							int fd1=creat(token,0644);
							dup2(fd1,1);
							close(fd1);
							out=0;
						}
						else if (out2==1){
						int fd2=open(token,O_RDONLY | O_WRONLY|O_APPEND);
						dup2(fd2,1);
						close(fd2);
						out2=0;
						}
						else{

							strcpy(a[tmp++],token);

						}
						//tokenising
						token=(char *)strtok(NULL," \n");
					}
					//indicating end of command
					if (strcmp(a[0],"ls")==0){
						a[tmp]=(char *)malloc(20*sizeof(char));
						strcpy(a[tmp++],"--color=auto");
					}

					a[tmp]=NULL;
					//calling execvp
					int i,err=execvp(a[0],a);
					//checking error cases

					if (err==-1 && errno==2 )fprintf(stderr,"%s: command not found\n",a[0]);
					for (i=0;i<tmp;i++)free(a[i]);
					_exit(0);
				}

				else{

					//Parent Process
					int i=0;
					char c;
					//Opening file in proc folder
					sprintf(tmp1,"/proc/%d/cmdline",pid);

					FILE *filep=fopen(tmp1,"r");
					//giving it a sleep to get correct buffer pid
					usleep(10000);
					//storing name into cacahe
					while((c=fgetc(filep))!=EOF){
						tmp2[i++]=c;
					}
					strcpy(cache[pid],tmp2);
					if (bgflag==0)
						waitpid(pid,&mysignal,0);	
				}
			}
		}
	}
	return 0;
}
