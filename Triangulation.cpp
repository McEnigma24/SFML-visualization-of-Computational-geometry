#pragma once
#include "header.h"

#define howToCheckFrequency 0
// 0 ---> map WORKING WELL
// 1 ---> list 

#if howToCheckFrequency == 1
    static bool isLinePresentInList(list<Line>& lineList, Line& l)
{
    for (auto& obj : lineList)
        if (obj == l) return true;

    return false;
}
#endif

// hashing function
namespace std {

    template <>
    struct hash<Line>
    {
        std::size_t operator()(const Line& k) const
        {   
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:

            return  ((hash<int>()(k.intPoint1.x) ^ 1) + 
                     (hash<int>()(k.intPoint1.y) ^ 2) +
                     (hash<int>()(k.intPoint2.x) ^ 3) +
                     (hash<int>()(k.intPoint2.y) ^ 4) );
        }
    };

}

// Triangle check
bool Triangulation::checkIfPointIsInTriangle(Vector2f* triangle_points, Vector2f point)
{
    double angle;
    double point_angle = 0;

    // manualne sprawdzenie wszystkich k¹tów
    
    // 1
    {
        angle = l.Angle_between(triangle_points[0], triangle_points[1], triangle_points[2]);

        point_angle = l.Angle_between(triangle_points[0], triangle_points[1], point);
        if (!(0 <= point_angle && point_angle <= angle)) return false;

        point_angle = l.Angle_between(triangle_points[0], triangle_points[2], point);
        if (!(0 <= point_angle && point_angle <= angle)) return false;
    }

    // 2
    {
        angle = l.Angle_between(triangle_points[1], triangle_points[0], triangle_points[2]);

        point_angle = l.Angle_between(triangle_points[1], triangle_points[0], point);
        if (!(0 <= point_angle && point_angle <= angle)) return false;

        point_angle = l.Angle_between(triangle_points[1], triangle_points[2], point);
        if (!(0 <= point_angle && point_angle <= angle)) return false;
    }

    // 3
    {
        angle = l.Angle_between(triangle_points[2], triangle_points[0], triangle_points[1]);

        point_angle = l.Angle_between(triangle_points[2], triangle_points[0], point);
        if (!(0 <= point_angle && point_angle <= angle)) return false;

        point_angle = l.Angle_between(triangle_points[2], triangle_points[1], point);
        if (!(0 <= point_angle && point_angle <= angle)) return false;
    }

    return true;
}
bool Triangulation::checkIfAllPointsAreInTriangle(Vector2f t_left, Vector2f t_right, Vector2f t_up, list<CircleShape*>& lpoints)
{
    Vector2f tab[3];   tab[0] = t_left; tab[1] = t_right; tab[2] = t_up;

    for (list<CircleShape*>::iterator it = lpoints.begin(); it != lpoints.end(); it++)
    {
        if (!checkIfPointIsInTriangle(tab, getMiddle(*it))) return false;
    }

    return true;
}


// Super Triangle
Triangle Triangulation::createSuperTriangle(list<CircleShape*>& lpoints)
{
    Triangle res;
        
    myVector up(Mesh_up(lpoints), false);       up.y += 100;
    myVector left(Down_left(lpoints), false);   left.y -= 100; left.x -= 100;
    myVector right(Down_right(lpoints), false); right.y -= 100; right.x += 100;

    while (!checkIfAllPointsAreInTriangle(*left, *right, *up, lpoints))
    {
        up.y += 100;
        left.y -= 100; left.x -= 100;
        right.y -= 100; right.x += 100;
    }

    res.A = *up;
    res.B = *left;
    res.C = *right;

    return res;
}

// Circium Circle
Vector2f Triangulation::sinMiddleOfCircle(Triangle t)
{
    myVector res(false);

    t.getAngles();

    res.x = ( (((t.A.x * sin(2 * t.Aangle))) + ((t.B.x * sin(2 * t.Bangle))) + ((t.C.x * sin(2 * t.Cangle))))
        /    ( sin(2 * t.Aangle) + sin(2 * t.Bangle) + sin(2 * t.Cangle) ));

    res.y = ((((t.A.y * sin(2 * t.Aangle))) + ((t.B.y * sin(2 * t.Bangle))) + ((t.C.y * sin(2 * t.Cangle))))
        / (sin(2 * t.Aangle) + sin(2 * t.Bangle) + sin(2 * t.Cangle)));

    return *res;
}
Vector2f Triangulation::sinMiddleOfCircle(Triangle t, float& rad)
{
    Vector2f res = sinMiddleOfCircle(t);

    rad = Get_length(res, t.A);

    return res;
}

// Identifying bad Triangles
void Triangulation::getBadTriangles(list<Triangle>& originalTriangles, list<Triangle>& badTriangles, Vector2f newPoint)
{
    float rad = 0;      float distance = 0;     Vector2f circum_circle;

    list<Triangle> after_eliminating_bad_ones;
    for (auto& o : originalTriangles)
    {
        circum_circle = sinMiddleOfCircle(o, rad);
        distance = Get_length(circum_circle, newPoint);

        if (distance < rad)
            badTriangles.push_back(o);
        else
            after_eliminating_bad_ones.push_back(o);
    }

    originalTriangles = after_eliminating_bad_ones;
}

// Creating new Triangles from badTriangles
void Triangulation::newTriangles(list<Triangle>& originalTriangles, list<Triangle>& badTriangles, Vector2f newPoint)
{
    // Rozbicie badTriangles na linie i wyrzucenie tych które siê nie powtarzaj¹
    

    // Podejœcie z map¹
    #if howToCheckFrequency == 0
        unordered_map<Line, int> map;
        for (auto& g : badTriangles)
        {
            Line l1(g.A, g.B);
            Line l2(g.A, g.C);
            Line l3(g.B, g.C);

            if (map.find(l1) == map.end())            map[l1] = 0;
            if (map.find(l2) == map.end())            map[l2] = 0;
            if (map.find(l3) == map.end())            map[l3] = 0;

            map[l1]++;
            map[l2]++;
            map[l3]++;
        }
    
        // teraz z niepowtarzaj¹cych siê lini tworzymy trójk¹ty i dodajemy do originalTriangles
        for (auto& m : map)
            if (m.second == 1)
            {
                //cout << "hittttttttttttt" << endl;
                originalTriangles.emplace_back(m.first.point1, m.first.point2, newPoint);
            }
    #endif   

    // Podejœcie z list¹
    #if howToCheckFrequency == 1
        list<Line> frequency;
        for (auto& g : badTriangles)
        {
            Line l1(g.A, g.B);
            Line l2(g.A, g.C);
            Line l3(g.B, g.C);

            if (!isLinePresentInList(frequency, l1))
            {

                frequency.push_back(l1);
            }
            if (!isLinePresentInList(frequency, l2))
            {

                frequency.push_back(l2);
            }
            if (!isLinePresentInList(frequency, l3))
            {

                frequency.push_back(l3);
            }
        }

        // teraz z niepowtarzaj¹cych siê lini tworzymy trójk¹ty i dodajemy do originalTriangles
        for (auto& f : frequency)
            originalTriangles.emplace_back(f.point1, f.point2, newPoint);        
    #endif
    


    badTriangles.clear();
}

// Deleting triangles with connections to Super Triangle
void Triangulation::removeTrianglesCommonTo(list<Triangle>& originalTriangles, Triangle superT)
{
    list<Triangle> res;

    for (auto& o : originalTriangles)
        if (!o.anyCommnoPointWith(superT)) res.push_back(o);

    originalTriangles = res;
}


// Delaunay Triangulation
list<Triangle> Triangulation::triangulationDelaunay(list<CircleShape*>& lpoints)
{
    BEN(ScopeTimer main_t("whole triangulation", ben::trinagulation, 1);)

    list<Triangle> original_Triangles;      list<Triangle> bad_Triangles;

    Triangle Super_Triangle;
    {
        BEN(ScopeTimer t("super triangle", ben::trinagulation);)
        Super_Triangle = createSuperTriangle(lpoints);
        original_Triangles.push_back(Super_Triangle);
    }

    
    {
        BEN(ScopeTimer t("main operations", ben::trinagulation);)

        Vector2f points_position;
        // g³ówne obroty przez wszystkie punkty
        for (auto& point : lpoints)
        {
            points_position = getMiddle(point);

            // loop through original_triangles
            // Identifying bad Triangles from original list                        
            getBadTriangles(original_Triangles, bad_Triangles, points_position);

            // getting new Triangles from bad Triangles
            newTriangles(original_Triangles, bad_Triangles, points_position);
        }

        // usuwanie tych co maj¹ choæ jedno po³¹czenie z super triangle

        // usuwamy super triangle
        original_Triangles.erase(original_Triangles.begin());
        removeTrianglesCommonTo(original_Triangles, Super_Triangle);
    }


    return original_Triangles;
}