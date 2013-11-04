#include <xc.h>

void msecbase(void);

void pause (unsigned short usvalue)
{
    unsigned short x;
    for (x=0; x<=usvalue; x++)
    {
        msecbase();
    }
}