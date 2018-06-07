#ifndef parsing_   /* Include guard */
#define parsing_

void trim(char * s);
int isAccepted(char* s);
void parsePathArg(char *c,char *path,char *arguments[]);
void getPATH(char *strings[]);
int echo(char *command);
int cd(char *command);


#endif
