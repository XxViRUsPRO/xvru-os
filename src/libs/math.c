#include "math.h"

f64 abs(f64 x)
{
    if (x < 0)
        return -x;
    return x;
}

i64 ceil(f64 x)
{
    i64 result = (i64)x;
    if (x > result)
        result++;
    return result;
}

i64 floor(f64 x)
{
    i64 result = (i64)x;
    if (x < result)
        result--;
    return result;
}

i64 pow(i64 x, i64 y)
{
    i64 result = 1;
    for (i64 i = 0; i < y; i++)
        result *= x;
    return result;
}

f64 log(f64 x)
{
    f64 result = 0.0;
    f64 inter = 1.0;
    f64 num = x - 1.0;
    for (int i = 1; i <= 6; i++)
    {
        f64 comp = 2.0 * i;
        f64 den = comp * (comp - 1.0);
        inter *= num / den;
        result += inter;
    }
    return result;
}

// using bit manipulation
i32 isqrt(f64 x)
{
    i32 msb = (i32)(LOG2(x));

    i32 a = 1 << msb;
    i32 result = 0;
    while (a != 0)
    {
        if ((result + a) * (result + a) <= x)
            result += a;
        a >>= 1;
    }
    return result;
}

// using binary search
f64 sqrt(f64 x)
{
    // Max and min are used to take into account numbers less than 1
    f64 lo = MIN(1, x), hi = MAX(1, x), mid;

    // Update the bounds to be off the target by a factor of 10
    while (100 * lo * lo < x)
        lo *= 10;
    while (0.01 * hi * hi > x)
        hi *= 0.1;

    for (int i = 0; i < 100; i++)
    {
        mid = (lo + hi) / 2;
        if (mid * mid == x)
            return mid;
        if (mid * mid > x)
            hi = mid;
        else
            lo = mid;
    }
    return mid;
}

f64 cos_taylor_literal_6terms(f64 x)
{
    x = MODD(x, PI2);
    char sign = 1;
    if (x > PI)
    {
        x -= PI;
        sign = -1;
    }
    double xx = x * x;

    return sign * (1 - ((xx) / (2)) + ((xx * xx) / (24)) - ((xx * xx * xx) / (720)) + ((xx * xx * xx * xx) / (40320)) - ((xx * xx * xx * xx * xx) / (3628800)) + ((xx * xx * xx * xx * xx * xx) / (479001600)));
}

f64 cos(f64 x)
{
    return cos_taylor_literal_6terms(x);
}

f64 sin(f64 x)
{
    return cos(x - PI / 2.0);
}

f64 tan(f64 x)
{
    return sin(x) / cos(x);
}

// simple approximation of acos using lagrange polynomial
f64 acos(f64 x)
{
    return (-0.69813170079773212 * x * x - 0.87266462599716477) * x + 1.5707963267948966;
}

f64 asin(f64 x)
{
    return acos(x) - PI / 2.0;
}

f64 atan(f64 x)
{
    return asin(x / sqrt(1 + x * x));
}

f64 atan2(f64 y, f64 x)
{
    if (x > 0)
        return atan(y / x);
    else if (x < 0 && y >= 0)
        return atan(y / x) + PI;
    else if (x < 0 && y < 0)
        return atan(y / x) - PI;
    else if (x == 0 && y > 0)
        return PI / 2.0;
    else if (x == 0 && y < 0)
        return -PI / 2.0;
    else
        return 0;
}
