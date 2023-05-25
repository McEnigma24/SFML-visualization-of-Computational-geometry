#pragma once
#include "Header.h"

myCircle::myCircle()
{
    m_id = 0;
    m_circle = make_shared<CircleShape>();
}
myCircle::myCircle(int id)
{
    m_id = id;
    m_circle = make_shared<CircleShape>();
}

// wspó³gra z odwróconymi operacjami dla "y"
Vector2f myCircle::getMiddle() const
{
    myVector myvec(*m_circle, false);
    float rad = m_circle->getRadius();

    // tak na prawdê nie rad, tylko sqrt(2)/2 * rad
    // rad *= sqrt(2);

    // jednak bez ró¿nicy

    myvec.x += rad;
    myvec.y -= rad;

    return *myvec;
}
Vector2f myCircle::getMiddle(CircleShape c)
{
    myVector myvec(c, false);
    float rad = c.getRadius();

    // tak na prawdê nie rad, tylko sqrt(2)/2 * rad
    // rad *= sqrt(2);

    // jednak bez ró¿nicy

    myvec.x += rad;
    myvec.y -= rad;

    return *myvec;
}
Vector2f myCircle::getMiddle(CircleShape* c)
{
    return getMiddle(*c);
}

Vector2f myCircle::setMiddle()
{
    myVector myvec(*m_circle, false);
    float rad = m_circle->getRadius();

    myvec.x -= rad;
    myvec.y += rad;

    return *myvec;
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

bool myCircle::operator==(const myCircle& other)
{
    return (m_id == other.m_id && m_circle == other.m_circle);
}
bool myCircle::operator!=(const myCircle& other)
{
    return !this->operator==(other);
}