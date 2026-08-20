#include "func.h"

float trimf(float x, float a, float b, float c)
{
    if (x <= a || x >= c)
        return 0.0;

    if (x == b)
        return 1.0;

    if (x < b)
        return (x - a) / (b - a);

    return (c - x) / (c - b);
}

float trapmf(float x, float a, float b, float c, float d)
{
    if (x <= a || x >= d)
        return 0.0;

    if (x >= b && x <= c)
        return 1.0;

    if (x > a && x < b)
        return (x - a) / (b - a);

    return (d - x) / (d - c);
}