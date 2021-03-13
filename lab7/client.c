#include "common.h"

int main(int argc, char * argv[])
{
    if (argc < 2)
        fatal_error("No port number supplied.");

    int opt;
    int port = 0;
    char host[20];

    while ((opt = getopt(argc, argv, "p:h:")) != -1)
    {
        switch (opt)
        {
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                strcpy(host, optarg);
                break;
            case ':':
                fatal_error("An option is missing a value.");
                break;
            case '?':
                fatal_error("An unkown option was given.");
                break;
        }
    }

    if (port < 50000)
        fatal_error("Port must be 50,000 or larger.");

    // Create the socket.
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        fatal_error("Error opening socket.");

    // Set the socket options
    struct hostent * server = gethostbyname(host);
    if (server == NULL)
        fatal_error("No such host found.");
    
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    // Connect to the host.
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        fatal_error("Error connecting to host.");

    // Start communicating.
    WriteString(sockfd, "Sup dumbass bitch.");

    // Wait for the server to respond.
    char buffer[255];
    ReadString(sockfd, buffer);

    printf("%s\n", buffer);

    // Close the socket.
    close(sockfd);
    close(sockfd);


    return 0;
}
