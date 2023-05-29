#pragma once
#include "Header.h"

Triangle::Triangle()
{
    A = Vector2f(0, 0);
    B = Vector2f(0, 0);
    C = Vector2f(0, 0);

    Aangle = 0;
    Bangle = 0;
    Cangle = 0;

    id_A = 0;
    id_B = 0;
    id_C = 0;
}
Triangle::Triangle(Vector2f aa, Vector2f bb, Vector2f cc)
{
    A = aa;
    B = bb;
    C = cc;

    Aangle = 0;
    Bangle = 0;
    Cangle = 0;

    id_A = 0;
    id_B = 0;
    id_C = 0;
}
Triangle::Triangle(CircleShape aa, CircleShape bb, CircleShape cc)
{
    myCircle cir;

    A = cir.getMiddle(aa);
    B = cir.getMiddle(bb);
    C = cir.getMiddle(cc);

    Aangle = 0;
    Bangle = 0;
    Cangle = 0;

    id_A = 0;
    id_B = 0;
    id_C = 0;
}
void Triangle::infoAboutPosition()
{
    cout << "A - " << A.x << " " << A.y << endl;
    cout << "B - " << B.x << " " << B.y << endl;
    cout << "C - " << C.x << " " << C.y << endl;
}

void Triangle::getAngles()
{
    if (!(A != Vector2f(0, 0) && B != Vector2f(0, 0) && C != Vector2f(0, 0))) return;

    Line l;

    Aangle = l.Angle_between(A, B, C);
    Bangle = l.Angle_between(B, A, C);
    Cangle = l.Angle_between(C, B, A);
}
bool Triangle::areAllPointsInLine()
{
    Line line(A, B);
    double result = (line.a * C.x) + line.b;

    double limit = 2.0;
    if (-limit <= C.y - result && C.y - result <= limit) return true;
    return false;
}
bool Triangle::anyCommnoPointWith(Triangle t)
{
    list<Vector2f> other;           other.push_back(t.A); other.push_back(t.B); other.push_back(t.C);

    for (auto& o : other)
    {
        if (o == A ||
            o == B ||
            o == C)
            return true;
    }
    return false;
}

Vector2f Triangle::getPositionForNumber()
{
    // stare nie dzia³a
    /*Line l;

    return l.Point_of_Intersect(l.lineInTheMiddleOf2(Line(A, B), Line(A, C))
                              , l.lineInTheMiddleOf2(Line(A, C), Line(C, B)));*/

    Operations o;

    float a = o.Get_length(B, C);
    float b = o.Get_length(A, C);
    float c = o.Get_length(A, B);

    Vector2f res;

    res.x = ((a * A.x) + (b * B.x) + (c * C.x)) / (a + b + c);
    res.y = ((a * A.y) + (b * B.y) + (c * C.y)) / (a + b + c);

    return res;
}
Vector2f Triangle::getPositionForNumber() const
{
    // stare nie dzia³a
    /*Line l;

    return l.Point_of_Intersect(l.lineInTheMiddleOf2(Line(A, B), Line(A, C))
                              , l.lineInTheMiddleOf2(Line(A, C), Line(C, B)));*/

    Operations o;

    float a = o.Get_length(B, C);
    float b = o.Get_length(A, C);
    float c = o.Get_length(A, B);

    Vector2f res;

    res.x = ((a*A.x) + (b * B.x) + (c * C.x)) / (a + b + c);
    res.y = ((a*A.y) + (b * B.y) + (c * C.y)) / (a + b + c);

    return res;
}


bool Triangle::operator==(const Triangle& other)
{
    return (A == other.A && B == other.B && C == other.C);
}
bool Triangle::operator!=(const Triangle& other)
{
    return !(this->operator==(other));
}

bool Triangle::operator==(const Triangle& other) const
{
    return (A == other.A && B == other.B && C == other.C);
}
bool Triangle::operator!=(const Triangle& other) const
{
    return !(this->operator==(other));
}

Triangle Triangle::downLeft()
{
    Triangle res;

    Line ab(A,B);
    Line ac(A,C);

    res.A = ab.MiddleOfLine();
    res.B = ac.MiddleOfLine();
    res.C = A;

    return res;
}
Triangle Triangle::downRight()
{
    Triangle res;

    Line one(C, A);
    Line two(C, B);

    res.A = one.MiddleOfLine();
    res.B = two.MiddleOfLine();
    res.C = C;

    return res;
}
Triangle Triangle::up()
{
    Triangle res;

    Line one(B, A);
    Line two(B, C);

    res.A = one.MiddleOfLine();
    res.B = two.MiddleOfLine();
    res.C = B;

    return res;
}
Triangle Triangle::middle()
{
    Triangle res;

    Line one(A, B);
    Line two(A, C);
    Line three(B, C);

    res.A = one.MiddleOfLine();
    res.B = two.MiddleOfLine();
    res.C = three.MiddleOfLine();

    return res;
}