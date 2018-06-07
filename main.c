#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>
#include "logging.h"
#include "parsing.h"
char vars[2000][514];
int varsLen = 0;
char datavars[2000][514];
int main( int argc, char *argv[] )
{
    //int argc=0;
    //char *argv[1];
    //argv[0]="/home/mohab/r.txt";
    if (argc == 2)
    {
        batch(argv[1]);

    }
    else if (argc == 1)
    {
        interactive();
    }
    else
    {
        printf("error wrong parameter argument\n");
        interactive();
    }
    return 0;
}
void batch(char *arg){
        FILE *fp;
        char str[1024];

        /* opening file for reading */
        fp = fopen(arg, "r");
        if(fp == NULL)
        {
            perror("Error opening file");
            return;
        }
        while( fgets (str,1024, fp)!=NULL )
        {

            execute(str);
            /* writing content to stdout */


        }
        fclose(fp);
}
void interactive()
{
    while(1)
    {
        printf("Shell>");
        char command[514];

        if(fgets (command, 514, stdin)==NULL)
        {
            exit(0);
        }

        execute(command);
    }

}

int findIndex(char *var)
{
    int i =0;
    for(i=0; i<varsLen; i++)
    {
        if(strcmp(var,vars[i])==0)
        {
            return i;
        }
    }
    return -1;

}

void cexecute(char *command)
{
    char c[600];
    char *arguments[512];
    int i=0;
    strcpy(c,command);
    char *strings [13];
    char path[514];
    parsePathArg(c,path,arguments);
// arg

    getPATH(strings);
//get env pathes
//try executing with pathes
    char temp[600];
    for(i=0; i<13; i++)
    {
        strcpy(temp,strings[i]);

        strcat(temp,"/");

        strcat(temp,path);

        if(execvp(temp,arguments)!=-1)
        {
            return ;
        }
    }
//exe
    perror("no such command");
}

int varPrint(char *co,char command[]){
    int i;
    char temp[128];
    int j=0;
    for(i=0; i<strlen(co); i++)
    {
        if (co[i]=='~')
        {
            if(findIndex("HOME")!=-1)
            {
                printf("error HOME value is changed\n");
                return 0;
            }

            strcat(command,getenv("HOME"));
            j+=strlen(getenv("HOME"));

        }
        else if(co[i]=='$')
        {
            int t=0;
            i++;
            for(; co[i]!='/'&&co[i]!='\t'&&co[i]!=' '&&co[i]!=0&&co[i]!='\"'&&co[i]!='\''&&co[i]!='$'; i++)
            {
                temp[t]=co[i];
                t++;
            }
            i--;
            temp[t]=0;
            int x=findIndex(temp);
            if(x!=-1)
            {
                strcat(command,datavars[x]);
                j+=strlen(datavars[x]);
            }
            else
            {
                if(getenv(temp)!=NULL)
                {
                    strcat(command,getenv(temp));
                    j+=strlen(getenv(temp));
                }
                else
                {
                    printf("error no variable  %s \n",temp);
                    return 0;
                }
            }
        }
        else
        {

            command[j]=co[i];
            j++;
        }
    }
    return 1;
    }

int varHandle(char *command){
    char varco[1024];
    char *sides[2];
    strcpy(varco,command);
    if(strchr(varco,'=')!=NULL)
    {

        if(varco[strlen(varco)-1] =='&')
        {
            varco[strlen(varco)-1] =0;
        }

        sides[0]=strtok(varco,"=");

        sides[1]=strtok(NULL,"=");
        trim(sides[0]);

        trim(sides[1]);
        int x=findIndex(sides[0]);

        if(!isAccepted(sides[0]))
        {
            printf("var is not in proper format\n");
            return 0;
        }
        else
        {

            if(x==-1)
            {
                if(varsLen>=2000)
                {
                    printf("lots of vars declared\n");
                    return 0;
                }
                x=varsLen;
                varsLen++;

                strcpy(vars[x],sides[0]);


            }
            if(sides[1][strlen(sides[1])-1]=='\"'||sides[1][strlen(sides[1])-1]=='\'')
            {
                sides[1][strlen(sides[1])-1]=0;
            }
            if(sides[1][0]=='\"'||sides[1][0]=='\'')
            {
                strcpy(datavars[x],sides[1]+1);
                return 0;
            }
            else
            {
                strcpy(datavars[x],sides[1]);
                return 0;
            }
        }

    }
    return 1;}

void execute(char *co)
{
    writeHistory(co);
    trim(co);
    if(strlen(co)>512)
    {
        printf("%s \n","error , Max size of line is 512");
        return ;
    }
    if(strlen(co)<=0 || co[0]=='#')
    {
        return;
    }
    int i;
    char command[1024]="";

// var print
 if(!varPrint(co,command))
    return;
//var print


//echo
if(!echo(command))
    return;

//echo
//exit

    if(strcmp(command,"exit")==0)
    {
        exit(0);
    }
//exit

//history

    if(strcmp(command,"history")==0)
    {
        readHistory();
        return;
    }

//history

//var handle
if(!varHandle(command))
    return;

//var handle

//cd
if(!cd(command))
    return;
//cd


    siginfo_t childstat;
    pid_t pid;
    pid=getpid();
    signal(SIGCHLD, handler);
    pid = fork();

    if(command[strlen(command)-1] =='&')
    {
        if(!pid)
        {
            command[strlen(command)-1]=0;
            trim(command);
            cexecute(command);
            return;
        }
    }
    else
    {


        if(!pid)
        {


            cexecute(command);
            kill(getpid(),SIGQUIT);
        }

        waitid(P_PID,pid,&childstat,WEXITED);
        return;
    }
}


