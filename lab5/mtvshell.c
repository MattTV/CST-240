#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

const short SIZE = 100;
const short ASIZE = 20;

int main()
{

    char in[SIZE];
    short quit = 0;

    while (!quit)
    {
        printf("\n$ ");
        fgets(in, SIZE, stdin);

        short numargs = 1;
        short ii = 0;
        while(in[ii] != '\n' && in[ii] != '\0')
        {
            if (in[ii] == ' ')
                ++numargs;

            ++ii;
        }

        if (in[ii - 1] == ' ')
            --numargs;

        char ** args = (char **)malloc((sizeof(char *) * numargs) + 1);

        ii = 0;
        char * token = NULL;
        token = strtok(in, " \n");
        while (token != 0)
        {
            args[ii] = token;
            token = strtok(NULL, " \n\t");
            ++ii;
        }

        args[ii + 1] = NULL;

        for (short kk = 0; kk < numargs; ++kk)
            printf("%s,",args[kk]);

        if (strcmp(args[0], "exit"))
        {
            pid_t a_pid = fork();

            if (a_pid == 0)
            {
                if (execvp(args[0], args))
                {
                    perror("Cannot execute");
                    exit(1);
                }
            }

            else if (a_pid > 0)
            {
                wait(&ii);
            }

            else
            {
                printf("Creating a new process to run the command failed.\n");
            }
        }
        else
        {
            quit = 1;
            printf("Thank you for using MTVShell! Make an amazing day for yourself!\n");
        }

    }

    return 0;

}
