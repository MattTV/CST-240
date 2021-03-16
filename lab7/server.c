#include "common.h"
#include <pthread.h>

void * user(void * param);

int main(int argc, char * argv[])
{
    // Check if a port number was supplied.
    if (argc < 2)
        fatal_error("No port number supplied.\n");

    int opt;
    int port = 0;

    // Process the port number argument.
    while ((opt = getopt(argc, argv, "p:")) != -1)
    {
        switch (opt)
        {
            case 'p':
                port = atoi(optarg);
                break;
            case ':':
                fatal_error("An option is missing a value.\n");
                break;
            case '?':
                fatal_error("An unkown option was given.\n");
                break;
        }
    }

    // Ensure the port is in the right range.
    if (port < 50000)
        fatal_error("Port must be 50,000 or larger.\n");

    // Create the socket.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_error("Error opening socket.");

    // Set socket options.
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the port.
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        fatal_error("Error binding socket to port.");


    // Listen for incoming connections, waiting for up to five clients.
    listen(sockfd, 5);

    // Accept any and all clients, because this is CSET, not Cybersecurity.
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);

    // Begin communicating.
    int clientfd = -1;
    pthread_t tid[3];
    //while (1)
    //{
        short jj = 0;

        // Accept a connection on a client file desciptor.
        while (jj < 3)
        {
            clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
            // Create a thread.
            if (pthread_create(&tid[jj], NULL, user, (void *)&clientfd) < 0)
                fatal_error("Failed to create thread.");

            ++jj;
        }

        // Join a thread. Only one because I don't understand how to do more and it doesn't work and it makes my emotions hurt :(
        for (short kk = 0; kk < 3; ++kk)
        {
            pthread_join(tid[kk], (void *)&clientfd);
        }
    //}


    // Close the socket's file descriptor.
    close(sockfd);

    return 0;
}
void * user(void * param)
{
    // Bring in the file descriptor, to a usable format.
    int clientfd = *((int *)param);

    // A character for the operation.
    char op = '\0';

    // Until a '.' character has been detected.
    while (op != '.')
    {
        // Make a buffer and read in the input.
        op = '\0';
        char inbuffer[256];
        bzero(inbuffer, 256);
        ReadString(clientfd, inbuffer);

        // Search for an operation, even a '.'.
        short ii = 0;
        short found = 0;
        while (found == 0 && inbuffer[ii] != '\0')
        {
            if (inbuffer[ii] == '+' || inbuffer[ii] == '-' || inbuffer[ii] == '*' || inbuffer[ii] == '/' || inbuffer[ii] == '%' || inbuffer[ii] == '.')
            {
                op = inbuffer[ii];
                found = 1;
                inbuffer[ii] = '\0';
            }
            ++ii;
        }

        // If not a period, continue working.
        if (op != '.')
        {
            // The second half of the equation.
            char * secbuffer = inbuffer + ii;

            // Convert the strings to integers.
            int num1 = atoi(inbuffer);
            int num2 = atoi(secbuffer);
            int result = 0;
            char outbuffer[256];
            short error = 0;

            // Perform the right operation.
            switch (op)
            {
                case '+':
                    result = num1 + num2;
                    break;
                case '-':
                    result = num1 - num2;
                    break;
                case '*':
                    result = num1 * num2;
                    break;
                case '/':
                    result = num1 / num2;
                    break;
                case '%':
                    result = num1 % num2;
                    break;
                default:
                    error = 1;
            }

            // If a bad operating character was given, send back an error.
            if (error)
                strcpy(outbuffer, "unable to be processed becuase an invalid operation was provided.");
            // Otherwise, convert the result into a string.
            else
                sprintf(outbuffer, "%i", result);

            // Write the result or error to the client.
            WriteString(clientfd, outbuffer);
        }
    }

    // Close the client file descriptor.
    close(clientfd);

    return param;
}
