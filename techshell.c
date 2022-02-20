//////////////////////////////////////////
// Name: Jay Reich, Josiah Norman
// Date: Feburary __, 2022
// Descriptions: Programming Assignment #3
//////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>

#ifndef MAX_SIZE
#define MAX_SIZE 256
#endif
#ifndef DEBUG
#define DEBUG 1
#endif

void printARGS(char* args[]);

int changeCWD(char* input)
{
    int change = chdir(input);
    if (change == -1)
    {
        // printf("Can't chdir to %s\n", input);
        // printf("Error 2 (No such file or directory)\n");
        printf("Error: %d, %s\n", errno, strerror(errno));
        // exit(1);
    }
    return change;
}

void exe(char* command, char* args[]) {
    if(fork() == 0)
    {
        // char* args[4] = {args[2], rand1s, rand2s, NULL};
        fflush(stdout);
        // execvp(strdup(command), args);

        
        execvp(command, args);
        printf("Error: %d, %s\n", errno, strerror(errno));
        // printf("%s command failed\n", args[0]);
        exit(1);
    }
    wait(NULL);
}

int main(int argc, char *argv)
{

    pid_t c1;
    char cwd[MAX_SIZE];
    getcwd(cwd, MAX_SIZE);

    char str[MAX_SIZE];
    // This is the token that is used to split the string
    const char s[2] = " ";
    char *token;
    // Set us a string that is the equivalent of the user typing exit to leave 
    // char exit[256];
    // strcpy(exit, "exit\n");
    char* token_ONE;
    while(1)
    {
        printf("%s$ ", cwd);
        //Get the user's input
        fgets(str, MAX_SIZE, stdin);

        /* //These are some tests of debugging
        scanf("%s", &str[0]);
        printf(str);
        printf("%s : %s = %i\n",str, exit, strcmp(str, exit));*/
        
        // printf("Line read: %s", str);
        token = strtok(str, s);
        int count = 0;
        // printf("Token(s):\n");
        // Go through every token using " " as the key to separate each

        token_ONE = token;
        // printf("exit\n");
        if(!strcmp(token, "cd") || !strcmp(token, "pwd\n") || !strcmp(token, "exit\n"))
        {
            
            //if the first token is cd, pwd, or exit, exicute the command and break loop
            if(DEBUG && 1)
            {
                // printf("str==pwd: %d\n", strcmp(token, "pwd\n"));
                // printf("str==cd: %d\n", strcmp(token, "cd"));
                // printf("str==pwd: %d\n", strcmp(token, "pwd\n"));
            }
            if(!strcmp(token, "cd"))
            {        
                token = strtok(NULL, s);
                token[strlen(token) - 1] = 0; //remove the "/n" from the end of the string
                if(DEBUG && 0)
                {
                    printf("cd to %s!\n", token);
                    }
                    changeCWD(token);
                    
                    getcwd(cwd, MAX_SIZE);
                    
                }
                else if(!strcmp(token, "pwd\n"))
                {
                    printf("%s\n", cwd);
                    
                }
                else if(!strcmp(token, "exit\n"))
                {
                    exit(0);
                }
            }
            else 
            {
                // printf("str==pwd: %d\n", strcmp(token, "pwd\n"))
                // run command
                // char* args[3] = {"ls", "-l", NULL};
                char* args[16];
                args[0] = token_ONE;
                // printf("First arg = %s\n", token_ONE);
                
                count = 1;
                int placeholder = 1;
                while (token != NULL)
                {
                    

                    
                    // malloc(128);
                    if(!strcmp(token, ">"))
                    {
                        char* my_output_file = strtok(NULL, s);
                        my_output_file[strlen(my_output_file) - 1] = 0;
                        FILE* outfile = fopen(my_output_file, "w");
                        dup2(fileno(outfile), 1);
                        fclose(outfile);

                        // count++;
                        args[count] = NULL;
                        placeholder = 0;
                        break;
                    }
                    // printf("-------REACHED\n");
                    if(!strcmp(token, "<"))
                    {
                        char* my_input_file = strtok(NULL, s);
                        printf("%s", my_input_file);
                        my_input_file[strlen(my_input_file) - 1] = 0;
                        FILE* infile = fopen(my_input_file, "r");
                        dup2(fileno(infile), 0);
                        fclose(infile);

                        // count++;
                        args[count] = NULL;
                        placeholder = 0;
                        break;
                    }
                    // printf("-------REACHED\n");
                    // printf("arg[%d] = %s\n", count, args[count]);
                    
                    token = strtok(NULL, s);
                    args[count] = token;
                    count++;
                }
                if(count == 2)
                {
                    token_ONE[strlen(token_ONE) - 1] = 0;
                    args[0] = token_ONE;
                }
                else if(placeholder)
                {
                    // printf("%s", args[count-1]);
                    args[count-2][strlen(args[count-2]) - 1] = 0;
                }
                else
                {
                    
                    args[count-1] = NULL;
                    // printARGS(args);
                    continue;
                }
                // printARGS(args);
                
                exe(args[0], args);
            }

            


        }

    return 0;
}
void printARGS(char* args[])

{
    char* token;
    int count = 0;
    token = args[count];
    while (token != NULL)
    {
        printf("%s ", token);
        token = args[count];
        count++;
    }
    printf("\n");
}
