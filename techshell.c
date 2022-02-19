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

        // execl("/bin/toch", "toch", "temp", NULL);
        //execlp
        // execlp(command, args);
        // char* ars[4] = {"ls", NULL};
        // char* ars = "ls";
        // execlp("ls", ars, "-l", NULL);
        execvp(command, args);
        printf("Error: %d, %s\n", errno, strerror(errno));
        // printf("%s command failed\n", args[0]);
        exit(1);
    }
    wait(NULL);
}

int main(int argc, char *argv)
{
    // if(argc != 2)
    // {
    //     printf("Usage: ./prog2 <dir>\n");
    //     return 0;
    // }
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

        // while(token != NULL)
        // {
            if(count == 0)
            {
                token_ONE = token;
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
                    //move cwd to 2nd arg
                    // int change = chdir(argv[1]);
                    // if (change == -1)
                    // {
                    //     printf("Can't chdir to %s\n", argv[1]);
                    //     exit(1);
                    // }
                    getcwd(cwd, MAX_SIZE);
                    break;
                }
                else if(!strcmp(token, "pwd\n"))
                {
                    printf("%s\n", cwd);
                    break;
                }
                else if(!strcmp(token, "exit\n"))
                {
                    exit(0);
                }
            }

            // run command
            // char* args[3] = {"ls", "-l", NULL};
            char* args[16];
            args[0] = token_ONE;
            // printf("First arg = %s\n", token_ONE);
            
            count = 1;
            while (token != NULL)
            {
                token = strtok(NULL, s);
                args[count] = token;
                // printf("arg[%d] = %s\n", count, args[count]);
                count++;
            }
            if(count == 2)
            {
                token_ONE[strlen(token_ONE) - 1] = 0;
                args[0] = token_ONE;
            }
            else
            {
                // printf("%s", args[count-1]);
                args[count-2][strlen(args[count-2]) - 1] = 0;
            }
            // if(count == 1)
            // {
            //     // printf("%s", token_ONE);
            //     token_ONE[strlen(token_ONE) - 1] = 0;
            //     args[0] = token_ONE;
            // }
            // else
            // {
            //     printf("%s", args[count-1]);
            //     args[count-1][strlen(args[count-1]) - 1] = 0;
            // }

            // args[count] = NULL;
            
            

            exe(args[0], args);
            

            // printf(" %s", token);
            // token = strtok(NULL, s);
            // if(token != NULL)
            //     printf("\n");
            // count++;


        }
        // if(DEBUG && 0)
        // {
        //     printf("%i token(s) read\n\n", count);
        // }
    // }    
    return 0;

    /*

    // where the fork happens, everything in the if is what the child is doing
    if ((c1 = fork()) == 0)
    {
        printf("Current working directory: %s\n", cwd);
        printf("Executing ls %s --all -l --human-readable\n", argv[1]);
        
        int change = chdir(argv[1]);
        if (change == -1)
        {
            printf("Can't chdir to %s\n", argv[1]);
            exit(1);
        }
        //// All the comments here were me just trying to figure out how to use ls in c
        // char* args[6] = {"ls", newcwd, "--all", "-l", "--human-readable", NULL};
        fflush(stdout);
        // execl(newcwd, "ls", "--all", "-l", "--human-readable", NULL);
        execl("/bin/ls", "ls", "--all", "-l", "--human-readable", argv[1], NULL);
        // execv(newcwd, args);
        // execl("bin/ls", "ls", "-la", "-l", "-h", NULL);
        // printf("Failed\n");
        exit(1); // exit with 1 in case the line failed, which would be a error in the code
    }

    int status;
    // wait for child to finish and get the exit status
    waitpid(c1, &status, 0);
    if (WIFEXITED(status))
    {
        const int es = WEXITSTATUS(status);
        printf("Exit status: %d\n", es);
    }
    exit(0);
    */
}