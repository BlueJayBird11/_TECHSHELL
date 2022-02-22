//////////////////////////////////////////
// Name: Jay Reich, Josiah Norman
// Date: Feburary 21, 2022
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

int changeCWD(char* input);

void exe(char* command, char* args[]);

int main(int argc, char *argv)
{
    // SECTION 1 START
    char cwd[MAX_SIZE];
    getcwd(cwd, MAX_SIZE);

    
    // This is the token that is used to split the string
    const char s[2] = " ";
    char *token;

    char* token_ONE;
    // SECTION 2 START
    while(1)
    {
        //Print out base line with the cwd
        char str[MAX_SIZE];
        printf("%s$ ", cwd);
        //Get the user's input
        fgets(str, MAX_SIZE, stdin);

        token = strtok(str, s);
        int count = 0;
        token_ONE = token;

        // SECTION 3 START
        if(!strcmp(token, "cd") || !strcmp(token, "pwd\n") || !strcmp(token, "exit\n"))
        {
            
            //if the first token is cd, pwd, or exit, execute the command
            if(!strcmp(token, "cd"))
            {        
                token = strtok(NULL, s);
                token[strlen(token) - 1] = 0; //remove the "/n" from the end of the string
                
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

        // SECTION 4 A START
        else 
        {
            char* args[16];
            args[0] = token_ONE;
            
            count = 1;
            int placeholder = 1;
            char* my_output_file;
            char* my_input_file;
            while (token != NULL)
            {
                // checks for piping operands
                if(!strcmp(token, ">"))
                {
                    my_output_file = strtok(NULL, s);                    
                    my_output_file[strlen(my_output_file) - 1] = 0;

                    FILE* outfile = fopen(my_output_file, "w");
                    dup2(fileno(outfile), 1);
                    fclose(outfile);

                    // printf("--REACHED\n");
                    args[count] = NULL;
                    placeholder = 0;
                    break;
                }

                if(!strcmp(token, "<"))
                {
                    my_input_file = strtok(NULL, s);
                    my_input_file[strlen(my_input_file) - 1] = 0;
                    
                    // printf("%s\n", my_input_file);

                    FILE* infile = fopen(my_input_file, "r");
                    dup2(fileno(infile), 0);
                    fclose(infile);

                    // printf("---REACHED");

                    args[count] = NULL;
                    placeholder = 0;
                    break;
                }

                // tokenizes the string and adds it to the list
                token = strtok(NULL, s);
                args[count] = token;
                count++;
            }
            
            // SECTION 4 B START
            if(count == 2)
            {
                token_ONE[strlen(token_ONE) - 1] = 0;
                args[0] = token_ONE;
            }
            else if(placeholder)
            {
                args[count-2][strlen(args[count-2]) - 1] = 0;
            }
            else
            {
                
                args[count-1] = NULL;
                // continue;
            }
            // printARGS(args);
            
            // Executes command
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

int changeCWD(char* input)
{
    int change = chdir(input);
    if (change == -1)
    {
        printf("Error: %d, %s\n", errno, strerror(errno));
    }
    return change;
}

void exe(char* command, char* args[]) 
{
    if(fork() == 0)
    {
        fflush(stdout);
        execvp(command, args);

        // Shows error if execvp fails
        printf("Error: %d, %s\n", errno, strerror(errno));
        exit(1);
    }else{
    wait(NULL);
    }
}
