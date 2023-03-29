#include "math.h"

f64 abs(f64 x)
{
    if (x < 0)
        return -x;
    return x;
}

i64 pow(i64 x, i64 y)
{
    i64 result = 1;
    for (i64 i = 0; i < y; i++)
        result *= x;
    return result;
}
