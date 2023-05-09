//#ifndef MAIN_HEADER_H
//#define MAIN_HEADER_H
// to na koñcu pliku     //#endif // !MAIN_HEADER_H
#pragma once
#include  <SFML/Graphics.hpp>

#include <iostream>
#include "windows.h"
#include <filesystem>
#include <fstream>
#include <string>

#include <stdlib.h>
#include <random>
#include <time.h>
#include <cmath>
#include <math.h>

#include <chrono>
#include <thread>

#include <list>
#include <array>
#include <unordered_map>

#include "PreProcessor.h"

struct ScopeTimer
{
    typedef std::chrono::system_clock::time_point clock;

    clock start;
    clock stop;

    string name;
    string file;
    bool new_line;

    ScopeTimer(string n, string f);
    ScopeTimer(string n, string f, int nl);
    ~ScopeTimer();
};

struct offSet
{
    int x_offset;
    int y_offset;

    offSet(int x, int y)
        :x_offset(x), y_offset(y) {}
};

struct Float
{
    float y;
public:
    float Get_y();

    // r-value // never used
    /*
    Float(float&& yy, bool recal_y)
    {
        LOG("&& constructor");
        if (recal_y) y = REC(yy);
        else y = yy;
    }
    */
    // l-value
    Float(float yy, bool recal_y);
    Float(int yy, bool recal_y);
    Float& operator=(float yy);

    void operator+=(float add);
    void operator-=(float sub);
};

class myVector
{
public:
    float x;
    Float y;

    myVector(bool recal_y);
    myVector(float xx, float yy, bool recal_y);

    myVector(Vector2f v, bool recal_y);

    myVector(CircleShape c, bool recal_y);
    myVector(CircleShape* c, bool recal_y);

    void Offset(int x_offset, int y_offset);
    void Offset(offSet off);

    void Print();

    Vector2f operator*();
};

class myCircle
{
public:
    // wspó³gra z odwróconymi operacjami dla "y"
    Vector2f getMiddle(CircleShape c);
    Vector2f getMiddle(CircleShape* c);

    Vector2f setMiddle(CircleShape c);
    Vector2f setMiddle(CircleShape* c);
};

struct Limits
{
private:
    int i_start;    int i_stop;
    int j_start;    int j_stop;

public:
    int Distance(int a, int b);
    bool Distance_of_Both(int limit);
    bool Border_or_Not(int** tab);

public:
    Limits();
    Limits(int istart, int istop, int jstart, int jstop);
    Limits(const Limits& other);
    void Print();
    void Print_offset(int x_offset, int y_offset);

    Limits Get_up_left();
    Limits Get_up_right();
    Limits Get_down_left();
    Limits Get_down_right();

    Vector2f Get_middle_position();
    Vector2f Get_middle_position_offset(int x_offset, int y_offset);

    // In respect to the middle
    Vector2f Middle_point_of_side_Left(int x_offset, int y_offset);
    Vector2f Middle_point_of_side_Right(int x_offset, int y_offset);
    Vector2f Middle_point_of_side_Up(int x_offset, int y_offset);
    Vector2f Middle_point_of_side_Down(int x_offset, int y_offset);
};

template<typename T>
class TxtOperations
{
public:
    // zczytanie z pliku do macierzy ---> zwraca wymiary
    T** txt_to_2d_tab(string p, int& i_limit, int& j_limit)
    {
        const char* path = p.c_str();

        ifstream FILE(path);
        if (!FILE.good()) return nullptr;

        int width, height;
        char tmp;

        FILE >> width;
        FILE >> height;

        // cout << width << " " << height << endl;

        T** tab = new T * [height]; for (int c = 0; c < height; c++) tab[c] = new T[width];

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                FILE >> tmp;
                tab[i][j] = tmp - 48;
                //cout << tmp << endl;
            }
        }
        FILE.close();

        i_limit = height;
        j_limit = width;

        return tab;
    }
};



class Operations : public myCircle
{
public:

    /////////////////////////////////////////////////////////////////
    Vector2f Middle_of_a_circle_no_yREC(CircleShape circle);
    Vector2f Middle_of_a_circle_no_yREC(CircleShape* circle);
    Vector2f Middle_of_a_circle_no_yREC(const CircleShape* circle);

    Vector2f Middle_of_a_circle_no_yREC(Vector2f positon, float rad);
    Vector2f Position_to_draw_middle_of_circle(Vector2f&& point, float rad);
    Vector2f Position_to_draw_middle_of_circle(Vector2f& point, float rad);
    void Position_to_draw_middle_of_circle_void(Vector2f& point, float rad);
    /////////////////////////////////////////////////////////////////

    class Old_file_operations
    {
        string name;
        PrimitiveType type;
        bool first;
    public:

        Old_file_operations(PrimitiveType t);
        Old_file_operations(string n, PrimitiveType t);
        void Change_PrimitiveType(PrimitiveType t);

        // zczytywanie z pliku
        Vector2f* get_points_from_file(int& size, string file_name);

        // wpisuje wartoœci wektorów do VertexArray    
        VertexArray construct_lines_based_on_input(Vector2f* tab, int size);

        // otoczka zczytania z pliku, tworzy gotowy do wyœwietlenia VertexArray
        VertexArray construct_lines_from_file(string file_name);
        VertexArray construct_lines_at_random(int how_many);

        void save_points_from_file_to_list(list<VertexArray>& list);
    };

    // mo¿e znajdê use
    void Passing_function(bool& pass, void (*ptr)());
public:

    // Essentials //
    void All_Points_Green(list<CircleShape*>& l);

    float Get_length(float x1, float y1, float x2, float y2);
    float Get_length(Vector2f firts, Vector2f second);
    float Get_length(CircleShape* c, CircleShape* ca);

    Vector2f Middle_point(CircleShape* distance_circle, CircleShape* ca);
    float a_factor_Line(CircleShape* c_1, CircleShape* c_2);

    void Print_Vector(Vector2f vec);






    ///////////////////////////////////////////////////////////////// 
    // Vertex Array --- check if point is in the right position
    VertexArray Cross_with_lines(Vector2f middle);
    VertexArray Cross_with_lines___with_limits(Vector2f middle, int up_y_limit, int down_y_limit,
        int left_x_limit, int right_x_limit);

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

    VertexArray Cross_with_lines___with_limits(Limits lim, int x_offset, int y_offset);


    VertexArray Draw_a_rectangle(Vector2f* tab, Color c);
    /////////////////////////////////////////////////////////////////



    // For Mesh //
    Vector2f Mesh_left(list<CircleShape*>& circle_list);
    Vector2f Mesh_right(list<CircleShape*>& circle_list);
    Vector2f Mesh_up(list<CircleShape*>& circle_list);
    Vector2f Mesh_down(list<CircleShape*>& circle_list);
    Vector2f Mesh_left(const list<CircleShape*>& circle_list);
    Vector2f Mesh_right(const list<CircleShape*>& circle_list);
    Vector2f Mesh_up(const list<CircleShape*>& circle_list);
    Vector2f Mesh_down(const list<CircleShape*>& circle_list);

    Vector2f Mesh_left_corner(list<CircleShape*>& circle_list);
    Vector2f Mesh_right_corner(list<CircleShape*>& circle_list);
    Vector2f Mesh_left_corner(const list<CircleShape*>& circle_list);
    Vector2f Mesh_right_corner(const list<CircleShape*>& circle_list);


    // For Triangulation //
    Vector2f Down_left(list<CircleShape*>& circle_list);
    Vector2f Down_right(list<CircleShape*>& circle_list);
};

class Line : private Operations
{
public:
    Vector2f point1, point2;
    Vector2i intPoint1, intPoint2;
    double a, b;

    Line();
    Line(double aa, double bb);
    Line(Vector2f p1, Vector2f p2);
    Line(CircleShape c1, CircleShape c2);
    bool operator==(const Line& other) const;
    bool operator!=(const Line& other) const;

    // intersecting 2 lines
    Vector2f Point_of_Intersect(Line l1, Line l2);
    Vector2f MiddleOfLine();

    Vector2f RandomPointOnLine();
    Vector2f NextPointAfter(Vector2f before, float jump, bool forward_backword);

    Line Line_between(Vector2f p1, Vector2f p2);
    
    float Angle_between(Vector2f middle, Vector2f left, Vector2f right);
    float Angle_between(CircleShape* point_of_reference, CircleShape* m_point);


    // wrong way to measure angle
    /*
    double Angle_between(Line line_a, Line line_b)
    {
        // to chyba nie dzia³a dobrze

        Operations o;
        Line perpendicular;

        Vector2f same;
        Vector2f line_a_different;
        Vector2f line_b_different;

        // calculating perpendicular
        {
            perpendicular.a = -(line_a.a);

            // warunki, ¿eby z³apaæ poprawne punkty
            {
                if (line_a.point1 == line_b.point1)
                {
                    same = line_a.point1;
                    line_a_different = line_a.point2;
                    line_b_different = line_b.point2;
                }
                else if (line_a.point2 == line_b.point1)
                {
                    same = line_a.point2;
                    line_a_different = line_a.point1;
                    line_b_different = line_b.point2;
                }
                else if (line_a.point1 == line_b.point2)
                {
                    same = line_a.point1;
                    line_a_different = line_a.point2;
                    line_b_different = line_b.point1;
                }
                // (line_a.point2 == line_b.point2)
                else
                {
                    same = line_a.point2;
                    line_a_different = line_a.point1;
                    line_b_different = line_b.point1;
                }
            }

            perpendicular.b = (line_b_different.y - (perpendicular.a * line_b_different.x));
        }

        Vector2f intersect = Point_of_Intersect(line_a, perpendicular);

        double length_a = o.Get_length(same, intersect);
        double length_b = o.Get_length(line_b_different, intersect);

        return atan(length_b / length_a);
    }
    */
};

class Triangle
{
public:
    Vector2f A, B, C;
    double Aangle; double Bangle; double Cangle;

    Triangle();
    Triangle(Vector2f aa, Vector2f bb, Vector2f cc);
    Triangle(CircleShape aa, CircleShape bb, CircleShape cc);
    void infoAboutPosition();

    void getAngles();
    bool areAllPointsInLine();
    bool anyCommnoPointWith(Triangle t);


    // For Recursion //
    Triangle downLeft();
    Triangle downRight();
    Triangle up();
    Triangle middle();
};

class Mesh : private Operations
{
public:

    void Save_points_to_file(list<CircleShape*>& circle_list, float how_many_x_slices, float how_many_y_slices);
    void Save_points_to_file_blocks(const list<CircleShape*>& circle_list, float how_many_x_slices, float how_many_y_slices);

    bool approximatelyEqual(float a, float b, float epsilon);
    void Visualize_Mesh(list<VertexArray>& list_mesh_blocks);
};

class QuadTree
{
    // Pojedynczy node jakby drzewa 4, gdzie ka¿dy ma 4 nody

    struct quad_entry
    {
        quad_entry* up_left; quad_entry* up_right; quad_entry* down_left; quad_entry* down_right;
        int** tab;

        quad_entry(Limits obj, int** t, int& counter, list<VertexArray>& line_list, int& x_offset, int& y_offset);

        int Distance_between(int a, int b);
        bool check_if_border(int istart, int istop, int jstart, int jstop);
        /*
        void visiting_every_node(list<VertexArray> lista_z_liniami_do_wyswitlania, int adding_to_visualize)
        {
            // sprawdziæ czy potrzebne jest pisanie this->
            myVector my(i_stop / 2 + adding_to_visualize, j_stop / 2 + adding_to_visualize);
            //Vector2f myv(i_stop / 2 + adding_to_visualize, j_stop / 2 + adding_to_visualize);

            // teraz u¿ywam myVector
            (lista_z_liniami_do_wyswitlania).push_back(Cross_with_lines___with_limits(*my,
                i_start + adding_to_visualize, i_stop + adding_to_visualize,
                j_start + adding_to_visualize, j_stop + adding_to_visualize));

            if (up_left != nullptr) up_left->visiting_every_node(lista_z_liniami_do_wyswitlania, adding_to_visualize);
            if (up_right != nullptr) up_right->visiting_every_node(lista_z_liniami_do_wyswitlania, adding_to_visualize);
            if (down_left != nullptr) down_left->visiting_every_node(lista_z_liniami_do_wyswitlania, adding_to_visualize);
            if (down_right != nullptr) down_right->visiting_every_node(lista_z_liniami_do_wyswitlania, adding_to_visualize);
        }
        */
        void Delete_All();
    };

    // t¹ listê pó¿niej przerobimy na VertexArray dla ka¿dego z nich i to damy na listê list<VertexArray> do wyœwietlania
    list<VertexArray> lista_z_liniami_do_wyswitlania;
    quad_entry* first;

    int i_stop;
    int j_stop;
    int** tab;
    int x_offset; int y_offset; // no recalculation

public:
    QuadTree(string path, int x_off, int y_off);
    ~QuadTree();

    void Start_Quadding();

    list<VertexArray>& Get_List_for_visualization();
};

class ConvexShell
{
    struct Angle_and_Point
    {
        float angle;
        CircleShape* measuring_point;

        Angle_and_Point();
        Angle_and_Point(CircleShape* point_of_reference, CircleShape* m_point);
        Angle_and_Point& operator=(const Angle_and_Point& other)
        {
            angle = other.angle;
            measuring_point = other.measuring_point;

            return *this;
        }

        Angle_and_Point(CircleShape* befor_last_to_reference, CircleShape* last_to_reference, CircleShape* m_point);
    };

    bool Compare_for_Sort(Angle_and_Point a, Angle_and_Point b);
    CircleShape* bottom_point(const list<CircleShape*>& list_of_points);
    CircleShape* smallest_angle_point(vector<Angle_and_Point>& vec);

public:

    list<CircleShape*> Alg_Graham(const list<CircleShape*>& list_of_points);
};

class Triangulation : private Operations
{
    struct possible_positions_for_triangle
    {
        Vector2f m_one;
        Vector2f m_two;

        possible_positions_for_triangle(Vector2f o, Vector2f t)
            :m_one(o), m_two(t){}
    };

    // tylko do funkcji
    Line l;    

public:    

    bool checkIfPointIsInTriangle(Vector2f* triangle_points, Vector2f point);
    bool checkIfAllPointsAreInTriangle(Vector2f t_left, Vector2f t_right, Vector2f t_up, list<CircleShape*>& lpoints);


    // Super Triangle
    Triangle createSuperTriangle(list<CircleShape*>& lpoints);

    // Circium Circle
    Vector2f sinMiddleOfCircle(Triangle t);
    Vector2f sinMiddleOfCircle(Triangle t, float& rad);

    // Identifying bad Triangles
    void getBadTriangles(list<Triangle>& originalTriangles, list<Triangle>& badTriangles, Vector2f newPoint);

    // Creating new Triangles from badTriangles
    void newTriangles(list<Triangle>& originalTriangles, list<Triangle>& badTriangles, Vector2f newPoint);
    
    // Deleting triangles with connections to Super Triangle
    void removeTrianglesCommonTo(list<Triangle>& originalTriangles, Triangle superT);
    

    // Delaunay Triangulation //
    list<Triangle> triangulationDelaunay(list<CircleShape*>& lpoints);
};


class VertexArrayFunctions
{
    // LineStrip ---> linie ³¹cz¹ce kolejne punkty
    // Lines     ---> tylko kolejne 2 punkty s¹ po³¹czone

public:

    // napisaæ funkcje w³aœciwie do user input --- podajesz listê punktów 
    // i robi kó³ka w tych miejscach

    list<CircleShape*> drawFileInputPoints(string p, float rad);
    
    VertexArray drawLine(Line l, Color c);

    VertexArray drawTriangle(Triangle t, Color c, offSet off);
    list<CircleShape> drawPointsOnEndOfTriangle(Triangle t, Color c, offSet off, float rad);
};

class InterestingVisualizations : private Operations, private VertexArrayFunctions
{
    struct recursion_limits
    {
        int up_left_limit;        int up_right_limit;
        int down_left_limit;        int down_right_limit;

        recursion_limits(int ul, int ur, int dl, int dr)
            :up_left_limit(ul), up_right_limit(ur), down_left_limit(dl), down_right_limit(dr) {}
    };

    struct recursion_limits_for_Triangle
    {
        int downLeft;
        int downRight;
        int up;
        int middle;

        recursion_limits_for_Triangle(int dl, int dr, int u, int m)
            :downLeft(dl), downRight(dr), up(u), middle(m) {}
    };

    void Body_Recursion_with_Cross_in_Rectangle(list<VertexArray>& cross_list, Limits obj, int& x_offset, int& y_offset, recursion_limits limits);
    void Body_Recursion_with_Cross_in_Rectangle(list<VertexArray>& cross_list, Limits obj, int& x_offset, int& y_offset, int limit);

    void Body_RecursionWithTriangles(list<VertexArray>& triangle_list, Triangle t, offSet off, int limit);
    void Body_RecursionWithTriangles(list<VertexArray>& triangle_list, Triangle t, offSet off, recursion_limits_for_Triangle limit);

public:

    list<VertexArray> RecursionWithTriangles(Triangle starting, offSet off, int limits);
    list<VertexArray> RecursionWithTriangles(Triangle starting, offSet off, int down_left, int down_right, int up, int middle);


    list<VertexArray> Recursion_with_Cross_in_Rectangle(int width, int height, int x_offset, int y_offset, int limits);
    list<VertexArray> Recursion_with_Cross_in_Rectangle(int width, int height, int x_offset, int y_offset, int up_left, int up_right, int down_left, int down_right);
};


class Recalculating : protected Operations
{
public:
    // Recalculating Lines
    void recalculateLinesVertexArrayBasedOnExistingCircleList(const list<CircleShape*>& circle_list, VertexArray* Vertex_tab, PrimitiveType type, bool connecting);
    // Recalculating mesh
    void recalculateMesh(list<VertexArray>& list_mesh_blocks, list<CircleShape*> list_circle_points, int x_slices, int y_slices);
    // Recalculating lines
    void recalculateSetVertexArrayForDistanceCircleLines(const list<CircleShape*>& l, VertexArray* Vertexarray, const CircleShape* distance_circle);
    // Recalculating text for distances
    void recalculateSetTextDistanceCircle(list<Text>& Ltext, const list<CircleShape*>& Lcircles, CircleShape* p_distance_calculation_circle, const Font& f);
    // Recalculating closect neightbour brout force
    void recalculateClosestNeightbour(list<CircleShape*> l, CircleShape* p_distance_calculation_circle);
    // Recalculating convex shell
    void recalculateConvexShell(const list<CircleShape*>& list_of_points, VertexArray* Vertexarray);
    // Recalculating circle for 3 points
    void recalculate3pointsCircle(const list<CircleShape*>& list_of_points, CircleShape** circum_circle, Color c, Triangle** t_lines);
    // Recalculating Triangulation
    void recalculateTriangulation(list<CircleShape*>& list_of_points, list<Triangle>& list_triangulaton_triangles);
};

struct Pass : private Recalculating
{
    // Tokens - for one time recalculation (Heavy Operations)
    struct Token
    {
        static const int token_size = 7;
        array<bool, token_size> token_tab;

        // token_tab[0] == mesh_token
        // token_tab[1] == mesh_token
        // token_tab[2] == mesh_token
        // token_tab[3] == mesh_token
        // token_tab[4] == convex_shell
        // token_tab[5] == circum_circle
        // token_tab[6] == triangulation

        Token();
        void Clear_Tokes();
    };

    bool prop_drawing_circles;
    bool prop_show_menu;
    bool prop_connecting_activated;

    bool pass_mesh;
    bool pass_connecting_points;
    bool pass_lines_to_all;
    bool pass_distance_to_all;
    bool pass_closest_point;
    bool pass_convex_shell;
    bool pass_circum_circle;
    bool pass_triangulation;

    Token tokens;

private:

    // const - ¿eby przypadkiem nie zmieniæ wartoœci na jak¹ pokazuje pointer
    // pointery - dla tego, ¿e funkcje musz¹ mieæ dostêp do faktycznych obiektów

    list<CircleShape*>* list_circle_points; // pointer do listy
    list<VertexArray>* list_mesh_blocks; int x_slices; int y_slices;

    VertexArray** tab_VertexArray_points_in_circle;
    CircleShape** p_distance_calculation_circle;
    CircleShape** p_chosen_circle_for_moving;
    CircleShape** p_circum_circle;
    Triangle** p_circum_triangle;

    const Font* font;
    list<Text>* list_length_of_distances_dcl;

    list<Triangle>* list_triangulation;

public:

    Pass(list<CircleShape*>* p_list_circle_points, VertexArray** p_tab_VertexArray_points_in_circle,
        list<VertexArray>* p_list_mesh_blocks, int x_sl, int y_sl,
        CircleShape** ppp_chosen_circle_for_moving, CircleShape** ppp_distance_calculation_circle, Font* p_font,
        list<Text>* p_list_length_of_distances_dcl, 
        CircleShape** circum_circle, Triangle** circum_triangle,
        list<Triangle>* p_list_triangulation
    );


    // Mo¿e do wszystkich daæ tokeny i to bêdzie tak samo jak wywo³ywanie funkcji tylko
    // ³adniej
    void Token_Mesh();
    void Token_Lines();
    void Token_Distance_line();
    void Token_Distance_text();
    void Token_Convex_shell();
    void Token_Circum_Circle();
    void tokenTriangulation();

    void Recalculating();
};