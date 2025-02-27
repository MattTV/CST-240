#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    const short SIZE = 100;

    char in[SIZE];
    char cwd[SIZE];
    short quit = 0;

    

    while (!quit)
    {
        // Print a prompt, clear the input buffer, and take input.
        getcwd(cwd, SIZE);
        printf("\n%s$ ", cwd);
        memset(&in, '\0', 100);
        fgets(in, SIZE, stdin);

        // Start with one argument given for sure, and count the number of arguments based on the number of spaces.
        short numargs = 1;
        int ii = 0;
        while(in[ii] != '\n' && in[ii] != '\0')
        {
            if (in[ii] == ' ')
                ++numargs;

            ++ii;
        }

        // If there is a trailing space that was counted as an extra argument, remove it.
        if (in[ii - 1] == ' ')
            --numargs;

        // Create an array of cStrings to hold each argument, plus the ending NULL.
        char ** args = (char **)malloc((sizeof(char *) * (numargs + 1)));

        // Start the iterative variable in the same spot and tokenize the arguments into individual cStrings.
        ii = 0;
        char * token = NULL;
        token = strtok(in, " \n\t");
        while (token != NULL)
        {
            args[ii] = token;
            token = strtok(NULL, " \n\t");
            ++ii;
        }

        // Set the last argument as NULL.
        args[ii] = NULL;

        // Look for one of the three internal commands.
        if (strcmp(args[0], "exit") == 0)
        {
            // Set a flag to avoid looping again and print a nice message.
            quit = 1;
            printf("Thank you for using MTVShell! Make an amazing day for yourself!\n");
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
            // Get and print the current working directory.
            getcwd(cwd, SIZE);
            printf("%s\n", cwd);
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            // Change the current working directory.
            chdir(args[1]);
        }
        // Or, if none of those commands were given, execute the given command as a child process.
        else
        {
            // Create a new process.
            pid_t a_pid = fork();

            // In the child process...
            if (a_pid == 0)
            {
                // ...Execute the given command and arguments, and show an error if there was one.
                if (execvp(args[0], args))
                {
                    perror("Cannot execute");
                    exit(1);
                }

                exit(0);
            }
          
            // The parent process...
            else if (a_pid > 0)
            {
                // Wait for the child to finish executing so the loop can run again.
                wait(&ii);
            }
           
            // If there was an error creating a new process, say so.
            else
            {
                printf("Creating a new process to run the command failed.\n");
                
            }
        }
        // Free the arguments array.
        free(args);
    }

    return 0;
}
