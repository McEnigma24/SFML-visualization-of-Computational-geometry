#pragma once
#include "Header.h"
#include <algorithm>

// using HASH FUNCTION myVector - saveCoordinetsToFile();
namespace std {

    //template <>
    //class hash<Float>
    //{
    //    std::size_t operator()(const Float& k) const
    //    {
    //        // Compute individual hash values for first,
    //        // second and third and combine them using XOR
    //        // and bit shifting:

    //        return ((hash<int>()((int)k.y)));
    //    }
    //};

    template <>
    class hash<myVector>
    {
        public:
        std::size_t operator()(const myVector& k) const
        {
            // Compute individual hash values for first,
            // second and third and combine them using XOR
            // and bit shifting:            

            /*return
                (
                    (hash<int>()((int)k.x) ^ 1) +
                    (hash<Float>()(k.y) ^ 2)
                );*/

            int x_int = static_cast<int>(k.x);
            int y_int = static_cast<int>(k.y);

            int result = x_int >= y_int ? x_int * x_int + x_int + y_int : x_int + y_int * y_int;

            return hash<int>()(result);
        }
    };

}

// Pojedynczy node jakby drzewa 4, gdzie ka¿dy ma 4 nody

QuadTree::quad_coordinets_to_file::quad_coordinets_to_file()
    :up_left(false), up_right(false), down_left(false), down_right(false), m_id(0)
{
}
bool QuadTree::quad_coordinets_to_file::operator==(const quad_coordinets_to_file& other)
{
    return (up_left == other.up_left && up_right == other.up_right
        && down_left == other.down_left && down_right == other.down_right);
}
bool QuadTree::quad_coordinets_to_file::operator!=(const quad_coordinets_to_file& other)
{
    return !this->operator==(other);
}



QuadTree::quad_entry::quad_entry(Limits obj, int** t, int& counter, 
    list<VertexArray>& line_list, list<QuadTree::quad_coordinets_to_file>& list_coordinets, 
    int& x_offset, int& y_offset,
    int quad_limit)
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

    QuadTree::quad_coordinets_to_file coordinets;

    coordinets.m_id = counter;
    coordinets.up_left = obj.getCoordinetsCorner_UpLeft(true);
    coordinets.up_right = obj.getCoordinetsCorner_UpRight(true);
    coordinets.down_left = obj.getCoordinetsCorner_DownLeft(true);
    coordinets.down_right = obj.getCoordinetsCorner_DownRight(true);

    list_coordinets.push_back(coordinets);
    //LOG("       "); obj.Print();
    //cout << "offset "; obj.Print_offset(x_offset, y_offset);

    //cout << endl << "middle position" << endl;
    //cout << "       "; Vector2f middle = obj.Get_middle_position(); o.Print_Vector(middle);
    //cout << "offset "; middle = obj.Get_middle_position_offset(x_offset, y_offset); o.Print_Vector(middle);

    // rekurencja
    if ((obj.Distance_of_Both(quad_limit) && obj.Border_or_Not(t)))
    {
        // up left
        {
            //cout << "up left\n";
            //obj.Get_up_left().Print();                    
            up_left = new quad_entry(obj.Get_up_left(), t, counter, line_list, list_coordinets, x_offset, y_offset, quad_limit);
        }

        // up right
        {
            //cout << "up right\n";
            //obj.Get_up_right().Print();
            up_right = new quad_entry(obj.Get_up_right(), t, counter, line_list, list_coordinets, x_offset, y_offset, quad_limit);
        }

        // down left
        {
            //cout << "down left\n";
            //obj.Get_down_left().Print();
            down_left = new quad_entry(obj.Get_down_left(), t, counter, line_list, list_coordinets, x_offset, y_offset, quad_limit);
        }

        // down right
        {
            //cout << "down right\n";
            //obj.Get_down_right().Print();
            down_right = new quad_entry(obj.Get_down_right(), t, counter, line_list, list_coordinets, x_offset, y_offset, quad_limit);
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

    // dopisaæ nak³adke co te¿ zwraca pointer tylko po analizie obrazka i te¿ ustawia i_stop, j_stop


    cout << "sprawdzam czy funkcja zlamala wartosci" << endl;
    cout << "i_stop: " << i_stop << " j_stop: " << j_stop << endl;

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
int** QuadTree::getTab()
{
    return tab;
}
int** QuadTree::getTab(int& i_end, int& j_end)
{
    i_end = i_stop;
    j_end = j_stop;

    return tab;
}
offSet QuadTree::getOffSet()
{
    return offSet(x_offset, y_offset);
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

void QuadTree::Start_Quadding(int quad_limit)
{
    int counter = 0;
    lista_z_liniami_do_wyswitlania.clear();
    list_quad_coordinets.clear();

    // Drawing BIG rectangle
    {
        Vector2f tab[4];
        tab[0] = Vector2f(static_cast<float>(x_offset), static_cast<float>(y_offset));
        tab[1] = Vector2f(static_cast<float>(j_stop + x_offset), static_cast<float>(y_offset));
        tab[2] = Vector2f(static_cast<float>(j_stop + x_offset), static_cast<float>(i_stop + y_offset));
        tab[3] = Vector2f(static_cast<float>(x_offset), static_cast<float>(i_stop + y_offset));

        Operations o;
        VertexArray rectangle = o.Draw_a_rectangle(tab, Color::Yellow);
        lista_z_liniami_do_wyswitlania.push_back(rectangle);
    }
         
    first = new quad_entry(Limits(0, i_stop, 0, j_stop), tab, counter, lista_z_liniami_do_wyswitlania, list_quad_coordinets, x_offset, y_offset, quad_limit);
        
    
    // wyœwietlamy cross_lines__with_limits
    // na ka¿dym nodzie --- i obramowania do tego g³ównego

    // t¹ macierz robimy translacje indexów na myVector
    // *myvector do Cross_with_lines___with_limits ¿eby wiedzia³ sk¹d rysowaæ
}
void QuadTree::Cleaning_quad_entry()
{
    if (first != nullptr) first->Delete_All();
    first = nullptr;
}

void QuadTree::saveCoordinetsToFile(string path)
{
    ofstream FILE(path.c_str());
    unordered_map<myVector, int> id_of_coordinet;

    // przyporz¹dkowujemy ka¿dej wspó³rzêdnej id
    {
        int id = 1;
        for (auto& coord : list_quad_coordinets)
        {
            // jeszcze nie ma takiej wspó³rzêdnej
            const myVector ul = coord.up_left;
            const myVector ur = coord.up_right;
            const myVector dl = coord.down_left;
            const myVector dr = coord.down_right;

            if (id_of_coordinet.find(ul) == id_of_coordinet.end())
            {
                id_of_coordinet[ul] = id;                
                id++;
            }

            if (id_of_coordinet.find(ur) == id_of_coordinet.end())
            {
                id_of_coordinet[ur] = id;
                id++;
            }

            if (id_of_coordinet.find(dr) == id_of_coordinet.end())
            {
                id_of_coordinet[dr] = id;
                id++;
            }

            if (id_of_coordinet.find(dl) == id_of_coordinet.end())
            {
                id_of_coordinet[dl] = id;
                id++;
            }
        }
    }

    // cwane przerzucenie na inn¹ strukturê
    std::vector<std::pair<myVector, int>> elems(id_of_coordinet.begin(), id_of_coordinet.end());
    
    // sortowanie wspó³rzêdnych 
    {
        std::sort(elems.begin(), elems.end(), [](pair<myVector, int> a, pair<myVector, int> b)
            {
                return a.second < b.second;
            }
        );
    }

    // zapisanie do pliku nodes
    {
        FILE << "nodes" << endl;
        for (auto& id_coord : elems)
            FILE << id_coord.second << " - " << id_coord.first.x << " " << id_coord.first.y << "\n";
    }

    // dzielenie na trójk¹ty i zapisywanie w³aœciwych id do wspó³rzêdnych
    {
        int quad_x_limit{};     int quad_y_limit{};
        int** tmp_tab = this->getTab(quad_y_limit, quad_x_limit);

        //cout << "x limit: " << quad_x_limit << endl;
        //cout << "y limit: " << quad_y_limit << endl;

        FILE << endl << "elements" << endl;

        int id = 1;     int y_coord; int x_coord;
        for (auto& coord : list_quad_coordinets)
        {
            const myVector ul = coord.up_left;
            const myVector ur = coord.up_right;
            const myVector dl = coord.down_left;
            const myVector dr = coord.down_right;

            myVector ull = coord.up_left;
            myVector urr = coord.up_right;
            myVector dll = coord.down_left;
            myVector drr = coord.down_right;
            
            Vector2f incircle;


            // górny trójk¹t
            {
                FILE << id << " - ";

                FILE << id_of_coordinet[ul] << " ";
                FILE << id_of_coordinet[ur] << " ";
                FILE << id_of_coordinet[dl];

                id++;

                {
                    Triangle triangle(*myVector(*ull, true), *myVector(*urr, true), *myVector(*dll, true));
                    incircle = triangle.getPositionForNumber();

                    y_coord = static_cast<int>(incircle.y);
                    x_coord = static_cast<int>(incircle.x);

                    //cout << "y " << y_coord << " x " << x_coord << " --- " << tab[y_coord][x_coord] << endl;
                    if (y_coord < quad_y_limit && x_coord < quad_x_limit)
                    {
                        if (tab[y_coord][x_coord] == 0)   FILE << " > osnowa";
                        else                              FILE << " > wydzielenie";
                    }
                }

                FILE << endl;
            }

            // dolny trójk¹t
            {
                FILE << id << " - ";

                FILE << id_of_coordinet[dl] << " ";
                FILE << id_of_coordinet[dr] << " ";
                FILE << id_of_coordinet[ur];

                id++;

                {
                    Triangle triangle(*myVector(*dll, true), *myVector(*drr, true), *myVector(*urr, true));
                    incircle = triangle.getPositionForNumber();

                    y_coord = static_cast<int>(incircle.y);
                    x_coord = static_cast<int>(incircle.x);

                    //cout << "y " << y_coord << " x " << x_coord << " --- " << tab[y_coord][x_coord] << endl;
                    if (y_coord < quad_y_limit && x_coord < quad_x_limit)
                    {
                        if (tab[y_coord][x_coord] == 0)   FILE << " > osnowa";
                        else                              FILE << " > wydzielenie";
                    }
                }

                FILE << endl;
            }
        }
    }

    FILE.close();
}

list<VertexArray> QuadTree::getListForVisualization() { return lista_z_liniami_do_wyswitlania; }
list<QuadTree::quad_coordinets_to_file> QuadTree::getListOfCoordinets() { return list_quad_coordinets; }