#pragma once
#include "header.h"

myVector::myVector(bool recal_y): x(0), y(0, recal_y){}
myVector::myVector(float xx, float yy, bool recal_y)
    :x(xx), y(Float(yy, recal_y))
{
    //LOG("Float copied");
}

myVector::myVector(Vector2f v, bool recal_y)
    : x(v.x), y(v.y, recal_y)
{
}
myVector::myVector(CircleShape c, bool recal_y)
    : x(c.getPosition().x), y(c.getPosition().y, recal_y)
{
}
myVector::myVector(CircleShape* c, bool recal_y)
    : x(c->getPosition().x), y(c->getPosition().y, recal_y)
{
}

void myVector::Offset(int x_offset, int y_offset)
{
    /*
    myVector res(x_offset, y_offset, true);

    res.x += x_offset;
    res.y += y_offset;

    this->x = res.x; this->y = res.y;
    */


    x += x_offset;
    y -= y_offset;
}
void myVector::Offset(offSet off)
{
    x += off.x_offset;
    y -= off.y_offset;
}

void myVector::Print()
{
    cout << "x: " << x << "   y: " << y.Get_y() << endl;
}

Vector2f myVector::operator*()
{
    Vector2f res;
    res.x = x;
    res.y = y.Get_y();

    return res;
}