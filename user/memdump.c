#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data);

int main(int argc, char *argv[])
{
  if (argc == 1)
  {
    printf("Example 1:\n");
    int a[2] = {61810, 2025};
    memdump("ii", (char *)a);

    printf("Example 2:\n");
    memdump("S", "a string");

    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *)&s);

    struct sss
    {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;

    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");

    printf("Example 4:\n");
    memdump("pihcS", (char *)&example);

    printf("Example 5:\n");
    memdump("sccccc", (char *)&example);
  }
  else if (argc == 2)
  {
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while (n < sizeof(data))
    {
      int nn = read(0, data + n, sizeof(data) - n);
      if (nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data);
  }
  else
  {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void memdump(char *fmt, char *data)
{
  // Your code here.
  // fmt: C string.
  char c = *fmt;
  while (c != '\0')
  {

    if (c == 'i')
    {
      int integer_32_dec = *(int *)data;
      data += 4;
      printf("%d\n", integer_32_dec);
      // char buf_integer_32_dec[4];

      // for (int i = 0; i < 4; i++)
      // {
      //   buf_integer_32_dec[i] = *data++;
      // }

      // unsigned int bytes_moved_safely = ((unsigned int)(unsigned char)buf_integer_32_dec[0]) |
      //                                   ((unsigned int)(unsigned char)buf_integer_32_dec[1] << 8) |
      //                                   ((unsigned int)(unsigned char)buf_integer_32_dec[2] << 16) |
      //                                   ((unsigned int)(unsigned char)buf_integer_32_dec[3] << 24);

      // int integer_32_dec;

      // if (bytes_moved_safely <= 0x7FFFFFFFu)
      // {
      //   integer_32_dec = (int)bytes_moved_safely;
      // }
      // else
      // {
      //   integer_32_dec = -1 - (int)(0xFFFFFFFFu - bytes_moved_safely);
      // }

      // int max_number_of_digits = 10; // -2,147,483,648 ~ 2,147,483,647;
      // int digits[max_number_of_digits];

      // int is_negative = integer_32_dec < 0;
      // unsigned int value;
      // if (is_negative)
      // {
      //   is_negative = 1;
      //   value = (unsigned int)((integer_32_dec + 1) * -1) + 1;
      // }
      // else
      // {
      //   value = (unsigned int)integer_32_dec;
      // }

      // int digit;
      // int ones_place = sizeof(digits) / sizeof(digits[0]) - 1;
      // int n = ones_place;

      // if (value == 0)
      // {
      //   digits[n] = 0;
      //   n--;
      // }

      // while (value != 0)
      // {
      //   digit = value % 10;
      //   digits[n] = digit;
      //   value /= 10;
      //   n--;
      // }

      // if (is_negative)
      // {
      //   printf("-");
      // }

      // for (int i = n + 1; i <= ones_place; i++)
      // {
      //   printf("%d", digits[i]);
      // }

      // printf("\n");
    }
    else if (c == 'p')
    {
      long integer_64_hex = *(long *)data;
      data += 8;
      printf("%lx\n", integer_64_hex);

      // memcpy(&integer_64_hex, data, 8);
      // if (integer_64_hex < 0)
      // {
      //   fprintf(2, "Cannot preserve value semantics of negative value on %%lx.\n");
      //   exit(1);
      // }
    }
    else if (c == 'h')
    {
      short integer_16_dec = *(short *)data;
      ;
      data += 2;
      printf("%d\n", integer_16_dec);

      // memcpy(&integer_16_dec, data, 2);
    }
    else if (c == 'c')
    {
      char c_ = *data++;
      printf("%c\n", c_);

      // if (c_ < 0)
      // {
      //   fprintf(2, "Cannot preserve value semantics of negative value on %%c.\n");
      //   exit(1);
      // }
    }
    else if (c == 's')
    {
      char *p_ = *(char **)data;
      data += 8;
      printf("%s\n", p_);

      // memcpy(&s_, data, 8);
    }
    else if (c == 'S')
    {
      // fmt and data are finished on 'S' formatter?
      printf("%s\n", data);
    }
    else
    {
      fprintf(2, "Invalid flag in fmt.\n");
      exit(1);
    }

    c = *(++fmt);
  }
}
