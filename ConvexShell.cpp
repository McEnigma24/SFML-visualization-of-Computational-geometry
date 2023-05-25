#pragma once
#include "Header.h"

// jedna funkcja ma definicjê w header.h
ConvexShell::Angle_and_Point::Angle_and_Point() :angle(0), measuring_point(nullptr) {}
ConvexShell::Angle_and_Point::Angle_and_Point(myCircle* point_of_reference, myCircle* m_point)
{
    if (point_of_reference == m_point)        MY_ERROR("Angle and Point --- pointery s¹ takie same");

    measuring_point = m_point;

    Line l;
    angle = l.Angle_between(point_of_reference, m_point);
}

ConvexShell::Angle_and_Point::Angle_and_Point(myCircle* befor_last_to_reference, myCircle* last_to_reference, myCircle* m_point)
{
    measuring_point = m_point;

    Vector2f first_position;

    // jak null to bierzemy pozycje last_to_reference
    if (befor_last_to_reference == nullptr)
    {
        first_position = last_to_reference->getMiddle();
        first_position = Vector2f(first_position.x - 50, first_position.y);
    }
    else first_position = befor_last_to_reference->getMiddle();

    Vector2f second_position = last_to_reference->getMiddle();
    Vector2f m_position = m_point->getMiddle();

    Line l;
    angle = l.Angle_between(second_position, first_position, m_position);
}


bool ConvexShell::Compare_for_Sort(Angle_and_Point a, Angle_and_Point b) { return (a.angle < b.angle); }
myCircle* ConvexShell::bottom_point(vector<myCircle>& vec_of_points)
{
    myCircle* bottom = &vec_of_points.front();    

    for (auto& l : vec_of_points)
        if (l.getMiddle().y < bottom->getMiddle().y) bottom = &l;

    return bottom;
}

myCircle* ConvexShell::smallest_angle_point(vector<Angle_and_Point>& vec)
{
    Angle_and_Point point_with_smallest_angle_to = vec.front();

    for (auto& v : vec)
        if (v.angle < point_with_smallest_angle_to.angle) point_with_smallest_angle_to = v;

    return (point_with_smallest_angle_to.measuring_point);
}



vector<myCircle> ConvexShell::Alg_Graham(vector<myCircle>& vec_of_points)
{
    // dodawaæ do tego jak bêdzie wykorzystany jakiœ
    // points_in_use, ¿eby mo¿na by³o z niego kasowaæ ju¿ wykorzystane punkty
    // PóŸniej zmieniæ, ¿eby by³ points_in_use

    // lista punktów otoczki
    vector<myCircle> points_in_ConvexShell;

    if (vec_of_points.size() < 3) return points_in_ConvexShell;


    // lista z której bêdziemy usuwaæ wykorzystane punkty, ¿eby niepotrzebnie nie sprawdzaæ
    list<myCircle> points_in_use;
    points_in_use.clear();  for (auto& i : vec_of_points) points_in_use.push_back(i);


    // pierwszy punkt
    myCircle* startpoint_bottom = bottom_point(vec_of_points);
    points_in_ConvexShell.push_back(*startpoint_bottom);

    #if BENCHMARKING == 0
        LOG("list of points " << vec_of_points.size());
        LOG("points_in_use " << points_in_use.size());
        LOG("points_in_ConvexShell " << points_in_ConvexShell.size());
    #endif
    // zatrzyma siê jak domkniemy otoczke
    while ((points_in_ConvexShell.size() == 1 ||
        ((points_in_ConvexShell.front()) != (points_in_ConvexShell.back()))))
    {
        // dla pierwszego punktu musi byæ prosta równoleg³a do osi x
        // 
        // z points_in_use usuwam wszystkie u¿yte oprócz pierwszego

        // bierze ostatni dodany element i porównuje k¹ty do wszytkich innych

        vector<myCircle>::iterator _end = points_in_ConvexShell.end();

        // pointery
        myCircle* last_to_reference = nullptr;
        myCircle* befor_last_to_reference = nullptr;

        if (points_in_ConvexShell.size() == 1)
        {
            vector<myCircle>::iterator last = std::prev(_end);
            last_to_reference = &(*last);
        }
        else
        {
            vector<myCircle>::iterator last = std::prev(_end);
            vector<myCircle>::iterator before_last = std::prev(std::prev(_end));

            befor_last_to_reference = &(*before_last);
            last_to_reference = &(*last);
        }

        Angle_and_Point biggest_angle_to;
        for (auto& p : points_in_use)
        {
            if (befor_last_to_reference != nullptr)
            {
                if (&*(befor_last_to_reference->m_circle) != &(*(p.m_circle)) && &*(last_to_reference->m_circle) != &(*(p.m_circle)))
                {
                    //Angle_and_Point tmp(point_of_reference, p);

                    // Dawaæ tutaj
                    Angle_and_Point tmp(befor_last_to_reference, last_to_reference, &p);

                    if (p == points_in_use.front() || tmp.angle > biggest_angle_to.angle)
                        biggest_angle_to = tmp;
                }
            }
            else
            {
                if (&*(last_to_reference->m_circle) != &(*(p.m_circle)))
                {
                    //Angle_and_Point tmp(point_of_reference, p);

                    // Dawaæ tutaj
                    Angle_and_Point tmp(nullptr, last_to_reference, &p);

                    if (p == points_in_use.front() || tmp.angle > biggest_angle_to.angle)
                        biggest_angle_to = tmp;
                }
            }
        }

        // dodajemy punkt do otoczki
        points_in_ConvexShell.push_back(*(biggest_angle_to.measuring_point));

        // usuwamy z listy points_in_use
        //points_in_use.remove(*biggest_angle_to.measuring_point);
    }

    for (auto& con : points_in_ConvexShell)
        con.m_circle->setFillColor(Color::White);

    return points_in_ConvexShell;
}