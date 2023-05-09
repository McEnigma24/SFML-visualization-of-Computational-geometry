#pragma once
#include "header.h"

list<CircleShape*> VertexArrayFunctions::drawFileInputPoints(string p, float rad)
{
    list<CircleShape*> res;
    const char* path = p.c_str();

    ifstream FILE(path);
    if (!FILE.good()) return res;

    ifstream FILEcheck_if_empty(path);

    double receved_x;
    double receved_y;

    myCircle cir;
    int i = 1;
    string line; string jump;
    while (!FILE.eof())
    {
        getline(FILEcheck_if_empty, line);
        if (line == "" || line == "\r\n")
        {
            getline(FILE, jump);
            continue;
        }

        FILE >> receved_x;
        FILE >> receved_y;

        CircleShape* c = new CircleShape();
        c->setFillColor(Color::Green);
        c->setRadius(rad);
        c->setPosition(*myVector(receved_x, receved_y, true));
        c->setPosition(cir.setMiddle(c));

        res.push_back(c);
    }

    return res;
}

VertexArray VertexArrayFunctions::drawLine(Line l, Color c)
{
    VertexArray lines(sf::LineStrip, 2);
    lines[0] = l.point1; lines[0].color = c;
    lines[1] = l.point2; lines[1].color = c;

    return lines;
}

VertexArray VertexArrayFunctions::drawTriangle(Triangle t, Color c, offSet off)
{
    VertexArray lines(sf::LineStrip, 4);

    myVector A(t.A, false);     A.x += off.x_offset; A.y += off.y_offset;
    myVector B(t.B, false);     B.x += off.x_offset; B.y += off.y_offset;
    myVector C(t.C, false);     C.x += off.x_offset; C.y += off.y_offset;

    lines[0] = *A; lines[0].color = c;
    lines[1] = *B; lines[1].color = c;
    lines[2] = *C; lines[2].color = c;
    lines[3] = *A; lines[3].color = c;

    return lines;
}
list<CircleShape> VertexArrayFunctions::drawPointsOnEndOfTriangle(Triangle t, Color c, offSet off, float rad)
{
    list<CircleShape> res_list;
    
    {
        myVector A(t.A, false);     A.x += off.x_offset; A.y -= off.y_offset;
        myVector B(t.B, false);     B.x += off.x_offset; B.y -= off.y_offset;
        myVector C(t.C, false);     C.x += off.x_offset; C.y -= off.y_offset;
        
        myCircle cir;

        // Adding A
        {
            CircleShape circle;
            circle.setFillColor(c);
            circle.setRadius(rad);
            circle.setPosition(*A);
            circle.setPosition(cir.setMiddle(circle));

            res_list.push_back(circle);
        }

        // Adding B
        {
            CircleShape circle;
            circle.setFillColor(c);
            circle.setRadius(rad);
            circle.setPosition(*B);
            circle.setPosition(cir.setMiddle(circle));

            res_list.push_back(circle);
        }

        // Adding C
        {
            CircleShape circle;
            circle.setFillColor(c);
            circle.setRadius(rad);
            circle.setPosition(*C);
            circle.setPosition(cir.setMiddle(circle));

            res_list.push_back(circle);
        }
    }  


    return res_list;
}