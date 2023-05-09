#pragma once
#include "header.h"

float Float::Get_y()
{
    return y;
}

// r-value // never used
/*
Float(float&& yy, bool recal_y)
{
    LOG("&& constructor");
    if (recal_y) y = REC(yy);
    else y = yy;
}
*/
// l-value
Float::Float(float yy, bool recal_y)
{
    //LOG("normal constructor");
    if (recal_y) y = REC(yy);
    else y = yy;
}
Float::Float(int yy, bool recal_y)
{
    if (recal_y) y = (float)REC(yy);
    else y = (float)yy;
}
Float& Float::operator=(float yy)
{
    y = yy;
    return *this;
}

void Float::operator+=(float add)
{
    //LOG("Float += operator");
    y -= add;
}
void Float::operator-=(float sub)
{
    //LOG("Float -= operator");
    y += sub;
}