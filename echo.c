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
	int fl = 0;
	input[strlen(input)-1]='\0';    //Removing the last newline character
	do{
		for(i=0;input[i]!='\0';i++){
			if(input[i]=='\\'&&!isalpha(input[i+1])){      //checking if a character has to be escaped
				i++;
				putchar(input[i]);
			}
			else if(input[i]!='\"')                  //Managing double quotes
				putchar(input[i]);
			else if(fl==0)
				fl = 1;
			else
				fl = 0;
		}
		puts("\n");
		if(fl == 1){                                 //Asking for more input if quotes are not balanced
			puts(">");
			input = (char *)malloc(MAX_LENGTH*sizeof(char));
			fgets(input,MAX_LENGTH,stdin);
			input[strlen(input)-1]='\0';
		}
	}while(fl == 1); 

}
