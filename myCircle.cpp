#pragma once
#include "header.h"

// wsp�gra z odwr�conymi operacjami dla "y"
Vector2f myCircle::getMiddle(CircleShape c)
{
    myVector myvec(c, false);
    float rad = c.getRadius();

    // tak na prawd� nie rad, tylko sqrt(2)/2 * rad
    // rad *= sqrt(2);

    // jednak bez r�nicy

    myvec.x += rad;
    myvec.y -= rad;

    return *myvec;
}
Vector2f myCircle::getMiddle(CircleShape* c)
{
    return getMiddle(*c);
}

Vector2f myCircle::setMiddle(CircleShape c)
{
    myVector myvec(c, false);
    float rad = c.getRadius();

    myvec.x -= rad;
    myvec.y += rad;

    return *myvec;
}
Vector2f myCircle::setMiddle(CircleShape* c)
{
    return setMiddle(*c);
}