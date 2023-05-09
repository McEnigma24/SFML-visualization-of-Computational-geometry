#pragma once
#include "header.h"

// Pojedynczy node jakby drzewa 4, gdzie ka¿dy ma 4 nody

QuadTree::quad_entry::quad_entry(Limits obj, int** t, int& counter, list<VertexArray>& line_list, int& x_offset, int& y_offset)
{
    counter++;
    //cout << counter << endl;

    tab = t;
    up_left = nullptr;
    up_right = nullptr;
    down_left = nullptr;
    down_right = nullptr;


    // Adding to list
    Operations o;

    //LOG( endl << "Limits");
    line_list.push_back(o.Cross_with_lines___with_limits(obj, x_offset, y_offset));
    //LOG("       "); obj.Print();
    //cout << "offset "; obj.Print_offset(x_offset, y_offset);

    //cout << endl << "middle position" << endl;
    //cout << "       "; Vector2f middle = obj.Get_middle_position(); o.Print_Vector(middle);
    //cout << "offset "; middle = obj.Get_middle_position_offset(x_offset, y_offset); o.Print_Vector(middle);

    // rekurencja
    if ((obj.Distance_of_Both(QUAD_LIMIT) && obj.Border_or_Not(t)))
    {
        // up left
        {
            //cout << "up left\n";
            //obj.Get_up_left().Print();                    
            up_left = new quad_entry(obj.Get_up_left(), t, counter, line_list, x_offset, y_offset);
        }

        // up right
        {
            //cout << "up right\n";
            //obj.Get_up_right().Print();
            up_right = new quad_entry(obj.Get_up_right(), t, counter, line_list, x_offset, y_offset);
        }

        // down left
        {
            //cout << "down left\n";
            //obj.Get_down_left().Print();
            down_left = new quad_entry(obj.Get_down_left(), t, counter, line_list, x_offset, y_offset);
        }

        // down right
        {
            //cout << "down right\n";
            //obj.Get_down_right().Print();
            down_right = new quad_entry(obj.Get_down_right(), t, counter, line_list, x_offset, y_offset);
        }
    }
}

int QuadTree::quad_entry::Distance_between(int a, int b)
{
    int result = a - b; if (result < 0) result *= -1;
    return result;
}
bool QuadTree::quad_entry::check_if_border(int istart, int istop, int jstart, int jstop)
{
    int first_value = tab[istart][jstart];

    for (int i = istart; i < istop; i++)
    {
        for (int j = jstart; j < jstop; j++)
        {
            if (first_value != tab[i][j]) return true;
        }
    }

    return false;
}
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
void QuadTree::quad_entry::Delete_All()
{
    if (up_left != nullptr) up_left->Delete_All();
    if (up_right != nullptr) up_right->Delete_All();
    if (down_left != nullptr) down_left->Delete_All();
    if (down_right != nullptr) down_right->Delete_All();

    delete this;
}


QuadTree::QuadTree(string path, int x_off, int y_off)
{
    x_offset = x_off; y_offset = y_off;
    i_stop = 0;
    j_stop = 0;

    TxtOperations<int> txt;
    tab = txt.txt_to_2d_tab(path, i_stop, j_stop); if (tab == nullptr) cout << "Unable to load data\n";

    cout << "sprawdzam czy funkcja zlamala wartosci" << endl;
    cout << "i_stop: " << i_stop
        << " j_stop: " << j_stop << endl;

    first = nullptr;

    // check
    /*
    int i_limit = first->i_stop;
    int j_limit = first->j_stop;

    for (int i = 0; i < i_limit; i++)
    {
        for (int j = 0; j < j_limit; j++)
        {
            cout << first->tab[i][j] << " ";
        }
        cout << endl;
    }
    */
}
QuadTree::~QuadTree()
{
    if (tab != nullptr)
    {
        for (int i = 0; i < i_stop; i++) delete[] tab[i];
        delete[] tab;
    }

    if (first != nullptr) first->Delete_All();
    first = nullptr;
}

void QuadTree::Start_Quadding()
{
    int counter = 0;
    lista_z_liniami_do_wyswitlania.clear();

    // Drawing BIG rectangle
    {
        Vector2f tab[4];
        tab[0] = Vector2f(x_offset, y_offset);
        tab[1] = Vector2f(j_stop + x_offset, y_offset);
        tab[2] = Vector2f(j_stop + x_offset, i_stop + y_offset);
        tab[3] = Vector2f(x_offset, i_stop + y_offset);

        Operations o;
        VertexArray rectangle = o.Draw_a_rectangle(tab, Color::Yellow);
        lista_z_liniami_do_wyswitlania.push_back(rectangle);
    }

    first = new quad_entry(Limits(0, i_stop, 0, j_stop), tab, counter, lista_z_liniami_do_wyswitlania, x_offset, y_offset);
    //cout << "quading finished" << endl;


    // wyœwietlamy cross_lines__with_limits
    // na ka¿dym nodzie --- i obramowania do tego g³ównego

    // t¹ macierz robimy translacje indexów na myVector
    // *myvector do Cross_with_lines___with_limits ¿eby wiedzia³ sk¹d rysowaæ
}
list<VertexArray>& QuadTree::Get_List_for_visualization() { return lista_z_liniami_do_wyswitlania; }