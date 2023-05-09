#pragma once
#include "header.h"



// Controling every cell //
void InterestingVisualizations::Body_Recursion_with_Cross_in_Rectangle(list<VertexArray>& cross_list, Limits obj, int& x_offset, int& y_offset, recursion_limits limits)
{
    cross_list.push_back(Cross_with_lines___with_limits(obj, x_offset, y_offset));
    //cout << "working" << endl;

    limits.up_left_limit--;
    limits.up_right_limit--;
    limits.down_left_limit--;
    limits.down_right_limit--;

    if (limits.up_left_limit > 0)
    {
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_up_left(), x_offset, y_offset, limits);
    }
    if (limits.up_right_limit > 0)
    {
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_up_right(), x_offset, y_offset, limits);
    }
    if (limits.down_left_limit > 0)
    {
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_down_left(), x_offset, y_offset, limits);
    }
    if (limits.down_right_limit > 0)
    {
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_down_right(), x_offset, y_offset, limits);
    }
        limits.up_left_limit++;
        limits.up_right_limit++;
        limits.down_left_limit++;
        limits.down_right_limit++;
}

void InterestingVisualizations::Body_Recursion_with_Cross_in_Rectangle(list<VertexArray>& cross_list, Limits obj, int& x_offset, int& y_offset, int limit)
{
    cross_list.push_back(Cross_with_lines___with_limits(obj, x_offset, y_offset));
    //cout << "working" << endl;

    if (limit > 0)
    {
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_up_left(), x_offset, y_offset, limit - 1);
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_up_right(), x_offset, y_offset, limit - 1);
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_down_left(), x_offset, y_offset, limit - 1);
        Body_Recursion_with_Cross_in_Rectangle(cross_list, obj.Get_down_right(), x_offset, y_offset, limit - 1);
    }
}

void InterestingVisualizations::Body_RecursionWithTriangles(list<VertexArray>& triangle_list, Triangle t, offSet off, int limit)
{
    triangle_list.push_back(drawTriangle(t, Color::Yellow, off));

    if (limit > 0)
    {
        Body_RecursionWithTriangles(triangle_list, t.downLeft(), off, limit - 1);
        Body_RecursionWithTriangles(triangle_list, t.downRight(), off, limit - 1);
        Body_RecursionWithTriangles(triangle_list, t.up(), off, limit - 1);
        Body_RecursionWithTriangles(triangle_list, t.middle(), off, limit - 1);
    }
}

void InterestingVisualizations::Body_RecursionWithTriangles(list<VertexArray>& triangle_list, Triangle t, offSet off, recursion_limits_for_Triangle limit)
{
    triangle_list.push_back(drawTriangle(t, Color::Yellow, off));

    limit.downLeft--;
    limit.downRight--;
    limit.up--;
    limit.middle--;

    if (limit.downLeft > 0) Body_RecursionWithTriangles(triangle_list, t.downLeft(), off, limit);
    if (limit.downRight > 0) Body_RecursionWithTriangles(triangle_list, t.downRight(), off, limit);
    if (limit.up > 0) Body_RecursionWithTriangles(triangle_list, t.up(), off, limit);
    if (limit.middle > 0) Body_RecursionWithTriangles(triangle_list, t.middle(), off, limit);

    limit.downLeft++;
    limit.downRight++;
    limit.up++;
    limit.middle++;    
}



list<VertexArray> InterestingVisualizations::RecursionWithTriangles(Triangle starting, offSet off, int limits)
{
    list<VertexArray> triangle_list;
    Body_RecursionWithTriangles(triangle_list, starting, off, limits);

    return triangle_list;
}

list<VertexArray> InterestingVisualizations::RecursionWithTriangles(Triangle starting, offSet off, 
    int down_left, int down_right, int up, int middle)
{
    list<VertexArray> triangle_list;
    InterestingVisualizations::recursion_limits_for_Triangle limits(down_left, down_right, up, middle);
    Body_RecursionWithTriangles(triangle_list, starting, off, limits);

    return triangle_list;
}


list<VertexArray> InterestingVisualizations::Recursion_with_Cross_in_Rectangle(int width, int height, int x_offset, int y_offset, int limits)
{
    Limits tmp(0, height, 0, width);

    int x = x_offset;
    int y = y_offset;

    list<VertexArray> cross_list;
    Body_Recursion_with_Cross_in_Rectangle(cross_list, tmp, x, y, limits);

    return cross_list;
}

list<VertexArray> InterestingVisualizations::Recursion_with_Cross_in_Rectangle(int width, int height, int x_offset, int y_offset,
    int up_left, int up_right, int down_left, int down_right)
{
    Limits tmp(0, height, 0, width);

    recursion_limits l(up_left, up_right, down_left, down_right);

    int x = x_offset;
    int y = y_offset;

    list<VertexArray> cross_list;
    Body_Recursion_with_Cross_in_Rectangle(cross_list, tmp, x, y, l);

    return cross_list;
}