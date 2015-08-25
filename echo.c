/* Author: Hemanth Kumar Veeranki */
/* Nick: Harry7 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<echo.h>

#ifdef _WIN32
#include <windows.h>
#define chdir _chdir

#else
#include <unistd.h>
#endif

#define MAX_LENGTH 1024

void implement_echo(char *input){
	int i;
	int fl = 0,fl2=0;

	if (input[0]=='$'){
		printf("%s\n",getenv(input+1));
		return ;
	}
	input[strlen(input)-1]='\0'; //Removing the terminating newline character
	do{
		for(i=0;input[i]!='\0';i++){
			if(input[i]=='\\'){      //checking if escaping has been assked
				i++;
				putchar(input[i]);
			}
			else if((input[i]!='\"'||fl2==1)&&(input[i]!='\''||fl==1))			//Managing double quotes
				putchar(input[i]);
			else if(input[i]=='\"')
				if(fl==0) fl = 1;
				else fl = 0;
			else 
				if(fl2 == 0) fl2 = 1;
				else  fl2 = 0;
		}
		putchar('\n');
		if(fl == 1){                                 //Asking for more input if quotes are not balanced
			putchar('>');
			fgets(input,MAX_LENGTH,stdin);
			input[strlen(input)-1]='\0'; //Same as done above

		}
	}while(fl == 1); 
	return ;
}
