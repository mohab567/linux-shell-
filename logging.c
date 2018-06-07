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

void readHistory(){
char path[200];
        strcpy(path,getenv("HOME"));
        strcat(path,"/history.txt");
        int c;
        FILE *file;
        file = fopen(path, "r");
        if (file)
        {
            while ((c = getc(file)) != EOF)
                printf("%c",c);
            fclose(file);

        }
}
void writeHistory(char *co)
{
    char path[200];
    strcpy(path,getenv("HOME"));
    strcat(path,"/history.txt");
    FILE *f = fopen(path, "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }


    fprintf(f, "%s", co);
    fclose(f);


}
void handler(int sig)
{
    pid_t pid;

    pid = wait(NULL);
    char path[200];
    strcpy(path,getenv("HOME"));
    strcat(path,"/log.txt");
    FILE *f = fopen(path, "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    const char *text = "Child process was terminated";
    fprintf(f, "%s\n", text);
    fclose(f);
}
