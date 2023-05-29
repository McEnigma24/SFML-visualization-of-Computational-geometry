#pragma once
#include "Header.h"

// hashing function for myVector and Float


// Constructor //
myVector::myVector(bool recal_y): x(0), y(0, recal_y){}
myVector::myVector(float xx, float yy, bool recal_y)
    :x(xx), y(Float(yy, recal_y))
{
    //LOG("Float copied");
}
myVector::myVector(int xx, int yy, bool recal_y)
    : x(static_cast<float>(xx)), y(Float(yy, recal_y))
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
myVector& myVector::operator=(const myVector& other)
{
    x = other.x;
    y = other.y;

    return *this;
}

// After Constructor //
void myVector::afterConstructor(bool recal_y)
{
    x = 0;
    y = Float(0, recal_y);
}
void myVector::afterConstructor(float xx, float yy, bool recal_y)
{
    x = xx;
    y = Float(yy, recal_y);
}

void myVector::afterConstructor(Vector2f v, bool recal_y)
{
    x = v.x;
    y = Float(v.y, recal_y);
}
void myVector::afterConstructor(CircleShape c, bool recal_y)
{
    Vector2f v = c.getPosition();

    x = v.x;
    y = Float(v.y, recal_y);
}
void myVector::afterConstructor(CircleShape* c, bool recal_y)
{
    Vector2f v = c->getPosition();

    x = v.x;
    y = Float(v.y, recal_y);
}

// Just recal y //
void myVector::recalExisting()
{    
    y = Float(y, true);    
}

// Offset //
void myVector::Offset(int x_offset, int y_offset)
{
    /*
    myVector res(x_offset, y_offset, true);

    res.x += x_offset;
    res.y += y_offset;

    this->x = res.x; this->y = res.y;
    */


    x += (float)x_offset;
    y -= (float)y_offset;
}
void myVector::Offset(offSet off)
{
    x += (float)off.x_offset;
    y -= (float)off.y_offset;
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

bool myVector::operator==(const myVector& other) const
{
    return (((int) x == (int) other.x) && ((int) y == (int) other.y));
}
bool myVector::operator!=(const myVector& other) const
{
    return !this->operator==(other);
}