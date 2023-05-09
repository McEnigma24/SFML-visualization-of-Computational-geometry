#pragma once
#include "header.h"

/////////////////////////////////////////////////////////////////
Vector2f Operations::Middle_of_a_circle_no_yREC(CircleShape circle)
{
    float rad = circle.getRadius();
    Vector2f tmp = circle.getPosition();
    tmp.x += rad;
    tmp.y += rad;

    return tmp;
}
Vector2f Operations::Middle_of_a_circle_no_yREC(CircleShape* circle)
{
    float rad = circle->getRadius();
    Vector2f tmp = circle->getPosition();
    tmp.x += rad;
    tmp.y += rad;

    return tmp;
}
Vector2f Operations::Middle_of_a_circle_no_yREC(const CircleShape* circle)
{
    float rad = circle->getRadius();
    Vector2f tmp = circle->getPosition();
    tmp.x += rad;
    tmp.y += rad;

    return tmp;
}

Vector2f Operations::Middle_of_a_circle_no_yREC(Vector2f positon, float rad)
{
    Vector2f tmp;

    tmp.x += rad;
    tmp.y += rad;

    return tmp;
}
Vector2f Operations::Position_to_draw_middle_of_circle(Vector2f&& point, float rad)
{
    Vector2f tmp = point;
    tmp.x -= rad;
    tmp.y -= rad;

    return tmp;
}
Vector2f Operations::Position_to_draw_middle_of_circle(Vector2f& point, float rad)
{
    Vector2f tmp = point;
    tmp.x -= rad;
    tmp.y -= rad;

    return tmp;
}
void Operations::Position_to_draw_middle_of_circle_void(Vector2f& point, float rad)
{
    point.x -= rad;
    point.y -= rad;
}
/////////////////////////////////////////////////////////////////


    Operations::Old_file_operations::Old_file_operations(PrimitiveType t)
    {
        name = "";
        type = t;
        first = true;
    }
    Operations::Old_file_operations::Old_file_operations(string n, PrimitiveType t)
    {
        name = n;
        type = t;
        first = true;
    }
    void Operations::Old_file_operations::Change_PrimitiveType(PrimitiveType t)
    {
        type = t;
    }

    // zczytywanie z pliku
    Vector2f* Operations::Old_file_operations::get_points_from_file(int& size, string file_name)
    {
        const char* input = file_name.c_str();
        int how_many_lines = 0;

        // odczyt z pliku sprawdza ile lini
        {
            ifstream ODCZYT(input);

            string line;
            while (!ODCZYT.eof())
            {
                getline(ODCZYT, line);
                how_many_lines++;
            }
            ODCZYT.close();
            size = how_many_lines;
            //cout << size << endl;
        }

        if (how_many_lines > 0)
        {
            Vector2f* tab = new Vector2f[how_many_lines];
            //for (int i = 0; i < how_many_lines; i++)
            /*{
                Vector2f tmp;
                tab[i] = tmp;
            }*/

            ifstream ONE_MORE_TIME(input);
            float x, y;

            int i = 0;
            if (first) if (name != "") cout << endl << name << endl;
            while (!ONE_MORE_TIME.eof())
            {
                ONE_MORE_TIME >> x;
                ONE_MORE_TIME >> y;
                Vector2f obj(WIDTH / 2 + x, HEIGHT / 2 + y);

                if (first) cout << i << ": " << x << " " << y << endl;


                if (i >= 0 && i < how_many_lines) tab[i] = obj;
                i++;
            }
            ONE_MORE_TIME.close();
            first = false;

            return tab;
        }

        return nullptr;
    }

    // wpisuje wartoœci wektorów do VertexArray    
    VertexArray Operations::Old_file_operations::construct_lines_based_on_input(Vector2f* tab, int size)
    {
        // just to be safe
        {
            if (!(size > 0))
            {
                VertexArray lines(type, 0);
                return lines;
            }
        }

        VertexArray lines(type, size);
        for (int i = 0; i < size; i++)
        {
            lines[i].position = tab[i];
        }

        return lines;
    }

    // otoczka zczytania z pliku, tworzy gotowy do wyœwietlenia VertexArray
    VertexArray Operations::Old_file_operations::construct_lines_from_file(string file_name)
    {
        int size = 0;
        Vector2f* tab = get_points_from_file(size, file_name);

        VertexArray result = construct_lines_based_on_input(tab, size);
        delete[] tab;

        return result;
    }
    VertexArray Operations::Old_file_operations::construct_lines_at_random(int how_many)
    {
        // robimy plik roboczy tmp.txt z przypadkowymi wspó³rzêdnymi, a nastêpnie zczytujemy z niego wczeœniejsz¹ funkcj¹
        int x_random = 0;
        int y_random = 0;

        string file_name = "tmp.txt";
        const char* path = file_name.c_str();
        ofstream TMP(path);

        for (int i = 0; i < how_many; i++)
        {
            x_random = rand() % WIDTH - WIDTH / 2;
            y_random = rand() % HEIGHT - HEIGHT / 2;

            TMP << x_random << " ";
            TMP << y_random << endl;
        }
        TMP.close();

        int size = 0;
        Vector2f* tab = get_points_from_file(size, file_name);

        VertexArray result = construct_lines_based_on_input(tab, size);
        delete[] tab;

        remove(path);
        return result;
    }

    void Operations::Old_file_operations::save_points_from_file_to_list(list<VertexArray>& list)
    {


    }


    // mo¿e znajdê use
    void Operations::Passing_function(bool& pass, void (*ptr)())
    {
        if (!pass)
        {
            pass = true;
            ptr();
            SLEEP(100);
        }
        else
        {
            pass = false;
            SLEEP(100);
        }

    }



// Essentials //
void Operations::All_Points_Green(list<CircleShape*>& l)
{
    for (list<CircleShape*>::iterator it = l.begin(); it != l.end(); it++)
        (*it)->setFillColor(Color::Green);
}

float Operations::Get_length(float x1, float y1, float x2, float y2)
{
    float a = x1 - x2;
    float b = y1 - y2;

    return sqrt(a * a + b * b);
}
float Operations::Get_length(Vector2f firts, Vector2f second)
{
    float x1 = firts.x; float y1 = firts.y;
    float x2 = second.x; float y2 = second.y;

    float a = x1 - x2;
    float b = y1 - y2;

    return sqrt(a * a + b * b);
}
float Operations::Get_length(CircleShape* c, CircleShape* ca)
{
    Vector2f first = c->getPosition();
    Vector2f second = ca->getPosition();

    float x1 = first.x; float y1 = first.y;
    float x2 = second.x; float y2 = second.y;

    float a = x1 - x2;
    float b = y1 - y2;

    return sqrt(a * a + b * b);
}

Vector2f Operations::Middle_point(CircleShape* distance_circle, CircleShape* ca)
{
    Vector2f distance_c = Middle_of_a_circle_no_yREC(distance_circle);
    Vector2f other = Middle_of_a_circle_no_yREC(ca);

    float a = other.x - distance_c.x;
    float b = other.y - distance_c.y;

    return Vector2f(distance_c.x + a / 2, distance_c.y + b / 2);
}
float Operations::a_factor_Line(CircleShape* c_1, CircleShape* c_2)
{
    Vector2f v_1 = Middle_of_a_circle_no_yREC(c_1);
    Vector2f v_2 = Middle_of_a_circle_no_yREC(c_2);

    float distance_x = v_1.x - v_2.x;
    float distance_y = v_1.y - v_2.y;

    return distance_y / distance_x;
}

void Operations::Print_Vector(Vector2f vec) { cout << "x: " << vec.x << " y: " << vec.y << endl; }




/// VERTEX ARRAY

///////////////////////////////////////////////////////////////// 
// Vertex Array --- check if point is in the right position
VertexArray Operations::Cross_with_lines(Vector2f middle)
{
    VertexArray lines(sf::LinesStrip, 8);

    // do prawego
    lines[0] = middle;
    lines[1] = Vector2f(WIDTH, middle.y);

    // do lewego
    lines[4] = middle;
    lines[5] = Vector2f(0, middle.y);

    // do góry
    lines[2] = middle;
    lines[3] = Vector2f(middle.x, 0);

    // w dó³
    lines[6] = middle;
    lines[7] = Vector2f(middle.x, HEIGHT);

    for (int i = 0; i < 8; i++) lines[i].color = Color::Red;

    return lines;
}
VertexArray Operations::Cross_with_lines___with_limits(Vector2f middle, int up_y_limit, int down_y_limit,
    int left_x_limit, int right_x_limit)
{
    VertexArray lines(sf::LinesStrip, 8);

    int smap_needed_1 = down_y_limit;
    int smap_needed_2 = right_x_limit;


    // do lewego
    lines[4] = middle;
    lines[5] = Vector2f(left_x_limit, middle.y);

    // do prawego ///////////////////////////////
    lines[0] = middle;
    lines[1] = Vector2f(smap_needed_1, middle.y);

    // do góry
    lines[2] = middle;
    lines[3] = Vector2f(middle.x, up_y_limit);

    // w dó³ /////////////////////////////////////
    lines[6] = middle;
    lines[7] = Vector2f(middle.x, smap_needed_2);

    for (int i = 0; i < 8; i++) lines[i].color = Color::Red;

    return lines;
}

// Wczeœniejszy offset
/*
VertexArray Cross_with_lines___with_limits(Limits lim, int offset)
{
    VertexArray lines(sf::LinesStrip, 8);
    Vector2f middle = lim.Get_middle_position_offset(offset, offset);

    // do lewego
    lines[4] = middle;
    lines[5] = lim.Middle_point_of_side_Left(offset, offset);

    // do prawego ///////////////////////////////
    lines[0] = middle;
    lines[1] = lim.Middle_point_of_side_Right(offset, offset);

    // do góry
    lines[2] = middle;
    lines[3] = lim.Middle_point_of_side_Up(offset, offset);

    // w dó³ /////////////////////////////////////
    lines[6] = middle;
    lines[7] = lim.Middle_point_of_side_Down(offset, offset);

    for (int i = 0; i < 8; i++) lines[i].color = Color::Red;

    return lines;
}
*/

VertexArray Operations::Cross_with_lines___with_limits(Limits lim, int x_offset, int y_offset)
{
    VertexArray lines(sf::LinesStrip, 8);
    Vector2f middle = lim.Get_middle_position_offset(x_offset, y_offset);

    // do lewego
    lines[4] = middle;
    lines[5] = lim.Middle_point_of_side_Left(x_offset, y_offset);

    // do prawego ///////////////////////////////
    lines[0] = middle;
    lines[1] = lim.Middle_point_of_side_Right(x_offset, y_offset);

    // do góry
    lines[2] = middle;
    lines[3] = lim.Middle_point_of_side_Up(x_offset, y_offset);

    // w dó³ /////////////////////////////////////
    lines[6] = middle;
    lines[7] = lim.Middle_point_of_side_Down(x_offset, y_offset);

    for (int i = 0; i < 8; i++) lines[i].color = Color::Red;

    return lines;
}


VertexArray Operations::Draw_a_rectangle(Vector2f* tab, Color c)
{
    VertexArray lines(sf::LinesStrip, 5);
    lines[0] = tab[0]; lines[0].color = c;
    lines[1] = tab[1]; lines[1].color = c;
    lines[2] = tab[2]; lines[2].color = c;
    lines[3] = tab[3]; lines[3].color = c;
    lines[4] = tab[0]; lines[4].color = c;

    return lines;
}


/////////////////////////////////////////////////////////////////



// For Mesh //
Vector2f Operations::Mesh_left(list<CircleShape*>& circle_list)
{
    CircleShape* left = nullptr;
    for (list<CircleShape*>::iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) left = (*it);
        else
        {
            if ((*it)->getPosition().x < left->getPosition().x)
            {
                left = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(left);
}
Vector2f Operations::Mesh_right(list<CircleShape*>& circle_list)
{
    CircleShape* right = nullptr;
    for (list<CircleShape*>::iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) right = (*it);
        else
        {
            if ((*it)->getPosition().x > right->getPosition().x)
            {
                right = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(right);
}
Vector2f Operations::Mesh_up(list<CircleShape*>& circle_list)
{
    CircleShape* up = nullptr;
    for (list<CircleShape*>::iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) up = (*it);
        else
        {
            if ((*it)->getPosition().y < up->getPosition().y)
            {
                up = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(up);
}
Vector2f Operations::Mesh_down(list<CircleShape*>& circle_list)
{
    CircleShape* down = nullptr;
    for (list<CircleShape*>::iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) down = (*it);
        else
        {
            if ((*it)->getPosition().y > down->getPosition().y)
            {
                down = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(down);
}
Vector2f Operations::Mesh_left(const list<CircleShape*>& circle_list)
{
    CircleShape* left = nullptr;
    for (list<CircleShape*>::const_iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) left = (*it);
        else
        {
            if ((*it)->getPosition().x < left->getPosition().x)
            {
                left = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(left);
}
Vector2f Operations::Mesh_right(const list<CircleShape*>& circle_list)
{
    CircleShape* right = nullptr;
    for (list<CircleShape*>::const_iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) right = (*it);
        else
        {
            if ((*it)->getPosition().x > right->getPosition().x)
            {
                right = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(right);
}
Vector2f Operations::Mesh_up(const list<CircleShape*>& circle_list)
{
    CircleShape* up = nullptr;
    for (list<CircleShape*>::const_iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) up = (*it);
        else
        {
            if ((*it)->getPosition().y < up->getPosition().y)
            {
                up = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(up);
}
Vector2f Operations::Mesh_down(const list<CircleShape*>& circle_list)
{
    CircleShape* down = nullptr;
    for (list<CircleShape*>::const_iterator it = circle_list.begin(); it != circle_list.end(); it++)
    {
        if (it == circle_list.begin()) down = (*it);
        else
        {
            if ((*it)->getPosition().y > down->getPosition().y)
            {
                down = (*it);
            }
        }
    }

    return Middle_of_a_circle_no_yREC(down);
}

Vector2f Operations::Mesh_left_corner(list<CircleShape*>& circle_list)
{
    Vector2f left = Mesh_left(circle_list);
    Vector2f down = Mesh_down(circle_list);

    return Vector2f(left.x, down.y);
}
Vector2f Operations::Mesh_right_corner(list<CircleShape*>& circle_list)
{
    Vector2f right = Mesh_right(circle_list);
    Vector2f up = Mesh_up(circle_list);

    return Vector2f(right.x, up.y);
}
Vector2f Operations::Mesh_left_corner(const list<CircleShape*>& circle_list)
{
    Vector2f left = Mesh_left(circle_list);
    Vector2f down = Mesh_down(circle_list);

    return Vector2f(left.x, down.y);
}
Vector2f Operations::Mesh_right_corner(const list<CircleShape*>& circle_list)
{
    Vector2f right = Mesh_right(circle_list);
    Vector2f up = Mesh_up(circle_list);

    return Vector2f(right.x, up.y);
}


// For Triangulation //
Vector2f Operations::Down_left(list<CircleShape*>& circle_list)
{
    Vector2f left = Mesh_left(circle_list);
    Vector2f down = Mesh_down(circle_list);

    return Vector2f(left.x, down.y);
}
Vector2f Operations::Down_right(list<CircleShape*>& circle_list)
{
    Vector2f right = Mesh_right(circle_list);
    Vector2f down = Mesh_down(circle_list);

    return Vector2f(right.x, down.y);
}