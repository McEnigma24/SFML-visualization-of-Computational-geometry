#pragma once
#include "Header.h"

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

// using HASH FUNCTION Line - newTriangles();
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
                    (hash<int>()(k.intPoint2.y) ^ 4));
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
bool Triangulation::checkIfAllPointsAreInTriangle(Vector2f t_left, Vector2f t_right, Vector2f t_up, vector<myCircle>& lpoints)
{
    Vector2f tab[3];   tab[0] = t_left; tab[1] = t_right; tab[2] = t_up;

    for (vector<myCircle>::iterator it = lpoints.begin(); it != lpoints.end(); it++)
    {
        if (!checkIfPointIsInTriangle(tab, (*it).getMiddle())) return false;
    }

    return true;
}


// Super Triangle
Triangle Triangulation::createSuperTriangle(vector<myCircle>& lpoints)
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

    res.x = ( (((t.A.x * static_cast<float>(sin(2 * t.Aangle)))) + ((t.B.x * static_cast<float>(sin(2 * t.Bangle)))) + ((t.C.x * static_cast<float>(sin(2 * t.Cangle)))))
        /    (static_cast<float>(sin(2 * t.Aangle)) + static_cast<float>(sin(2 * t.Bangle)) + static_cast<float>(sin(2 * t.Cangle)) ));

    res.y = ((((t.A.y * static_cast<float>(sin(2 * t.Aangle)))) + ((t.B.y * static_cast<float>(sin(2 * t.Bangle)))) + ((t.C.y * static_cast<float>(sin(2 * t.Cangle)))))
        / (static_cast<float>(sin(2 * t.Aangle)) + static_cast<float>(sin(2 * t.Bangle)) + static_cast<float>(sin(2 * t.Cangle))));

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
void Triangulation::newTriangles(vector<myCircle>& lpoints, list<Triangle>& originalTriangles, list<Triangle>& badTriangles, Vector2f newPoint)
{
    // Rozbicie badTriangles na linie i wyrzucenie tych które siê nie powtarzaj¹   

    // Podejœcie z map¹
    #if howToCheckFrequency == 0
        unordered_map<Line, int> map;
        for (auto& g : badTriangles)
        {
            // tutaj znajduje myCircle które ma takie same wspó³rzêdne i w ten sposób znajduje w³aœciwe id

            Line l1(g.A, g.B);       l1.id_a = g.id_A;      l1.id_b = g.id_B;
            Line l2(g.A, g.C);       l2.id_a = g.id_A;      l2.id_b = g.id_C;
            Line l3(g.B, g.C);       l3.id_a = g.id_B;      l3.id_b = g.id_C;

            if (map.find(l1) == map.end())            map[l1] = 0;
            if (map.find(l2) == map.end())            map[l2] = 0;
            if (map.find(l3) == map.end())            map[l3] = 0;

            map[l1]++;
            map[l2]++;
            map[l3]++;
        }
    
        
        // teraz z niepowtarzaj¹cych siê lini tworzymy trójk¹ty i dodajemy do originalTriangles
        for (auto& m : map)
            if (m.second == 1 && m.first.point1 != m.first.point2)
            {                
                Triangle t(m.first.point1, m.first.point2, newPoint);

                t.id_A = getId(lpoints, m.first.point1);
                t.id_B = getId(lpoints, m.first.point2);
                t.id_C = getId(lpoints, newPoint);

                originalTriangles.push_back(t);
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

// Finding correct id for point based on coordinets
int Triangulation::getId(vector<myCircle>& lpoints, Vector2f Coordinets)
{    
    for (auto& point : lpoints)
        if (point.getMiddle() == Coordinets)
            return point.m_id;

    return -1;
}

// Deleting triangles with connections to Super Triangle
void Triangulation::removeTrianglesCommonTo(list<Triangle>& originalTriangles, Triangle superT)
{
    list<Triangle> res;

    for (auto& o : originalTriangles)
        if (!o.anyCommnoPointWith(superT)) res.push_back(o);

    originalTriangles = res;
}


// Delaunay Triangulation //
list<Triangle> Triangulation::triangulationDelaunay(vector<myCircle>& lpoints)
{
    BEN(ScopeTimer main_t("triangulation", ben::trinagulation, 1);)
    INFO(ScopeInfo info("Triangles Position"))

    list<Triangle> original_Triangles;      list<Triangle> bad_Triangles;

    Triangle Super_Triangle;
    {
        //BEN(ScopeTimer t("super triangle", ben::trinagulation);)
            Super_Triangle = createSuperTriangle(lpoints);
            Super_Triangle.id_A = -1;
            Super_Triangle.id_B = -2;
            Super_Triangle.id_C = -3;
        original_Triangles.push_back(Super_Triangle);
    }

    {
        //BEN(ScopeTimer t("main operations", ben::trinagulation);)

        Vector2f points_position;
        // g³ówne obroty przez wszystkie punkty
        for (auto& point : lpoints)
        {
            points_position = point.getMiddle();

            // loop through original_triangles
            // Identifying bad Triangles from original list                        
            getBadTriangles(original_Triangles, bad_Triangles, points_position);

            // getting new Triangles from bad Triangles            
            newTriangles(lpoints, original_Triangles, bad_Triangles, points_position);
        }

        // usuwanie tych co maj¹ choæ jedno po³¹czenie z super triangle

        // usuwamy super triangle
        original_Triangles.erase(original_Triangles.begin());
        removeTrianglesCommonTo(original_Triangles, Super_Triangle);
    }

    INFO(info.info_triangle = original_Triangles)
    return original_Triangles;
}

// Save to File //
void Triangulation::saveToFile(string p, const list<Triangle>& listTriangle, const vector<myCircle> vecPoints, QuadTree& q)
{
    const char* path = p.c_str();

    ofstream FILE(path);
    const Triangle last = listTriangle.back();
    vector<myCircle> vecPoints_no_const = vecPoints;

    myVector position(false);       Vector2f v;
    FILE << "nodes" << endl;
    for (auto& point : vecPoints)
    {
        position.afterConstructor(point.getMiddle(), true);
        v = *position;

        FILE << point.m_id << " - ";
        FILE << v.x << " ";
        FILE << v.y << endl;
    }


    int id = 1;
    int y_coord; int x_coord;   Vector2f incircle;

    int quad_x_limit{};     int quad_y_limit{};
    int** tab = q.getTab(quad_y_limit, quad_x_limit);

    // x = 1305     // y = 730
    //cout << "x limit: " << quad_x_limit << endl;
    //cout << "y limit: " << quad_y_limit << endl;

    
    FILE << endl << "elements" << endl;
    for (auto& triangle : listTriangle)
    {
        FILE << id << " - ";  id++;
        FILE << getId(vecPoints_no_const, triangle.A) << " " << getId(vecPoints_no_const, triangle.B)
            << " " << getId(vecPoints_no_const, triangle.C);

        incircle = triangle.getPositionForNumber();

        y_coord = static_cast<int>(incircle.y - q.getOffSet().y_offset);
        x_coord = static_cast<int>(incircle.x - q.getOffSet().x_offset);

        if (y_coord < quad_y_limit && x_coord < quad_x_limit)
        {
            //FILE << "y " << y_coord << " x " << x_coord << " --- " << tab[y_coord][x_coord] << "  ";

            if (tab[y_coord][x_coord] == 0)   FILE << " > osnowa";
            else                              FILE << " > wydzielenie";
        }

        if(triangle != last) FILE << endl;
    }

    FILE.close();
}