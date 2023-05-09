#pragma once
#include "header.h"

// jedna funkcja ma definicjê w header.h
ConvexShell::Angle_and_Point::Angle_and_Point() :angle(0), measuring_point(nullptr) {}
ConvexShell::Angle_and_Point::Angle_and_Point(CircleShape* point_of_reference, CircleShape* m_point)
{
    if (point_of_reference == m_point) LOG("Angle and Point --- pointery s¹ takie same");

    measuring_point = m_point;

    Line l;
    angle = l.Angle_between(point_of_reference, m_point);
}

ConvexShell::Angle_and_Point::Angle_and_Point(CircleShape* befor_last_to_reference, CircleShape* last_to_reference, CircleShape* m_point)
{
    myCircle c;

    measuring_point = m_point;

    Vector2f first_position;

    // jak null to bierzemy pozycje last_to_reference
    if (befor_last_to_reference == nullptr)
    {
        first_position = c.getMiddle(last_to_reference);
        first_position = Vector2f(first_position.x - 50, first_position.y);
    }
    else first_position = c.getMiddle(befor_last_to_reference);

    Vector2f second_position = c.getMiddle(last_to_reference);
    Vector2f m_position = c.getMiddle(m_point);

    Line l;
    angle = l.Angle_between(second_position, first_position, m_position);
}


bool ConvexShell::Compare_for_Sort(Angle_and_Point a, Angle_and_Point b) { return (a.angle < b.angle); }
CircleShape* ConvexShell::bottom_point(const list<CircleShape*>& list_of_points)
{
    myCircle c;
    CircleShape* bottom = *(list_of_points.begin());

    for (auto& l : list_of_points)
        if (c.getMiddle(l).y < c.getMiddle(bottom).y) bottom = l;

    return bottom;
}
CircleShape* ConvexShell::smallest_angle_point(vector<Angle_and_Point>& vec)
{
    Angle_and_Point point_with_smallest_angle_to = vec.front();

    for (auto& v : vec)
        if (v.angle < point_with_smallest_angle_to.angle) point_with_smallest_angle_to = v;

    return point_with_smallest_angle_to.measuring_point;
}



list<CircleShape*> ConvexShell::Alg_Graham(const list<CircleShape*>& list_of_points)
{
    // dodawaæ do tego jak bêdzie wykorzystany jakiœ
    // points_in_use, ¿eby mo¿na by³o z niego kasowaæ ju¿ wykorzystane punkty
    // PóŸniej zmieniæ, ¿eby by³ points_in_use

    // lista punktów otoczki
    list<CircleShape*> points_in_ConvexShell;

    if (list_of_points.size() < 3) return points_in_ConvexShell;


    // lista z której bêdziemy usuwaæ wykorzystane punkty, ¿eby niepotrzebnie nie sprawdzaæ
    list<CircleShape*> points_in_use;
    points_in_use.clear();  for (auto& i : list_of_points) points_in_use.push_back(i);


    // pierwszy punkt
    CircleShape* startpoint_bottom = bottom_point(list_of_points);
    points_in_ConvexShell.push_back(startpoint_bottom);

    LOG("list of points " << list_of_points.size());
    LOG("points_in_use " << points_in_use.size());
    LOG("points_in_ConvexShell " << points_in_ConvexShell.size());

    // zatrzyma siê jak domkniemy otoczke
    while ((points_in_ConvexShell.size() == 1 ||
        ((points_in_ConvexShell.front()) != (points_in_ConvexShell.back()))))
    {
        // dla pierwszego punktu musi byæ prosta równoleg³a do osi x
        // 
        // z points_in_use usuwam wszystkie u¿yte oprócz pierwszego

        // bierze ostatni dodany element i porównuje k¹ty do wszytkich innych

        list<CircleShape*>::iterator _end = points_in_ConvexShell.end();

        // pointery
        CircleShape* last_to_reference = nullptr;
        CircleShape* befor_last_to_reference = nullptr;

        if (points_in_ConvexShell.size() == 1)
        {
            list<CircleShape*>::iterator last = std::prev(_end);
            last_to_reference = *last;
        }
        else
        {
            list<CircleShape*>::iterator last = std::prev(_end);
            list<CircleShape*>::iterator before_last = std::prev(std::prev(_end));

            befor_last_to_reference = *before_last;
            last_to_reference = *last;
        }

        Angle_and_Point biggest_angle_to;
        for (auto& p : points_in_use)
        {
            if (befor_last_to_reference != p && last_to_reference != p)
            {
                //Angle_and_Point tmp(point_of_reference, p);

                // Dawaæ tutaj
                Angle_and_Point tmp(befor_last_to_reference, last_to_reference, p);

                if (p == points_in_use.front() || tmp.angle > biggest_angle_to.angle)
                    biggest_angle_to = tmp;
            }
        }

        // dodajemy punkt do otoczki
        points_in_ConvexShell.push_back(biggest_angle_to.measuring_point);

        // usuwamy z listy points_in_use
        points_in_use.remove(biggest_angle_to.measuring_point);
    }

    for (auto& con : points_in_ConvexShell)
        con->setFillColor(Color::White);

    return points_in_ConvexShell;
}