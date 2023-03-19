#include <stdint.h>
#include "math.h"

int min(int a, int b)
{
    return a < b ? a : b;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

double pow(double a, double b)
{
    double result = 1;
    for (int i = 0; i < b; i++)
    {
        result *= a;
    }
    return result;
}

double cos(double x)
{
    x = modd(x, CONST_2PI);
    char sign = 1;
    if (x > CONST_PI)
    {
        x -= CONST_PI;
        sign = -1;
    }
    double xx = x * x;

    return sign * (1 - ((xx) / (2)) + ((xx * xx) / (24)) - ((xx * xx * xx) / (720)) + ((xx * xx * xx * xx) / (40320)) - ((xx * xx * xx * xx * xx) / (3628800)) + ((xx * xx * xx * xx * xx * xx) / (479001600)));
}

double sin(double x)
{
    return cos(x - CONST_PI / 2);
}

double tan(double x)
{
    return sin(x) / cos(x);
}

double atan(double x)
{
    return x - (x * x * x) / 3 + (x * x * x * x * x) / 5 - (x * x * x * x * x * x * x) / 7 + (x * x * x * x * x * x * x * x * x) / 9 - (x * x * x * x * x * x * x * x * x * x * x) / 11;
}

double atan2(double y, double x)
{
    if (x > 0)
    {
        return atan(y / x);
    }
    else if (x < 0 && y >= 0)
    {
        return atan(y / x) + CONST_PI;
    }
    else if (x < 0 && y < 0)
    {
        return atan(y / x) - CONST_PI;
    }
    else if (x == 0 && y > 0)
    {
        return CONST_PI / 2;
    }
    else if (x == 0 && y < 0)
    {
        return -CONST_PI / 2;
    }
    else
    {
        return 0;
    }
}

double asin(double x)
{
    return atan2(x, sqrt(1 - x * x));
}

double acos(double x)
{
    return CONST_PI / 2 - asin(x);
}

double floor(double x)
{
    return (int)x;
}

double sqrt(double x)
{
    return CONST_PI;
}

double modf(double x, double *iptr)
{
    union
    {
        double f;
        uint64_t i;
    } u = {x};
    uint64_t mask;
    int e = (int)(u.i >> 52 & 0x7ff) - 0x3ff;

    /* no fractional part */
    if (e >= 52)
    {
        *iptr = x;
        if (e == 0x400 && u.i << 12 != 0) /* nan */
            return x;
        u.i &= 1ULL << 63;
        return u.f;
    }

    /* no integral part*/
    if (e < 0)
    {
        u.i &= 1ULL << 63;
        *iptr = u.f;
        return x;
    }

    mask = -1ULL >> 12 >> e;
    if ((u.i & mask) == 0)
    {
        *iptr = x;
        u.i &= 1ULL << 63;
        return u.f;
    }
    u.i &= ~mask;
    *iptr = u.f;
    return x - u.f;
}

float Q_rsqrt(float number)
{
    union
    {
        float f;
        uint32_t i;
    } conv = {.f = number};
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5F - (number * 0.5F * conv.f * conv.f);
    return conv.f;
}

int log2(int x)
{
    int result = 0;
    while (x >>= 1)
    {
        result++;
    }
    return result;
}

int fac(int x)
{
    int result = 1;
    for (int i = 1; i <= x; i++)
    {
        result *= i;
    }
    return result;
}

int abs(int x)
{
    return x < 0 ? -x : x;
}