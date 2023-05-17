#ifndef MATH_H
#define MATH_H
#include <types.h>

#define PI 3.14159265358979323846
#define PI2 6.28318530717958647692
#define E 2.71828182845904523536
#define LOG2E 1.44269504088896340736
#define LOG10E 0.434294481903251827651
#define LN2 0.693147180559945309417
#define LN10 2.30258509299404568402
#define SQRT2 1.41421356237309504880
#define SQRT1_2 0.707106781186547524401

#define MODD(x, y) ((x) - (int)((x) / (y)) * (y))
#define LOG2(X) ((unsigned)(8 * sizeof(unsigned long long) - __builtin_clzll((X)) - 1))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MAX_INT ((i32)0x7FFFFFFF)
#define MIN_INT ((i32)0x80000000)
#define MAX_UINT ((u32)0xFFFFFFFF)
#define MIN_UINT ((u32)0x00000000)
#define MAX_LONG ((i64)0x7FFFFFFFFFFFFFFF)
#define MIN_LONG ((i64)0x8000000000000000)
#define MAX_ULONG ((u64)0xFFFFFFFFFFFFFFFF)
#define MIN_ULONG ((u64)0x0000000000000000)

f64 abs(f64 x);
i64 ceil(f64 x);
i64 floor(f64 x);
i64 pow(i64 x, i64 y);
f64 log(f64 x);
i32 isqrt(f64 x);
f64 sqrt(f64 x);
f64 cos(f64 x);
f64 sin(f64 x);
f64 tan(f64 x);
f64 acos(f64 x);
f64 asin(f64 x);
f64 atan(f64 x);
f64 atan2(f64 y, f64 x);

#endif