#include "../peripheral.h"

void exit(int status)
{
    *(unsigned char*)EXIT_POINT_RC = status;
    while (1)
    {
    }
}
