#include <stdio.h>

int main(int argc, char **argv)
{
    int ii;

    for (ii=0; ii<argc; ii++)
    {
        printf("%d %s\n", ii, argv[ii]);
    }
    return 0;
}
