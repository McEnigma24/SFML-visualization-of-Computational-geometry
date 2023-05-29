#pragma once
#include "Header.h"

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
Float::Float(const Float& other)
{
    y = other.y;
}
Float::Float(Float other, bool recal_y)
{
    if (recal_y) y = (float)REC(other.y);
    else y = (float)other.y;
}
Float& Float::operator=(float yy)
{
    y = yy;
    return *this;
}


// conversion function
Float::operator int() const
{
    return static_cast<int>(y);
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


bool Float::operator==(const Float& other) const
{
    return ((int)y == (int)other.y);
}
bool Float::operator!=(const Float& other) const
{
    return !this->operator==(other);
}

bool Float::operator<(const Float& other)
{
    return (this->y < other.y);
}
bool Float::operator<=(const Float& other)
{
    return (this->y <= other.y);
}

bool Float::operator>(const Float& other)
{
    return (this->y > other.y);
}
bool Float::operator>=(const Float& other)
{
    return (this->y >= other.y);
}