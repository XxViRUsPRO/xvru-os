#include "magic.h"

void sleep(u32 ms)
{
    counter = ms;
    while (counter-- > 0)
        ;
}
