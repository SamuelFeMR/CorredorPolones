#include "func.h"

float trimf(float x, float a, float b, float c)
{
    if (x < a || x > c)
        return 0.0f;

    if (x == b)
        return 1.0f;

    if (x < b)
    {
        if (b == a)
            return 1.0f;

        return (x - a) / (b - a);
    }

    if (c == b)
        return 1.0f;

    return (c - x) / (c - b);
}

float trapmf(float x, float a, float b, float c, float d)
{
    if (x < a || x > d)
        return 0.0f;

    if (x >= b && x <= c)
        return 1.0f;

    if (x < b)
    {
        if (b == a)
            return 1.0f;

        return (x - a) / (b - a);
    }

    if (d == c)
        return 1.0f;

    return (d - x) / (d - c);
}