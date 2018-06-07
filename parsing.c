#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include "parsing.h"

void trim(char * s)
{
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}

int isAccepted(char* s)
{
    int i=0;
    for(i=0; i<strlen(s); i++)
    {
        if(!isalpha(s[i])&&!isdigit(s[i]))
            return 0;


    }
    return 1;



}
void parsePathArg(char *c,char *path,char *arguments[])
{
    int i=0;

    strcpy(path,strtok(c," \t"));
//arg
    char *token = strtok(NULL," \t");
    for(i=1; token!=NULL; i++)
    {

        arguments[i]=token;
        token = strtok(NULL," \t");

    }
    arguments[0]=path;
    arguments[i]=NULL;
}
void getPATH(char *strings[])
{
    int i;
    strings[0]="";
    strings[1]=strtok(getenv("PATH"),":");

    char *token = strtok(NULL,":");
    for(i=2; token!=NULL; i++)
    {
        strings[i]=token;
        token = strtok(NULL,":");
    }
}
int echo(char *command){
    char *toPrint=malloc(sizeof(char) * 514);
    char *toPrint2=malloc(sizeof(char) * 514);
    strcpy(toPrint,command);
    strcpy(toPrint2,command);
    if(strcmp(strtok(toPrint2," \t"),"echo")==0)
    {
        toPrint+=4;
        trim(toPrint);
        if(toPrint[0]=='\''||toPrint[0]=='\"')
        {
            toPrint++;
        }
        if(toPrint[strlen(toPrint)-1]=='\"'||toPrint[strlen(toPrint)-1]=='\'')
        {

            toPrint[strlen(toPrint)-1]=0;
        }
        printf("%s\n",toPrint);
        return 0;
    }
    return 1;
    }
int cd(char *command){
    char c[600];
    strcpy(c,command);
    char *path=strtok(c," \t");


    if(strcmp(path,"cd")==0)
    {
        if(c[strlen(c)-1] =='&')
        {
            c[strlen(c)-1]=0;
        }
        char *dir=strtok(NULL," \t");
        char homeindir[550];
        strcpy(homeindir,dir);



        if(chdir(homeindir)==0)
        {
            return 0;

        }

        else
        {
            printf("error dir is incorrect or you have no access to %s\n",homeindir);
            return 0;
        }

    }
    return 1;
}
