////////////////////////////////////
// Names: Jay Reich, Josiah Norman
////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: ./q2 <num_proc> <executable>\ne.g., ./q2 2 ./q1\n");
        return 0;
    }
    int num = atoi(argv[1]);
    //char exe[256] = strdup();
    
    for (int i = 0; i < num; i++)
    {
        time_t t;
        srand((unsigned) time(&t)*i*i);
        // printf("%li",t);
        // printf("\n");
        if(fork() == 0)
        {
            printf("In child %i: ", getpid());
            // // char* args[4] = {argv[1], argv[2], argv[3], NULL};
            
            int rand1 = rand()%99+1;
            char rand1s[256];
            sprintf(rand1s, "%d", rand1);

            int rand2 = rand()%99+1;
            char rand2s[256];
            sprintf(rand2s, "%d", rand2);

            char* args[4] = {argv[2], rand1s, rand2s, NULL};
            fflush(stdout);
            execvp(strdup(argv[2]), args);
        }
        wait(NULL);
        // printf("\n");
    }
    exit(0);
}