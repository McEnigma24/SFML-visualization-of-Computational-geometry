#include  <SFML/Graphics.hpp>

#include <iostream>
#include <time.h>
#include <cmath>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include <thread>
#include <string>
#include "windows.h"
#include <list>

using namespace std;
using namespace sf;

# define PI          3.141592653589793238462643383279502884

#define WIDTH 1000
#define HEIGHT 1000
#define VERTEX_ARRAY_SIZE 2
#define CIRCLE_RAD 10

#ifdef MY_DEBUG
#define LOG(x) cout << x << endl
#define OPERATION
#else
#define LOG(x)
#endif

#define SLEEP() std::this_thread::sleep_for(std::chrono::milliseconds(125));
#define SLEEP(x) std::this_thread::sleep_for(std::chrono::milliseconds(x));

class Old_file_operations
{
    string name;
    PrimitiveType type;
    bool first;
public:

    Old_file_operations(PrimitiveType t)
    {
        name = "";
        type = t;
        first = true;
    }
    Old_file_operations(string n, PrimitiveType t)
    {
        name = n;
        type = t;
        first = true;
    }
    void Change_PrimitiveType(PrimitiveType t)
    {
        type = t;
    }

    // zczytywanie z pliku
    Vector2f* get_points_from_file(int& size, string file_name)
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
    VertexArray construct_lines_based_on_input(Vector2f* tab, int size)
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
    VertexArray construct_lines_from_file(string file_name)
    {
        int size = 0;
        Vector2f* tab = get_points_from_file(size, file_name);

        VertexArray result = construct_lines_based_on_input(tab, size);
        delete[] tab;

        return result;
    }
    VertexArray construct_lines_at_random(int how_many)
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

    void save_points_from_file_to_list(list<VertexArray>& list)
    {


    }
};

class Operations
{    
public:

    float Get_length(float x1, float y1, float x2, float y2)
    {
        float a = x1 - x2;
        float b = y1 - y2;

        return sqrt(a * a + b * b);
    }
    float Get_length(Vector2f firts, Vector2f second)
    {
        float x1 = firts.x; float y1 = firts.y;
        float x2 = second.x; float y2 = second.y;

        float a = x1 - x2;
        float b = y1 - y2;

        return sqrt(a * a + b * b);
    }
    float Get_length(CircleShape* c, CircleShape* ca)
    {
        Vector2f first = c->getPosition();
        Vector2f second = ca->getPosition();

        float x1 = first.x; float y1 = first.y;
        float x2 = second.x; float y2 = second.y;

        float a = x1 - x2;
        float b = y1 - y2;

        return sqrt(a * a + b * b);
    }

    Vector2f Middle_point(CircleShape* distance_circle, CircleShape* ca)
    {
        Vector2f distance_c = distance_circle->getPosition();
        Vector2f other = ca->getPosition();

        float a = other.x - distance_c.x;
        float b = other.y - distance_c.y;

        return Vector2f(distance_c.x + a/2, distance_c.y + b/2);
    }

    void VertexArray_based_on_existing_circle_list(list<CircleShape*>& circle_list, VertexArray* Vertex_tab, PrimitiveType type, bool connecting)
    {
        // albo tak jak z tym pointerem, zmieniæ wspó³rzêdne coordów --- za du¿o szukania a i tak trzeba zrobiæ nowy obiekt VertexArray

        int size = distance(circle_list.begin(), circle_list.end());
        if (connecting) size++;
        VertexArray recalculated_points(type, size);
        Vector2f tmp;
        int i = 0;
        float radious = 0;
        for (list<CircleShape*>::iterator it = circle_list.begin(); it != circle_list.end(); it++)
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
            list<CircleShape*>::iterator it = (circle_list.begin());
            radious = (*it)->getRadius();
            tmp = (*it)->getPosition();
            tmp.x = tmp.x + radious;
            tmp.y = tmp.y + radious;

            recalculated_points[size - 1] = tmp;
        }

        Vertex_tab[0] = recalculated_points;
    }
    Vector2f Middle_of_a_circle(CircleShape circle)
    {
        float rad = circle.getRadius();
        Vector2f tmp = circle.getPosition();
        tmp.x += rad;
        tmp.y += rad;

        return tmp;
    }
    Vector2f Middle_of_a_circle(CircleShape* circle)
    {
        float rad = circle->getRadius();
        Vector2f tmp = circle->getPosition();
        tmp.x += rad;
        tmp.y += rad;

        return tmp;
    }   
    float a_factor_Line(CircleShape* c_1, CircleShape* c_2)
    {
        Vector2f v_1 = c_1->getPosition();
        Vector2f v_2 = c_2->getPosition();

        float distance_x = v_1.x - v_2.x;
        float distance_y = v_1.y - v_2.y;

        return distance_y / distance_x;
    }

    // mo¿e znajdê use
    void Passing_function(bool& pass, void (*ptr)())
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

    void All_Points_Green(list<CircleShape*>& l)
    {
        for (list<CircleShape*>::iterator it = l.begin(); it != l.end(); it++)
            (*it)->setFillColor(Color::Green);
    }


    // Recalculating // 
    
    // Recalculating mesh
    void Recalculate_Mesh(list<VertexArray>& list_mesh_blocks, list<CircleShape*> list_circle_points, int x_slices, int y_slices)
    {
        list_mesh_blocks.clear();
        Save_points_to_file_blocks(list_circle_points, x_slices, y_slices);
        Visualize_Mesh(list_mesh_blocks);
    }
    // Recalculating lines
    void Recalculate_set_VertexArray_for_distance_circle_lines(list<CircleShape*> l, VertexArray* Vertexarray, CircleShape* distance_circle)
    {        
        int list_size = distance(l.begin(), l.end());
        VertexArray lines(sf::LinesStrip, list_size * 2);
        // *2 bo ka¿dy z punktów musi za ka¿dym razem
        // wracaæ do œrodkowego i do tego innego
        int i = 0;
        for (list<CircleShape*>::iterator it = l.begin(); it != l.end(); it++)
        {
            lines[i] = Middle_of_a_circle(distance_circle);
            lines[i + 1] = Middle_of_a_circle(*it);

            lines[i].color = Color::Yellow;
            lines[i + 1].color = Color::Blue;

            i += 2;
        }

        Vertexarray[1] = lines;
    }
    // Recalculating text for distances
    void Recalcutale_set_text_distance_circle(list<Text>& Ltext, list<CircleShape*> Lcircles, CircleShape* p_distance_calculation_circle, Font& f)
    {
        int i = 1;
        for (list<CircleShape*>::iterator it = Lcircles.begin(); it != Lcircles.end(); it++)
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



    // Mesh Stuff
    Vector2f Mesh_left(list<CircleShape*>& circle_list)
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

        return Middle_of_a_circle(left);
    }
    Vector2f Mesh_right(list<CircleShape*>& circle_list)
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

        return Middle_of_a_circle(right);
    }
    Vector2f Mesh_up(list<CircleShape*>& circle_list)
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

        return Middle_of_a_circle(up);
    }
    Vector2f Mesh_down(list<CircleShape*>& circle_list)
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

        return Middle_of_a_circle(down);
    }

    Vector2f Mesh_left_corner(list<CircleShape*>& circle_list)
    {
        Vector2f left = Mesh_left(circle_list);
        Vector2f down = Mesh_down(circle_list);

        return Vector2f(left.x, down.y);
    }
    Vector2f Mesh_right_corner(list<CircleShape*>& circle_list)
    {
        Vector2f right = Mesh_right(circle_list);
        Vector2f up = Mesh_up(circle_list);

        return Vector2f(right.x, up.y);
    }

    // first x than y
    ////
    /// TRZEBA JESZCZE MODYFIKOWAÆ POZYCJE Z UWAGI NA ŒRODKE PUNKTU
    void Save_points_to_file(list<CircleShape*>& circle_list, float how_many_x_slices, float how_many_y_slices)
    {
        Vector2f bottom_left = Mesh_left_corner(circle_list);
        Vector2f upper_right = Mesh_right_corner(circle_list);

        LOG("bottom left " << bottom_left.x << ". " << bottom_left.y);
        LOG("upper right " << upper_right.x << ". " << upper_right.y);

        float x_distance = bottom_left.x - upper_right.x; if (x_distance < 0) x_distance *= (-1);
        float y_distance = bottom_left.y - upper_right.y; if (y_distance < 0) y_distance *= (-1);

        float x_jumper = x_distance / how_many_x_slices;
        float y_jumper = y_distance / how_many_y_slices;

        LOG("jumper x " << x_jumper);
        LOG("jumper y " << y_jumper);

        ofstream FILE("mesh.txt");

        for (float i = bottom_left.x; i <= upper_right.x; i += x_jumper)
        {
            for (float j = bottom_left.y; j <= upper_right.y; j += y_jumper)
            {
                FILE << i << " " << j << endl;
            }
        }
    }

    // save in blocks        
    void Save_points_to_file_blocks(list<CircleShape*>& circle_list, float how_many_x_slices, float how_many_y_slices)
    {
        Vector2f bottom_left = Mesh_left_corner(circle_list);
        Vector2f upper_right = Mesh_right_corner(circle_list);

        LOG("bottom left " << bottom_left.x << ". " << bottom_left.y);
        LOG("upper right " << upper_right.x << ". " << upper_right.y);

        float x_distance = bottom_left.x - upper_right.x; if (x_distance < 0) x_distance *= (-1);
        float y_distance = bottom_left.y - upper_right.y; if (y_distance < 0) y_distance *= (-1);

        float x_jumper = x_distance / how_many_x_slices;
        float y_jumper = y_distance / how_many_y_slices;

        LOG("jumper x " << x_jumper);
        LOG("jumper y " << y_jumper);

        ofstream FILE("list_mesh_blocks.txt");

        int block_count = 1;
        for (float i = bottom_left.x; i < upper_right.x; i += x_jumper)
        {
            LOG("i: " << i << "   limit: " << upper_right.x);
            
            if (approximatelyEqual(i, upper_right.x, 0.01)) continue;

            for (float j = bottom_left.y; j > upper_right.y; j -= y_jumper)
            {
                if ((int)j == (int)upper_right.y) continue;
                //LOG("j: " << j << "   limit: " << upper_right.y);

                FILE << "Block " << block_count << endl; block_count++;

                FILE << i << " " << j << endl;
                FILE << i + x_jumper << " " << j << endl;
                FILE << i + x_jumper << " " << j - y_jumper << endl;
                FILE << i << " " << j - y_jumper << endl;
            }
        }
    }

    bool approximatelyEqual(float a, float b, float epsilon)
    {
        return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
    }

    void Visualize_Mesh(list<VertexArray>& list_mesh_blocks)
    {
        /*
        int line_counter = 0;
        {
            ifstream LINES("mesh.txt");
            string tmp;
            while (!LINES.eof())
            {
                getline(LINES, tmp);
                line_counter++;
            }
        }
        */

        ifstream FILE("list_mesh_blocks.txt");

        string block; int block_number;
        float x, y;

        while (!FILE.eof())
        {
            /*
            VertexArray mesh(sf::LinesStrip, 5);
            FILE >> block; FILE >> block_number;

            FILE >> x; FILE >> y; mesh[0] = Vector2f(x, y); mesh[4] = Vector2f(x, y);
            FILE >> x; FILE >> y; mesh[1] = Vector2f(x, y);
            FILE >> x; FILE >> y; mesh[2] = Vector2f(x, y);// mesh[3] =
            FILE >> x; FILE >> y; mesh[3] = Vector2f(x, y);
            */

            VertexArray mesh(sf::LinesStrip, 7);
            FILE >> block; FILE >> block_number;

            FILE >> x; FILE >> y; mesh[0] = Vector2f(x, y); mesh[3] = Vector2f(x, y); mesh[6] = Vector2f(x, y);
            FILE >> x; FILE >> y; mesh[1] = Vector2f(x, y);
            FILE >> x; FILE >> y; mesh[2] = Vector2f(x, y); mesh[4] = Vector2f(x, y);
            FILE >> x; FILE >> y; mesh[5] = Vector2f(x, y);

            list_mesh_blocks.push_front(mesh);
        }
    }
};



class my_node;
struct My_node_and_lenght
{
    CircleShape* p_circle;
    float length;

    My_node_and_lenght(CircleShape* obj, float l)
        :p_circle(obj), length(l)
    {
    }
};

class my_node
{
public:
    Operations o;
    // true x
    // false y
    int comparator;
    int number;
    CircleShape* circle_obj;
    Vector2f circle_position;

    my_node* left;
    my_node* right;
    my_node* parent;

    void show_inorder()
    {
        if (this->left != nullptr) this->left->show_inorder();
        if (this->parent == nullptr) cout << "root: " << this << " p: " << this->parent << endl;
        else cout<<"      " << this << " p: " << this->parent << endl;
        if (this->right != nullptr) this->right->show_inorder();
    }

    my_node(CircleShape* obj, int c, my_node* pa, int counter)
    {
        circle_obj = obj;
        circle_position = o.Middle_of_a_circle(circle_obj);
        left = nullptr;
        right = nullptr;
        parent = pa;
        comparator = c;        
        number = counter;

        LOG("Node added circle: " << circle_obj << " " << this << " " << parent
            << " " << comparator << "  " << counter);
    }
    void delete_all()
    {        
        if (this->left != nullptr) this->left->delete_all();
        if (this->right != nullptr) this->right->delete_all();

        delete this;
    }
    // c == true
    void add(CircleShape* circle, int counter)
    {
        Vector2f v = o.Middle_of_a_circle(*circle);

        LOG("Middle of a circle");
        LOG(v.x);
        LOG(v.y);

        // compare based on x
        if (this->comparator > 0)
        {            
            if (v.x <= this->circle_position.x)
            {
                LOG("X - left");
                LOG(this->circle_position.x);
                LOG(this->circle_position.y);

                if (this->left != nullptr) (this->left)->add(circle, counter);
                else this->left = new my_node(circle, -(this->comparator), this, counter);
            }
            else
            {
                LOG("X - right");
                LOG(this->circle_position.x);
                LOG(this->circle_position.y);
                if (this->right != nullptr) (this->right)->add(circle, counter);
                else this->right = new my_node(circle, -(this->comparator), this, counter);
            }
        }

        // compare based on y
        else
        {            
            if (v.y > this->circle_position.y)
            {
                LOG("Y - left");
                LOG(this->circle_position.x);
                LOG(this->circle_position.y);
                if (this->left != nullptr) (this->left)->add(circle, counter);
                else this->left = new my_node(circle, -(this->comparator), this, counter);
            }
            else
            {
                LOG("Y - right");
                LOG(this->circle_position.x);
                LOG(this->circle_position.y);
                if (this->right != nullptr) (this->right)->add(circle, counter);
                else this->right = new my_node(circle, -(this->comparator), this, counter);
            }
        }
    }
        
    void To_List_Circle_and_Length(Vector2f point_position, list<My_node_and_lenght>& list)
    {
        My_node_and_lenght tmp(this->circle_obj, o.Get_length(this->circle_position, point_position));
        list.push_front(tmp);

        // compare based on x
        if (this->comparator > 0)
        {
            if (point_position.x <= this->circle_position.x)
            {
                if (this->left != nullptr) (this->left)->To_List_Circle_and_Length(point_position, list);

                // this->left == nullptr
                else
                {
                    // ostatni check, ¿eby sprawdziæ na pewno wszystkie punkty                    
                    if ((this->parent != nullptr) && (this->parent->right != nullptr))
                    {
                        My_node_and_lenght tmp_(this->parent->right->circle_obj, o.Get_length(this->parent->right->circle_position, point_position));
                        list.push_front(tmp_);
                    }
                }
            }                
                
            else 
            {
                if (this->right != nullptr) (this->right)->To_List_Circle_and_Length(point_position, list);
                
                // this->right == nullptr
                else
                {
                    // ostatni check, ¿eby sprawdziæ na pewno wszystkie punkty
                    if ((this->parent != nullptr) && (this->parent->left != nullptr))
                    {
                        My_node_and_lenght tmp_(this->parent->left->circle_obj, o.Get_length(this->parent->left->circle_position, point_position));
                        list.push_front(tmp_);                        
                    }
                }
            }
        }

        // compare based on y
        else
        {
            if (point_position.y > this->circle_position.y)
            {
                if (this->left != nullptr) (this->left)->To_List_Circle_and_Length(point_position, list);

                // this->left == nullptr
                else
                {
                    // ostatni check, ¿eby sprawdziæ na pewno wszystkie punkty
                    if (this->parent->right != nullptr)
                    {
                        My_node_and_lenght tmp_(this->parent->right->circle_obj, o.Get_length(this->parent->right->circle_position, point_position));
                        list.push_front(tmp_);                        
                    }
                }
            }

            else
            {
                if (this->right != nullptr) (this->right)->To_List_Circle_and_Length(point_position, list);

                // this->right == nullptr
                else
                {
                    // ostatni check, ¿eby sprawdziæ na pewno wszystkie punkty
                    if (this->parent->left != nullptr)
                    {
                        My_node_and_lenght tmp_(this->parent->left->circle_obj, o.Get_length(this->parent->left->circle_position, point_position));
                        list.push_front(tmp_);                        
                    }
                }
            }
        }
    }
};
class my_Tree
{
    my_node* root;
    int counter;

public:
    my_Tree()
    {
        root = nullptr;
        counter = 0;
    }
    my_node* Get_root()
    {
        return root;
    }
    void Clear()
    {
        if (root != nullptr) root->delete_all();
        root = nullptr;
    }
    void Add(CircleShape* obj)
    {
        counter++;
        if (root == nullptr)
        {
            my_node* n = new my_node(obj, 1, nullptr, counter);
            root = n;
        }
        else root->add(obj, counter);
    }

    CircleShape* Return_pointer_to_closest_circle(Vector2f v)
    {
        if (root == nullptr) return nullptr;

        // calculate
        list<My_node_and_lenght> lista;
        LOG("From recursion function");
        root->To_List_Circle_and_Length(v, lista);
        
        // looking for smallest length
        int i = 0;
        list<My_node_and_lenght>::iterator smallest;

        LOG("iterating to find smallest...");
        for (list<My_node_and_lenght>::iterator it = lista.begin(); it != lista.end(); it++)
        {
            if (it == lista.begin() || (it->length < smallest->length)) smallest = it;

            //LOG(i << ".   " << it->node);            
            i++;
        }

        // found smallest
        return (smallest->p_circle);
    }
    void Show_tree_structure()
    {
        if (root != nullptr) root->show_inorder();
    }
};
