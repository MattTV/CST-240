#include "common.h"
#include <pthread.h>

void * user(void * param);

int main(int argc, char * argv[])
{
    if (argc < 2)
        fatal_error("No port number supplied.\n");

    int opt;
    int port = 0;

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
    //int clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    // Begin communicating.


    // Testing
    int clientfd = -1;
    pthread_t tid;
    while (1)
    {
        while (clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen))
        {
            if (pthread_create(&tid, NULL, user, (void *)&clientfd) < 0)
                fatal_error("Failed to create thread.");

    //int clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    //pthread_create(&tid, NULL, user, (void *)&clientfd);
        }
        
            pthread_join(tid, (void *)&clientfd);
    }


    /*
    pthread_t tids[3];
    int i = 0;
    int clientfd = -1;
    struct sockaddr_in cli_addr;

    while (1)
    {
    socklen_t clilen = sizeof(cli_addr);
    clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        
            if (pthread_create(&tids[i++], NULL, user, (void *)&clientfd) < 0)
                fatal_error("Failed to create thread.");

    //int clientfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    //pthread_create(&tid, NULL, user, (void *)&clientfd);
        }
        
            if (i >= 3)
            {
                i = 0;
                while (i < 3)
                {
            pthread_join(tids[i++], (void *)&clientfd);
                }
                i = 0;
            }
    //}*/

    // Close the socket.
    close(sockfd);

    return 0;
}
void * user(void * param)
{
    int clientfd = *((int *)param);

    char op = '\0';

    while (op != '.')
    {
        op = '\0';
        char inbuffer[256];
        bzero(inbuffer, 256);
        ReadString(clientfd, inbuffer);

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
        if (op != '.')
        {
            char * secbuffer = inbuffer + ii;

            int num1 = atoi(inbuffer);
            int num2 = atoi(secbuffer);
            int result = 0;
            char outbuffer[256];
            short error = 0;

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
            if (error)
                strcpy(outbuffer, "unable to be processed becuase an invalid operation was provided.");
            else
                sprintf(outbuffer, "%i", result);

            WriteString(clientfd, outbuffer);
        }
    }

    close(clientfd);

    return param;
}
