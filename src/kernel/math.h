#pragma once
#include <stdint.h>
#define modd(x, y) ((x) - (int)((x) / (y)) * (y))
#define CONST_PI 3.14159265358979323846
#define CONST_2PI 6.28318530717958647692

int min(int a, int b);

int max(int a, int b);

double pow(double a, double b);

double cos(double x);
double sin(double x);
double tan(double x);
double atan(double x);
double atan2(double y, double x);
double asin(double x);
double acos(double x);

double floor(double x);

double sqrt(double x);

double modf(double x, double *iptr);

float Q_rsqrt(float number);

int log2(int x);

int fac(int x);

int abs(int x);
