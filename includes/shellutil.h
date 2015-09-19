#ifndef SHELLUTIL_H
#define SHELLUTIL_H

void print_command(pid_t pid,char tmp1[],unsigned char buffer[]);
void print_prompt(char* usrname,char* hostname,char* homedir,char *curdir);
void cache_store(char temp[],pid_t pid,char cache[32778][20]);
#endif
