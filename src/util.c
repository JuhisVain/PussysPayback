#include "util.h"

int sign(int num)
{
    return (num<0)?-1:(num>0)?1:0;
}

double distance(int ox, int oy, int tx, int ty)
{
    return sqrt(((tx-ox)^2) + ((ty-oy)^2));
}
