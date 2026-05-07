#include "user/user.h"
#include "kernel/fcntl.h"

#define SEQUENCE_SIZING_FACTOR 16
#define BUF_SIZE 64

void sixfive(char *filename);
void commit_sequence(const char *sequence);
char *append_sequence(char c, char *sequence, int *sequence_length, int *sequence_capacity);

int main(int argc, char *argv[])
{
    for (int i = 1; i <= argc; i++)
    {
        sixfive(argv[i]);
    }

    exit(0);
}

void sixfive(char *filename)
{
    char *sequence; // A sequence to build for a decimal.
    int sequence_capacity = SEQUENCE_SIZING_FACTOR;
    int sequence_length = 0;   // The length of sequence. sequence + (sequence_length - 1) == end byte of sequence.
    int sequence_is_valid = 1; // Validity of sequence. Start of the file is an imlicit separator. So, it is initialized as true.

    sequence = (char *)malloc(sequence_capacity);
    sequence[sequence_length] = '\0';

    if (sequence == 0)
    {
        fprintf(2, "malloc failed.\n");
        exit(1);
    }

    int fd = open(filename, O_RDONLY);

    if (fd < 0)
    {
        fprintf(2, "open failed.\n");
        exit(1);
    }

    char buf[BUF_SIZE];
    int n;

    while (1)
    {
        n = read(fd, buf, sizeof(buf));

        if (n == 0)
        {
            // EOF.
            // Implicit separator.
            close(fd);

            if (sequence_length > 0)
            {
                // Commit sequence.
                commit_sequence(sequence);

                // Reset sequence.
                sequence_length = 0;
            }

            break;
        }
        if (n > 0)
        {
            const char *explicit_separators = " -\r\t\n./,";
            char c;
            for (int i = 0; i < n; i++)
            {
                c = buf[i];

                if (c >= '0' && c <= '9')
                {
                    // c is a number.

                    if (sequence_is_valid == 1)
                    {
                        // Append c to sequence.

                        if ((sequence = append_sequence(c, sequence, &sequence_length, &sequence_capacity)) == 0)
                        {
                            fprintf(2, "append failed.");
                            exit(1);
                        }
                    }
                }
                else if (strchr(explicit_separators, c) > 0)
                {
                    // c is one of explicit separators.

                    if (sequence_is_valid == 1)
                    {
                        if (sequence_length > 0)
                        {
                            // Commit sequence.
                            commit_sequence(sequence);

                            // Reset sequence.
                            sequence_length = 0;
                        }
                    }
                    else
                    {
                        sequence_is_valid = 1;
                    }
                }
                else
                {
                    // Makes sequence invalid.
                    sequence_is_valid = 0;

                    // Reset sequence.
                    sequence_length = 0;
                }
            }
        }
        else
        {
            close(fd);
            fprintf(2, "read failed.\n");
            exit(1);
        }
    }
}

void commit_sequence(const char *sequence)
{

    int decimal = atoi(sequence); // A decimal to print.

    if (decimal % 5 == 0 || decimal % 6 == 0)
    {
        // Check whether decimal is a multiple of 5 or 6.
        fprintf(1, "%d\n", decimal);
    }
}

char *append_sequence(char c, char *sequence, int *sequence_length, int *sequence_capacity)
{
    fprintf(1, "checkpoint #1: char: %c\n", c);

    if (*sequence_length + 1 > *sequence_capacity)
    {

        // Reallocation.
        char *tmp = sequence;
        *sequence_capacity += SEQUENCE_SIZING_FACTOR;
        sequence = (char *)malloc(*sequence_capacity);

        if (sequence == 0)
        {
            fprintf(2, "malloc failed.\n");
            return 0;
        }

        for (int i = 0; i <= *sequence_length; i++)
        {
            sequence[i] = tmp[i];
        }

        free(tmp);
    }

    sequence[(*sequence_length)++] = c;
    sequence[*sequence_length] = '\0';
    return sequence;
}