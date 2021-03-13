#include <string.h>

void fatal_error(const char * msg)
{
    perror(msg);
    exit(1);
}

void WriteString(int fd, char * buffer)
{
    short len = strlen(buffer);
    short net_len = htons(len);
    if (write(fd, &net_len, sizeof(short)) < 0)
        fatal_error("Failed to write buffer length.");
    if (write(fd, buffer, sizeof(char) * len) < 0)
        fatal-error("Failed to write buffer data.");
}

void ReadString(int fd, char * buffer)
{
    short net_len = 0;
    if (read(fd, &net_len, sizeof(short)) < 0)
        fatal_error("Failed to read buffer length.");
    short len = ntohs(net_len);
    if (read(fd, buffer, len) < 0)
        fatal_error("Failed to read buffer data.");
}
