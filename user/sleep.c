#include "user/user.h"

int main(int argc, char *argv[])
{
    // fprintf(1, "sleep started. argc: %d\n", argc);

    if (argc < 2)
    {
        fprintf(2, "no args. expected 1.\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    // fprintf(1, "sleep while %d ticks\n", n);

    pause(n);

    exit(0);
}