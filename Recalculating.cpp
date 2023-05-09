#pragma once
#include "header.h"

// Recalculating Lines
void Recalculating::recalculateLinesVertexArrayBasedOnExistingCircleList(const list<CircleShape*>& circle_list, VertexArray* Vertex_tab, PrimitiveType type, bool connecting)
{
    // albo tak jak z tym pointerem, zmieniæ wspó³rzêdne coordów --- za du¿o szukania a i tak trzeba zrobiæ nowy obiekt VertexArray

    int size = distance(circle_list.begin(), circle_list.end()); if (size < 1) return;
    if (connecting) size++;
    VertexArray recalculated_points(type, size);
    Vector2f tmp;
    int i = 0;
    float radious = 0;

    for (list<CircleShape*>::const_iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        radious = (*it)->getRadius();
        tmp = (*it)->getPosition();
        tmp.x = tmp.x + radious;
        tmp.y = tmp.y + radious;

        recalculated_points[i] = tmp;
        i++;
    }
    if (connecting)
    {
        list<CircleShape*>::const_iterator it = (circle_list.begin());
        radious = (*it)->getRadius();
        tmp = (*it)->getPosition();
        tmp.x = tmp.x + radious;
        tmp.y = tmp.y + radious;

        recalculated_points[size - 1] = tmp;
    }

    Vertex_tab[0] = recalculated_points;
}
// Recalculating mesh
void Recalculating::recalculateMesh(list<VertexArray>& list_mesh_blocks, list<CircleShape*> list_circle_points, int x_slices, int y_slices)
{
    int list_size = distance(list_circle_points.begin(), list_circle_points.end()); if (list_size == 0) return;
    list_mesh_blocks.clear();

    Mesh m;
    m.Save_points_to_file_blocks(list_circle_points, x_slices, y_slices);
    m.Visualize_Mesh(list_mesh_blocks);
}
// Recalculating lines
void Recalculating::recalculateSetVertexArrayForDistanceCircleLines(const list<CircleShape*>& l, VertexArray* Vertexarray, const CircleShape* distance_circle)
{
    int list_size = distance(l.begin(), l.end()); if (list_size == 0) return;

    VertexArray lines(sf::LinesStrip, list_size * 2);
    // *2 bo ka¿dy z punktów musi za ka¿dym razem
    // wracaæ do œrodkowego i do tego innego
    int i = 0;
    for (list<CircleShape*>::const_iterator it = l.begin(); it != l.end(); it++)
    {
        lines[i] = Middle_of_a_circle_no_yREC(distance_circle);
        lines[i + 1] = Middle_of_a_circle_no_yREC(*it);

        lines[i].color = Color::Yellow;
        lines[i + 1].color = Color::Blue;

        i += 2;
    }

    Vertexarray[1] = lines;
}
// Recalculating text for distances
void Recalculating::recalculateSetTextDistanceCircle(list<Text>& Ltext, const list<CircleShape*>& Lcircles, CircleShape* p_distance_calculation_circle, const Font& f)
{
    int list_size = distance(Lcircles.begin(), Lcircles.end()); if (list_size == 0) return;

    Ltext.clear();
    int i = 1;
    for (list<CircleShape*>::const_iterator it = Lcircles.begin(); it != Lcircles.end(); it++)
    {
        Text text;
        text.setFont(f);

        float angle = atan(a_factor_Line(p_distance_calculation_circle, *it));
        angle = angle * (360 / (PI * 2));
        //if (angle < 0) angle *= (-1);            
        //while (angle > 360) angle -= 360;
        //cout << i << " " << angle << endl; i++;            

        text.setPosition(Middle_point(p_distance_calculation_circle, *it));
        text.setRotation(angle);

        string input = to_string((int)Get_length(p_distance_calculation_circle, *it));
        text.setString(input);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);

        Ltext.push_back(text);
    }
}
// Recalculating closect neightbour brout force
void Recalculating::recalculateClosestNeightbour(list<CircleShape*> l, CircleShape* p_distance_calculation_circle)
{
    if (p_distance_calculation_circle == nullptr) return;
    int list_size = distance(l.begin(), l.end()); if (list_size == 0) return;

    // wszystkie kó³ka na zielono
    for (list<CircleShape*>::iterator it = l.begin(); it != l.end(); it++)
        (*it)->setFillColor(Color::Green);

    float length = 0;
    float tmp = 0;
    CircleShape* closest = nullptr;
    for (list<CircleShape*>::iterator it = l.begin(); it != l.end(); it++)
    {
        tmp = Get_length(*it, p_distance_calculation_circle);
        if (it == l.begin() || length > tmp)
        {
            length = tmp;
            closest = *it;
        }
    }

    closest->setFillColor(Color::Magenta);
}
// Recalculating convex shell
void Recalculating::recalculateConvexShell(const list<CircleShape*>& list_of_points, VertexArray* Vertexarray)
{
    if (list_of_points.size() < 3) return;

    ConvexShell con;
    list<CircleShape*> list_of_points_in_Convex_Shell = con.Alg_Graham(list_of_points);

    int list_size = distance(list_of_points_in_Convex_Shell.begin(), list_of_points_in_Convex_Shell.end());
    Vector2f first = getMiddle(list_of_points_in_Convex_Shell.front());



    VertexArray lines(sf::LinesStrip, list_size + 1);
    int i = 0;

    for (auto& l : list_of_points_in_Convex_Shell)
    {
        lines[i] = getMiddle(l);
        lines[i].color = Color::White;

        i++;
    }
    lines[list_size] = first;
    lines[list_size].color = Color::White;

    Vertexarray[2] = lines;
}
// Recalculating circle for 3 points
void Recalculating::recalculate3pointsCircle(const list<CircleShape*>& list_of_points, CircleShape** circum_circle, Color c, Triangle** t_lines)
{
    list<CircleShape*>::const_iterator it = list_of_points.begin();

    advance(it, 0); CircleShape first = (**it);
    advance(it, 1); CircleShape second = (**it);
    advance(it, 1); CircleShape third = (**it);

    Triangle t(first, second, third);    
    if (t.areAllPointsInLine())
    {
        if (*t_lines != nullptr) delete *t_lines;
        *t_lines = new Triangle(first, second, third);

        if (*circum_circle != nullptr) delete *circum_circle; 
        *circum_circle = nullptr;        

        return;
    }

        
    Triangulation trian; float rad{};
    Vector2f circle_position = trian.sinMiddleOfCircle(t, rad);

    CircleShape* tmp = new CircleShape();
    tmp->setFillColor(c);
    tmp->setRadius(rad);
    tmp->setPosition(circle_position);
    tmp->setPosition(setMiddle(tmp));

    if (*circum_circle != nullptr) delete *circum_circle;
    *circum_circle = tmp;

    if (*t_lines != nullptr) delete * t_lines;
    *t_lines = nullptr;
}
// Recalculating Triangulation
void Recalculating::recalculateTriangulation(list<CircleShape*>& list_of_points, list<Triangle>& list_triangulaton_triangles)
{
    list_triangulaton_triangles.clear();
    Triangulation trian;
    list_triangulaton_triangles = trian.triangulationDelaunay(list_of_points);
}