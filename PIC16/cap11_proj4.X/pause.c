#include <pic.h>

void pause(unsigned short msvalue)
{
    unsigned short x;
    for (x=0; x<=msvalue; x++)
    {
        msecbase();
    }
}
