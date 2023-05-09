#pragma once
#include "header.h"

Line::Line()
    :a(0), b(0), point1(0, 0), point2(0, 0), intPoint1(0, 0), intPoint2(0, 0)
{
}
Line::Line(double aa, double bb)
{
    a = aa;
    b = bb;

    point1.x = 0; point1.y = 0;
    point2.x = 0; point2.y = 0;

    intPoint1.x = 0; intPoint1.y = 0;
    intPoint2.x = 0; intPoint2.y = 0;
}
Line::Line(Vector2f p1, Vector2f p2)
{
    point1 = p1;
    point2 = p2;

    intPoint1.x = p1.x;     intPoint1.y = p1.y;
    intPoint2.x = p2.x;     intPoint2.y = p2.y;

    a = ((p1.y - p2.y) / (p1.x - p2.x));
    b = (p1.y - (a * p1.x));
}
Line::Line(CircleShape c1, CircleShape c2)
{
    point1 = getMiddle(c1);
    point2 = getMiddle(c2);

    intPoint1.x = getMiddle(c1).x;     intPoint1.y = getMiddle(c1).y;
    intPoint2.x = getMiddle(c2).x;     intPoint2.y = getMiddle(c2).y;

    a = ((point1.y - point2.y) / (point1.x - point2.x));
    b = (point1.y - (a * point1.x));
}

bool Line::operator==(const Line& other) const
{
    return (point1 == other.point1 &&
        point2 == other.point2 &&
        a == other.a &&
        b == other.b);
}
bool Line::operator!=(const Line& other) const
{
    return !(this->operator==(other));
}

// intersecting 2 lines
Vector2f Line::Point_of_Intersect(Line l1, Line l2)
{
    Vector2f res;

    res.x = ((l2.b - l1.b) / (l1.a - l2.a));
    res.y = (l1.a * res.x) + l1.b;

    return res;
}

Line Line::Line_between(Vector2f p1, Vector2f p2)
{
    Line result;

    result.point1 = p1;
    result.point2 = p2;
    result.a = ((p1.y - p2.y) / (p1.x - p2.x));
    result.b = (p1.y - (result.a * p1.x));

    return result;
}
Vector2f Line::MiddleOfLine()
{
    float x = point2.x - point1.x;
    float y = point2.y - point1.y;

    return Vector2f((point1.x + (x / 2)), (point1.y + (y / 2)));
    //return RandomPointOnLine();
}

Vector2f Line::RandomPointOnLine()
{
    float distance_x = point1.x - point2.x; if (distance_x < 0) distance_x *= (-1);
    float distance_y = point1.y - point2.y; if (distance_y < 0) distance_y *= (-1);

    Vector2f res(0,0);
    if (distance_x > distance_y)
    {
        float r3;
        if (point1.x < point2.x) r3 = point1.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (point2.x - point1.x)));
        else                     r3 = point2.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (point1.x - point2.x)));

        res.x = r3;
        res.y = a * res.x + b;

        return res;
    }
    else
    {
        float r4;
        if (point1.x < point2.x) r4 = point1.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (point2.x - point1.x)));
        else                     r4 = point2.x + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (point1.x - point2.x)));

        res.y = r4;
        res.x = (res.y - b) / a;

        return res;
    }
    
    return res;
}
Vector2f Line::NextPointAfter(Vector2f before, float jump, bool forward_backword)
{
    // wystarczy sprawdziæ gdzie idzie "x" i obliczyæ "y"
    // która æwiartka
    myVector res(before, false);

    // forward
    if (forward_backword)
    {
        float x_direction = point2.x - point1.x;        
                
        if (x_direction > 0)
        {
            res.x += jump;

            // jak wiêszy to zwracamy pocz¹tek lini
            if (res.x > point2.x) return point1;

            res.y = ((res.x * this->a) + this->b);
        }

        else
        {
            res.x -= jump;

            // jak wiêszy to zwracamy pocz¹tek lini
            if (res.x < point2.x) return point1;

            res.y = ((res.x * this->a) + this->b);
        }
    }

    // backword
    else
    {
        float x_direction = point1.x - point2.x;
        
        if (x_direction > 0)
        {
            res.x += jump;

            // jak wiêszy to zwracamy pocz¹tek lini
            if (res.x > point1.x) return point2;

            res.y = ((res.x * this->a) + this->b);
        }

        
        else
        {
            res.x -= jump;

            // jak wiêszy to zwracamy pocz¹tek lini
            if (res.x < point1.x) return point2;

            res.y = ((res.x * this->a) + this->b);
        }
    }

    return *res;
}

float Line::Angle_between(Vector2f middle, Vector2f left, Vector2f right)
{
    float middle_to_left = Get_length(middle, left);
    float middle_to_right = Get_length(middle, right);
    float left_to_right = Get_length(left, right);

    float a = left_to_right;
    float b = middle_to_left;
    float c = middle_to_right;

    double for_acos = ((a * a - b * b - c * c) / ((-2) * b * c));
    return acos(for_acos);
}
float Line::Angle_between(CircleShape* point_of_reference, CircleShape* m_point)
{
    if (!point_of_reference) cout << "point_of_reference to nullptr" << endl;
    if (!m_point) cout << "m_point to nullptr" << endl;

    Vector2f reference_position = getMiddle(point_of_reference);
    Vector2f measuring_position = getMiddle(m_point);

    Line parallel(reference_position, Vector2f(reference_position.x + 100, reference_position.y));
    Line perpendicular(measuring_position, Vector2f(measuring_position.x, measuring_position.y + 100));

    Vector2f interset = Point_of_Intersect(parallel, perpendicular);

    return Angle_between(reference_position, interset, measuring_position);
}