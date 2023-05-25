#pragma once
#include "Header.h"

// Recalculating lines
void Recalculating::recalculateLinesVertexArrayBasedOnExistingCircleList(const vector<myCircle>& circle_list, VertexArray* Vertex_tab, PrimitiveType type, bool connecting)
{
    // albo tak jak z tym pointerem, zmieniæ wspó³rzêdne coordów --- za du¿o szukania a i tak trzeba zrobiæ nowy obiekt VertexArray

    int size = static_cast<int>(distance(circle_list.begin(), circle_list.end())); if (size < 1) return;
    if (connecting) size++;
    VertexArray recalculated_points(type, size);
    Vector2f tmp;
    int i = 0;
    float radious = 0;

    for (vector<myCircle>::const_iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        radious = (*it).m_circle->getRadius();
        tmp = (*it).m_circle->getPosition();
        tmp.x = tmp.x + radious;
        tmp.y = tmp.y + radious;

        recalculated_points[i] = tmp;
        i++;
    }
    if (connecting)
    {
        vector<myCircle>::const_iterator it = (circle_list.begin());
        radious = (*it).m_circle->getRadius();
        tmp = (*it).m_circle->getPosition();
        tmp.x = tmp.x + radious;
        tmp.y = tmp.y + radious;

        recalculated_points[size - 1] = tmp;
    }

    Vertex_tab[0] = recalculated_points;
}
// Recalculating mesh
void Recalculating::recalculateMesh(list<VertexArray>& list_mesh_blocks, vector<myCircle> vec_circle_points, int x_slices, int y_slices)
{
    int list_size = static_cast<int>(distance(vec_circle_points.begin(), vec_circle_points.end())); if (list_size == 0) return;
    list_mesh_blocks.clear();

    Mesh m;
    m.Save_points_to_file_blocks(vec_circle_points, static_cast<float>(x_slices), static_cast<float>(y_slices));
    m.Visualize_Mesh(list_mesh_blocks);
}
// Recalculating distance lines
void Recalculating::recalculateSetVertexArrayForDistanceCircleLines(const vector<myCircle>& l, VertexArray* Vertexarray, const CircleShape* distance_circle)
{
    int list_size = static_cast<int>(distance(l.begin(), l.end())); if (list_size == 0) return;

    VertexArray lines(sf::LinesStrip, list_size * 2);
    // *2 bo ka¿dy z punktów musi za ka¿dym razem
    // wracaæ do œrodkowego i do tego innego
    int i = 0;
    for (auto& it : l)
    {
        lines[i] = Middle_of_a_circle_no_yREC(distance_circle);
        lines[i + 1] = Middle_of_a_circle_no_yREC(*it.m_circle);

        lines[i].color = Color::Yellow;
        lines[i + 1].color = Color::Blue;

        i += 2;
    }

    Vertexarray[1] = lines;
}
// Recalculating text for distances
void Recalculating::recalculateSetTextDistanceCircle(list<Text>& Ltext, const vector<myCircle>& Lcircles, CircleShape* p_distance_calculation_circle, const Font& f)
{
    int list_size = static_cast<int>(distance(Lcircles.begin(), Lcircles.end())); if (list_size == 0) return;

    Ltext.clear();
    int i = 1;
    for (auto& it : Lcircles)
    {
        Text text;
        text.setFont(f);

        float angle = atan(a_factor_Line(p_distance_calculation_circle, &*it.m_circle));
        angle = angle * ((float)360 / (float)((float)PI * 2));
        //if (angle < 0) angle *= (-1);            
        //while (angle > 360) angle -= 360;
        //cout << i << " " << angle << endl; i++;            

        text.setPosition(Middle_point(p_distance_calculation_circle, &*it.m_circle));
        text.setRotation(angle);

        string input = to_string((int)Get_length(p_distance_calculation_circle, &*it.m_circle));
        text.setString(input);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);

        Ltext.push_back(text);
    }
}
// Recalculating closect neightbour brout force
void Recalculating::recalculateClosestNeightbour(vector<myCircle> l, CircleShape* p_distance_calculation_circle)
{
    if (p_distance_calculation_circle == nullptr) return;
    int list_size = static_cast<int>(distance(l.begin(), l.end())); if (list_size == 0) return;

    // wszystkie kó³ka na zielono
    for (vector<myCircle>::iterator it = l.begin(); it != l.end(); it++)
        (*it).m_circle->setFillColor(Color::Green);

    float length = 0;
    float tmp = 0;
    CircleShape* closest = nullptr;
    for (vector<myCircle>::iterator it = l.begin(); it != l.end(); it++)
    {
        tmp = Get_length(&(*it), p_distance_calculation_circle);
        if (it == l.begin() || length > tmp)
        {
            length = tmp;
            closest = &*(*it).m_circle;
        }
    }

    closest->setFillColor(Color::Magenta);
}
// Recalculating convex shell
void Recalculating::recalculateConvexShell(vector<myCircle>& vec_of_points, VertexArray* Vertexarray)
{
    if (vec_of_points.size() < 3) return;

    ConvexShell con;
    vector<myCircle> list_of_points_in_Convex_Shell = con.Alg_Graham(vec_of_points);

    int list_size = static_cast<int>(distance(list_of_points_in_Convex_Shell.begin(), list_of_points_in_Convex_Shell.end()));
    Vector2f first = (list_of_points_in_Convex_Shell.front()).getMiddle();



    VertexArray lines(sf::LinesStrip, list_size + 1);
    int i = 0;

    for (auto& l : list_of_points_in_Convex_Shell)
    {
        lines[i] = l.getMiddle();
        lines[i].color = Color::White;

        i++;
    }
    lines[list_size] = first;
    lines[list_size].color = Color::White;

    Vertexarray[2] = lines;
}
// Recalculating circle for 3 points
void Recalculating::recalculate3pointsCircle(const vector<myCircle>& vec_of_points, CircleShape** circum_circle, Color c, Triangle** t_lines)
{
    vector<myCircle>::const_iterator it = vec_of_points.begin();

    advance(it, 0); CircleShape first = *(*it).m_circle;
    advance(it, 1); CircleShape second = *(*it).m_circle;
    advance(it, 1); CircleShape third = *(*it).m_circle;

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
// Recalculating triangulation
void Recalculating::recalculateTriangulation(vector<myCircle>& vec_of_points, list<Triangle>& list_triangulation_triangles)
{
    list_triangulation_triangles.clear();
    Triangulation trian;
    list<Triangle> res = trian.triangulationDelaunay(vec_of_points);

    int id = 1;
    for (auto& triangle : res)
    {
        triangle.id_A = id;
        id++;
    }

    list_triangulation_triangles = res;
}
// Recalculating triangle numbers

// Recalculating random points positions
void Recalculating::recalculateRandomPointPositions(vector<myCircle>& vec_of_points, float speed)
{    
    Vector2f tmp;
    Vector2f last;
    int random;

    for (auto& point : vec_of_points)
    {
        random = rand() % 4 + 1;
        tmp = point.getMiddle();
        last = tmp;
        
        switch (random)
        {
            // + x // + y //
            case 1:
            {
                tmp.x += speed;
                tmp.y += speed;
                break;
            }

            // + x // - y //
            case 2:
            {
                tmp.x += speed;
                tmp.y -= speed;
                break;
            }

            // - x // + y //
            case 3:
            {
                tmp.x -= speed;
                tmp.y += speed;
                break;
            }

            // - x // - y //
            case 4:
            {
                tmp.x -= speed;
                tmp.y -= speed;
                break;
            }

            default:
                break;
        }
        //LOG(random << "   " << last.x << " " << last.y << " ---> " << tmp.x << " " << tmp.y);


        // coœ bardziej przemyœlane, ¿eby bra³o pod uwagê wszystkie mo¿liwe kierunki
        if (tmp.x > WIDTH) tmp.x = tmp.x / WIDTH;
        if (tmp.y > HEIGHT) tmp.y = tmp.y / HEIGHT;
        
        point.m_circle->setPosition(tmp);
        point.m_circle->setPosition(point.setMiddle());
    }
}