#include "common.h"

void fatal_error(const char * msg)
{
    perror(msg);
    exit(1);
}

void WriteString(int fd, char * buffer)
{
    // Get the length of the given message.
    short len = strlen(buffer);
    // Get the network version of the length of the given message.
    short net_len = htons(len);
    // Write the length to the recipient.
    if (write(fd, &net_len, sizeof(short)) < 0)
        fatal_error("Failed to write buffer length.");
    // Write the message to the recipient.
    if (write(fd, buffer, sizeof(char) * len) < 0)
        fatal_error("Failed to write buffer data.");
}

void ReadString(int fd, char * buffer)
{
    // Read in the length of the incoming message.
    short net_len = 0;
    if (read(fd, &net_len, sizeof(short)) < 0)
        fatal_error("Failed to read buffer length.");
    // Convert the length to the host's format.
    short len = ntohs(net_len);
    // Read in the incoming message.
    if (read(fd, buffer, len) < 0)
        fatal_error("Failed to read buffer data.");
}
