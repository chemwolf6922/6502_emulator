#include "../peripheral.h"
#include <stdio.h>

void exit(int status)
{
    *(volatile unsigned char*)EXIT_POINT_RC = status;
    while (1)
    {
    }
}

void __putchar(char c)
{
    *(volatile unsigned char*)TERMINAL_WRITE = c;
}

int __getchar()
{
    return *(volatile unsigned char*)TERMINAL_READ;
}
