#pragma once
#include "Header.h"

// ¿eby ³atwo mo¿na by³o przekazywaæ dalej limity i j --- X to J, Y to I

int Limits::Distance(int a, int b)
{
    int result = b - a; if (result < 0) result *= -1;
    return result;
}
bool Limits::Distance_of_Both(int limit)
{
    int first = i_start - i_stop; if (first < 0) first *= -1;
    int second = j_start - j_stop; if (second < 0) second *= -1;

    return (first > limit && second > limit);
}
bool Limits::Border_or_Not(int** tab)
{
    int first_value = tab[i_start][j_start];

    for (int i = i_start; i < i_stop; i++) for (int j = j_start; j < j_stop; j++)

        if (first_value != tab[i][j]) return true;

    return false;
}

Limits::Limits() :i_start(0), i_stop(0), j_start(0), j_stop(0) 
{
}
Limits::Limits(int istart, int istop, int jstart, int jstop)
    : i_start(istart), i_stop(istop), j_start(jstart), j_stop(jstop) {}
Limits::Limits(const Limits& other)
{    
    i_start = other.i_start;
    i_stop = other.i_stop;

    j_start = other.j_start;
    j_stop = other.j_stop;
}
void Limits::Print() 
{
    cout << "i " << i_start << " -> " << i_stop << "   " << "j " << j_start << " -> " << j_stop << endl; 
}
void Limits::Print_offset(int x_offset, int y_offset)
{
    cout << "i " << i_start + y_offset << " -> " << i_stop + y_offset << "   " << "j " << j_start + x_offset << " -> " << j_stop + x_offset << endl;
}

myVector Limits::getCoordinetsCorner_UpLeft(bool recal)
{
    return myVector(j_start, i_start, recal);
}
myVector Limits::getCoordinetsCorner_UpRight(bool recal)
{
    return myVector(j_stop, i_start, recal);
}
myVector Limits::getCoordinetsCorner_DownLeft(bool recal)
{
    return myVector(j_start, i_stop, recal);
}
myVector Limits::getCoordinetsCorner_DownRight(bool recal)
{
    return myVector(j_stop, i_stop, recal);
}

Limits Limits::Get_up_left() {
    return Limits(
        i_start, (i_start + (Distance(i_start, i_stop)) / 2),
        j_start, (j_start + (Distance(j_start, j_stop)) / 2)
    );
}
Limits Limits::Get_up_right() {
    return Limits(
        i_start, (i_start + (Distance(i_start, i_stop)) / 2),
        (j_start + (Distance(j_start, j_stop)) / 2), j_stop
    );
}
Limits Limits::Get_down_left() {
    return Limits(
        i_start + (Distance(i_start, i_stop) / 2), i_stop,
        j_start, (j_start + (Distance(j_start, j_stop)) / 2)
    );
}
Limits Limits::Get_down_right() {
    return Limits(
        i_start + (Distance(i_start, i_stop) / 2), i_stop,
        (j_start + (Distance(j_start, j_stop)) / 2), j_stop
    );
}

Vector2f Limits::Get_middle_position()
{
    myVector res(
        (float)j_start + (Distance(j_start, j_stop) / 2),
        (float)i_start + (Distance(i_start, i_stop) / 2),
        false
    );

    // wczeœniejsze liczenie
    /*
    myVector res(
        i_start + (Distance(i_start, i_stop) / 2),
        j_start + (Distance(j_start, j_stop) / 2),
        false
    );
    */

    return *res;
}
Vector2f Limits::Get_middle_position_offset(int x_offset, int y_offset)
{

    myVector res(
        (float) j_start + (Distance(j_start, j_stop) / 2),
        (float) i_start + (Distance(i_start, i_stop) / 2),
        false
    );


    // wczeœniejsze liczenie œrodka
    /*
    myVector res(
        i_start + (Distance(i_start, i_stop) / 2),
        j_start + (Distance(j_start, j_stop) / 2),
        false
    );
    */

    res.Offset(x_offset, y_offset);
    return *res;
}

// In respect to the middle
Vector2f Limits::Middle_point_of_side_Left(int x_offset, int y_offset)
{
    //              x           y
    myVector res((float) j_start, (float) i_start + (Distance(i_start, i_stop) / 2), false);

    res.Offset(x_offset, y_offset);
    return *res;
}
Vector2f Limits::Middle_point_of_side_Right(int x_offset, int y_offset)
{
    //              x           y
    myVector res((float)j_stop, (float)i_start + (Distance(i_start, i_stop) / 2), false);

    res.Offset(x_offset, y_offset);
    return *res;
}
Vector2f Limits::Middle_point_of_side_Up(int x_offset, int y_offset)
{
    //                   x                                   y
    myVector res((float)j_start + (Distance(j_start, j_stop) / 2), (float)i_start, false);

    res.Offset(x_offset, y_offset);
    return *res;
}
Vector2f Limits::Middle_point_of_side_Down(int x_offset, int y_offset)
{
    //                    x                                  y
    myVector res((float)j_start + (Distance(j_start, j_stop) / 2), (float)i_stop, false);

    res.Offset(x_offset, y_offset);
    return *res;
}