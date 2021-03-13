int main(int argc, char * argv[])
{
    if (argc < 2)
        fatal_error("No port number supplied.\n");

    int opt;
    int ii;
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
    int clifd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    
    // Begin communicating.
    char buffer[256];
    bzero(buffer, 256);

    ReadString(sockfd, buffer);
    printf("%s\n", buffer);

    WriteString(clientfd, "Sup cuntface");

    // Close the socket.
    close(clientfd);
    close(sockfd);

    
   

    return 0;
}
