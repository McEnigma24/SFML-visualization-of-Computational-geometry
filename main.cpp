#include "incl_tree_operations.cpp"

// change the target to x64
// Project -> Properties (last on the list)
// Configurations: Debug -> Debugging -> Environment -> PATH=S:\Programy Visual Studio\aaa SFML - for Visualization\SFML-2.5.1\bin
// PATH= gdziekolwiek jest folder bin plików SFML
// apply -> ok

string cr, cl, cp;
void printBT(string sp, string sn, my_node* v)
{
    string s;
    if (v)
    {
        s = sp;
        if (sn == cr) s[s.length() - 2] = ' ';
        printBT(s + cp, cr, v->right);

        s = s.substr(0, sp.length() - 2);
        cout << s << sn << v->number << endl;

        s = sp;
        if (sn == cl) s[s.length() - 2] = ' ';
        printBT(s + cp, cl, v->left);
    }
}
void Show_Tree(my_node* obj)
{
    cout << endl;
    printBT("", "", obj);
    cout << endl;
}
void printBT_comparator(string sp, string sn, my_node* v)
{
    string s;
    if (v)
    {
        s = sp;
        if (sn == cr) s[s.length() - 2] = ' ';
        printBT_comparator(s + cp, cr, v->right);

        s = s.substr(0, sp.length() - 2);
        cout << s << sn << v->circle_position.x << " "<< v->circle_position.y << endl;

        s = sp;
        if (sn == cl) s[s.length() - 2] = ' ';
        printBT_comparator(s + cp, cl, v->left);
    }
}
void Show_Tree_comparator(my_node* obj)
{
    cout << endl;
    printBT_comparator("", "", obj);
    cout << endl;
}


int main()
{
    srand(time(NULL));

    // for tree show
    cr = cl = cp = "  ";
    cr[0] = 218; cr[1] = 196;
    cl[0] = 192; cl[1] = 196;
    cp[0] = 179;

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "C++ is the best");
    window.setFramerateLimit(69);

    // wczeœniejsze z pliku
    /*
    My_operations o("from_file", LinesStrip);
    VertexArray from_file = o.construct_lines_from_file("input.txt");
    list_of_points.push_front(o.construct_lines_from_file("input.txt"));
    */

    // Napis
    sf::Text text;
    sf::Font font;
    font.loadFromFile("./raleway/Raleway-Italic.ttf");

    text.setFont(font);
    string input = "";
    input += "1. new points\n- right to add\n- left to connect\n- middle to add special point\n- c to close connections\n";
    input += "2. select and move\n";
    input += "S Mesh\n";
    input += "Space Lines from all points to special point\n";
    input += "D distances to all from special point\n";    
    input += "\nR clear\n";

    text.setString(input);
    text.setCharacterSize(20); // in pixels, not points!
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    // ~Napis

    // properties
    int chosen = 1;
    bool connecting_activated = true;
    int circle_moving_speed = 3;
    // ~properties    

    // signal circle
    CircleShape signal_circle;
    signal_circle.setRadius(15);
    if (connecting_activated) signal_circle.setFillColor(sf::Color::Green);
    else signal_circle.setFillColor(sf::Color::Red);
    signal_circle.setPosition(150, 0);
    // ~signal circle

    // Mesh
    int x_slices, y_slices;
    x_slices = 15;
    y_slices = x_slices;
    // y_slices = 10;
    // ~Mesh

    // used often
    Operations o;    
    Vector2i previous_mouse_position = (Vector2i(0, 0));
        
    list<CircleShape*> list_circle_points;
    CircleShape* p_chosen_circle_for_moving = nullptr;
    CircleShape* p_distance_calculation_circle = nullptr;

    list<VertexArray> list_mesh_blocks;
    VertexArray* tab_VertexArray_points_in_circle = new VertexArray[VERTEX_ARRAY_SIZE];
    // [0] - connecting circles    
    // [1] - lines to all points from dcl
    list<Text> list_length_of_distances_dcl;

    my_Tree* p_main_tree = nullptr;
    // ~used often

    // content on off
    bool pass_mesh = false;
    bool pass_connecting_points = false;
    bool pass_lines_to_all = false;
    bool pass_distance_to_all = false;
    // ~content on off

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            // Modes
            switch (chosen)
            {
                // tworzenie punktów
                case 1:
                {
                    Vector2i localPosition = sf::Mouse::getPosition(window);

                    // nowy punkt
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (!(localPosition.x < 0 || localPosition.x > WIDTH || localPosition.y < 0 || localPosition.y > HEIGHT))
                        {
                            o.All_Points_Green(list_circle_points);

                            // CircleShape circle;
                            // circle.setRadius(CIRCLE_RAD);
                            // circle.setFillColor(sf::Color::Red);
                            // circle.setPosition(localPosition.x - CIRCLE_RAD, localPosition.y - CIRCLE_RAD);

                            CircleShape* circle = new CircleShape;
                            circle->setRadius(CIRCLE_RAD);
                            circle->setFillColor(sf::Color::Red);
                            circle->setPosition(localPosition.x - CIRCLE_RAD, localPosition.y - CIRCLE_RAD);

                            // dodanie do listy                            
                            list_circle_points.push_back(circle);


                            // Recalculating //
                            {
                                if (pass_mesh) o.Recalculate_Mesh(list_mesh_blocks, list_circle_points, x_slices, y_slices);
                                list_length_of_distances_dcl.clear();
                                if (pass_lines_to_all && p_distance_calculation_circle != nullptr) o.Recalculate_set_VertexArray_for_distance_circle_lines(list_circle_points, tab_VertexArray_points_in_circle, p_distance_calculation_circle);
                                if (pass_distance_to_all && p_distance_calculation_circle != nullptr) o.Recalcutale_set_text_distance_circle(list_length_of_distances_dcl, list_circle_points, p_distance_calculation_circle, font);
                            }

                            SLEEP(100);
                        }
                    }

                    // ³aczymy kó³ka liniami biegn¹cymi do œrodka kó³ka, czyœci vertex_list i wstawia tam nowy VertexArray z wykalkulowanymi œrokami kó³ek
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        if (!pass_connecting_points)
                        {
                            pass_connecting_points = true;
                            int list_size = distance(list_circle_points.begin(), list_circle_points.end());
                            if (list_size > 0)
                            {
                                if (connecting_activated) list_size++;

                                VertexArray lines(sf::LinesStrip, list_size);
                                int i = 0;
                                Vector2f tmp;
                                float rad;
                                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                                {
                                    rad = (*it)->getRadius();
                                    tmp = (*it)->getPosition();
                                    tmp.x = tmp.x + rad;
                                    tmp.y = tmp.y + rad;

                                    lines[i] = tmp;
                                    i++;
                                }
                                if (connecting_activated)
                                {
                                    rad = (*list_circle_points.begin())->getRadius();
                                    tmp = (*list_circle_points.begin())->getPosition();
                                    tmp.x = tmp.x + rad;
                                    tmp.y = tmp.y + rad;

                                    lines[list_size - 1] = tmp;
                                }

                                tab_VertexArray_points_in_circle[0] = lines;
                            }
                            SLEEP();
                        }
                        else
                        {
                            pass_connecting_points = false;
                            SLEEP();
                        }
                    }

                    // punkt do znalezienia najbli¿szego z innych
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
                    {
                        if (!(localPosition.x < 0 || localPosition.x > WIDTH || localPosition.y < 0 || localPosition.y > HEIGHT))
                        {                            
                            o.All_Points_Green(list_circle_points);

                            if (p_distance_calculation_circle != nullptr) 
                                delete p_distance_calculation_circle;

                            CircleShape* circle = new CircleShape();
                            circle->setRadius(CIRCLE_RAD);
                            circle->setFillColor(sf::Color::Yellow);
                            circle->setPosition(localPosition.x - CIRCLE_RAD, localPosition.y - CIRCLE_RAD);

                            p_distance_calculation_circle = circle;

                            // Recalculating //
                            {
                                list_length_of_distances_dcl.clear();
                                if (pass_lines_to_all && p_distance_calculation_circle != nullptr) o.Recalculate_set_VertexArray_for_distance_circle_lines(list_circle_points, tab_VertexArray_points_in_circle, p_distance_calculation_circle);
                                if (pass_distance_to_all && p_distance_calculation_circle != nullptr) o.Recalcutale_set_text_distance_circle(list_length_of_distances_dcl, list_circle_points, p_distance_calculation_circle, font);
                            }

                            SLEEP();
                        }
                    }

                    break;
                }

                // poruszanie punktów strza³kami i myszk¹
                case 2:
                {
                    Vector2i previous = previous_mouse_position;

                    Vector2i localPosition = sf::Mouse::getPosition(window);
                    previous_mouse_position = localPosition;


                    // prawym zaznaczasz punkt
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        Vector2f tmp;
                        float radious;
                        float x, y;

                        // znalaz³ ku³ko o które nam chodzi
                        for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                        {
                            tmp = (*it)->getPosition();
                            radious = (*it)->getRadius();
                            x = tmp.x + radious / 2;
                            y = tmp.y + radious / 2;

                            if ((float)radious >= o.Get_length(localPosition.x, localPosition.y, x, y)) p_chosen_circle_for_moving = *it;
                        }
                    }

                    // lewym nim ruszacza jeœli kursor jest w jego zasiêgu
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (p_chosen_circle_for_moving != nullptr)
                        {
                            Vector2f tmp = p_chosen_circle_for_moving->getPosition();

                            //LOG("Pressed ");
                            //LOG(o.Get_length(localPosition.x, localPosition.y, tmp.x, tmp.y));
                            //if (p_chosen_circle_for_moving->getRadius() * 2 >= Get_length(localPosition.x, localPosition.y, tmp.x + p_chosen_circle_for_moving->getRadius(), tmp.y + p_chosen_circle_for_moving->getRadius()))
                            //{
                                //LOG("Pressed in the middle of a circle");                                

                            Vector2f circle_position;
                            if (p_chosen_circle_for_moving != nullptr)
                            {
                                circle_position = p_chosen_circle_for_moving->getPosition();
                                circle_position.x += (localPosition.x - previous.x); //LOG("x: " + (localPosition.x - previous.x));
                                circle_position.y += (localPosition.y - previous.y); //LOG("x: " + (localPosition.y - previous.y));

                                p_chosen_circle_for_moving->setPosition(circle_position);
                            }

                            o.VertexArray_based_on_existing_circle_list(list_circle_points, tab_VertexArray_points_in_circle, sf::LinesStrip, connecting_activated);
                        }
                    }

                    /*
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        Vector2f tmp;
                        float radious;
                        float x, y;

                        // znalaz³ ku³ko o które nam chodzi
                        for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                        {
                            tmp = (*it)->getPosition();
                            radious = (*it)->getRadius();
                            x = tmp.x + radious / 2;
                            y = tmp.y + radious / 2;

                            if ((float)radious >= o.Get_length(localPosition.x, localPosition.y, x, y)) p_chosen_circle_for_moving = *it;
                        }
                    }

                    // usuwanie chosen_point EMPTY
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {

                    }
                    */

                    // zmiana pozycji p_chosen_circle_for_moving + rekalkulacja vertex_list i VertexArray_tab
                    if ((p_chosen_circle_for_moving != nullptr) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                        || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
                    {
                        // tutaj dynamiczne obliczanie coordinet_list
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                        {
                            LOG("up");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.y -= circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                        {
                            LOG("down");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.y += circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                        {
                            LOG("left");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.x -= circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }
                        else
                        {
                            LOG("right");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.x += circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }

                        o.VertexArray_based_on_existing_circle_list(list_circle_points, tab_VertexArray_points_in_circle, sf::LinesStrip, connecting_activated);
                    }


                    // Recalculating //
                    {
                        if (pass_mesh) o.Recalculate_Mesh(list_mesh_blocks, list_circle_points, x_slices, y_slices);
                        list_length_of_distances_dcl.clear();
                        if (pass_lines_to_all && p_distance_calculation_circle != nullptr) o.Recalculate_set_VertexArray_for_distance_circle_lines(list_circle_points, tab_VertexArray_points_in_circle, p_distance_calculation_circle);
                        if (pass_distance_to_all && p_distance_calculation_circle != nullptr) o.Recalcutale_set_text_distance_circle(list_length_of_distances_dcl, list_circle_points, p_distance_calculation_circle, font);
                    }

                    break;
                }

                // empty
                case 3:
                {
                    // wczeœniejsze ustawienia do poruszania myszk¹
                    /*
                    Vector2i previous = previous_mouse_position;

                    Vector2i localPosition = sf::Mouse::getPosition(window);
                    previous_mouse_position = localPosition;


                    // prawym zaznaczasz punkt
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        Vector2f tmp;
                        float radious;
                        float x, y;

                        // znalaz³ ku³ko o które nam chodzi
                        for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                        {
                            tmp = (*it)->getPosition();
                            radious = (*it)->getRadius();
                            x = tmp.x + radious / 2;
                            y = tmp.y + radious / 2;

                            if ((float)radious >= o.Get_length(localPosition.x, localPosition.y, x, y)) p_chosen_circle_for_moving = *it;
                        }
                    }

                    // lewym nim ruszacza jeœli kursor jest w jego zasiêgu
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (p_chosen_circle_for_moving != nullptr)
                        {
                            Vector2f tmp = p_chosen_circle_for_moving->getPosition();

                            //LOG("Pressed ");
                            //LOG(o.Get_length(localPosition.x, localPosition.y, tmp.x, tmp.y));
                            //if (p_chosen_circle_for_moving->getRadius() * 2 >= Get_length(localPosition.x, localPosition.y, tmp.x + p_chosen_circle_for_moving->getRadius(), tmp.y + p_chosen_circle_for_moving->getRadius()))
                            //{
                                //LOG("Pressed in the middle of a circle");

                            Vector2f circle_position;
                            if (p_chosen_circle_for_moving != nullptr)
                            {
                                circle_position = p_chosen_circle_for_moving->getPosition();
                                circle_position.x += (localPosition.x - previous.x); //LOG("x: " + (localPosition.x - previous.x));
                                circle_position.y += (localPosition.y - previous.y); //LOG("x: " + (localPosition.y - previous.y));

                                p_chosen_circle_for_moving->setPosition(circle_position);
                            }

                            o.VertexArray_based_on_existing_circle_list(list_circle_points, tab_VertexArray_points_in_circle, sf::LinesStrip, connecting_activated);
                        }
                    }
                    */

                    // wczeœniejsze porusznie bez aktualizacji punków
                    /*
                    Vector2i previous = previous_mouse_position;

                    Vector2i localPosition = sf::Mouse::getPosition(window);
                    previous_mouse_position = localPosition;

                    // prawym zaznaczasz punkt
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        Vector2f tmp;
                        float radious;
                        float x, y;

                        // znalaz³ ku³ko o które nam chodzi
                        for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                        {
                            tmp = (*it)->getPosition();
                            radious = (*it)->getRadius();
                            x = tmp.x + radious / 2;
                            y = tmp.y + radious / 2;

                            if ((float)radious >= o.Get_length(localPosition.x, localPosition.y, x, y)) p_chosen_circle_for_moving = *it;
                        }
                    }

                    // lewym nim ruszacza jeœli kursor jest w jego zasiêgu
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (p_chosen_circle_for_moving != nullptr)
                        {
                            Vector2f tmp = p_chosen_circle_for_moving->getPosition();

                            //LOG("Pressed ");
                            //LOG(o.Get_length(localPosition.x, localPosition.y, tmp.x, tmp.y));
                            //if (p_chosen_circle_for_moving->getRadius() * 2 >= Get_length(localPosition.x, localPosition.y, tmp.x + p_chosen_circle_for_moving->getRadius(), tmp.y + p_chosen_circle_for_moving->getRadius()))
                            //{
                                //LOG("Pressed in the middle of a circle");                                

                            Vector2f circle_position;
                            if (p_chosen_circle_for_moving != nullptr)
                            {
                                circle_position = p_chosen_circle_for_moving->getPosition();
                                circle_position.x += (localPosition.x - previous.x); //LOG("x: " + (localPosition.x - previous.x));
                                circle_position.y += (localPosition.y - previous.y); //LOG("x: " + (localPosition.y - previous.y));

                                p_chosen_circle_for_moving->setPosition(circle_position);
                            }

                            // czyszczenia siatki
                            delete[] tab_VertexArray_points_in_circle;
                            tab_VertexArray_points_in_circle = new VertexArray[VERTEX_ARRAY_SIZE];
                            //o.VertexArray_based_on_existing_circle_list(list_circle_points, tab_VertexArray_points_in_circle, sf::LinesStrip, connecting_activated);
                        }
                    }
                    */

                    break;
                }

                // to check mouse coordinet
                case 4:
                {
                    Vector2i localPosition = sf::Mouse::getPosition(window);
                    LOG(localPosition.x << ". " << localPosition.y);

                    break;
                }

                default:
                {
                    break;
                }
            }


            // clear
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                    delete* it;
                list_circle_points.clear();
                list_length_of_distances_dcl.clear();

                list_mesh_blocks.clear();

                delete[] tab_VertexArray_points_in_circle;
                tab_VertexArray_points_in_circle = new VertexArray[VERTEX_ARRAY_SIZE];

                p_chosen_circle_for_moving = nullptr;
                if (p_distance_calculation_circle != nullptr) delete p_distance_calculation_circle;
                p_distance_calculation_circle = nullptr;

                if (p_main_tree != nullptr)
                {
                    p_main_tree->Clear();
                    delete p_main_tree;
                    p_main_tree = nullptr;
                }
            }
            // connect
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            {
                if (connecting_activated)
                {
                    connecting_activated = false;
                    signal_circle.setFillColor(sf::Color::Red);
                }
                else
                {
                    connecting_activated = true;
                    signal_circle.setFillColor(sf::Color::Green);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            // system("cls");
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                system("cls");
            }
            // nearest neighbour            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                if (p_distance_calculation_circle)
                {
                    #ifdef MY_DEBUG
                    system("cls");
                    #endif

                    // wszystkie kó³ka na zielono
                    for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                    {
                        (*it)->setFillColor(Color::Green);
                    }

                    // jeœli coœ by³o wczeœniej to kasujemy to
                    if (p_main_tree != nullptr)
                    {
                        p_main_tree->Clear();
                        delete p_main_tree;
                        p_main_tree = nullptr;
                    }

                    // tworzymy nowe i dodajemy z circle_list
                    p_main_tree = new my_Tree();
                    int element = 1;
                    for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                    {
                        p_main_tree->Add(*it);
                        LOG(element << "added");
                        element++;
                    }
                    cout << "Tree build" << endl;

                    LOG("Q Pressed - Searching for nearest neighbour");
                    LOG("calculation circle position: " << o.Middle_of_a_circle(*p_distance_calculation_circle).x << ". " << o.Middle_of_a_circle(*p_distance_calculation_circle).y);

                    CircleShape* tmp = p_main_tree->Return_pointer_to_closest_circle(
                        (o.Middle_of_a_circle(*p_distance_calculation_circle)));
                    LOG("Function worked");

                    // ustawienie koloru i wypisanie wszystkich wczeœniejszych wêz³ów
                    if (tmp != nullptr)
                    {
                        int i = 1;
                        LOG("iterating thourgh list_circle_points");
                        for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                        {
                            LOG(i << " adress:   " << *it << " " << (*it)->getPosition().x + (*it)->getRadius() << ". " << (*it)->getPosition().y + (*it)->getRadius());
                            i++;
                        }

                        LOG("");
                        LOG("tmp adress: " << tmp << " " << tmp->getPosition().x + (tmp)->getRadius() << ". " << tmp->getPosition().y + (tmp)->getRadius());

                        tmp->setFillColor(Color::Magenta);
                        LOG("Color set");
                    }
                    else LOG("RETURNED NULL");

                    SLEEP(100);
                }
            }
            // Lab 5 - siatka prostok¹tna
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                if (!pass_mesh && !list_circle_points.empty())
                {
                    pass_mesh = true;
                    o.Recalculate_Mesh(list_mesh_blocks, list_circle_points, x_slices, y_slices);

                    LOG("Mesh complete");
                    SLEEP(100);
                }
                else
                {
                    pass_mesh = false;
                    SLEEP(100);
                }
            }
            // Lines to all points
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (p_distance_calculation_circle)
                {
                    if (!pass_lines_to_all)
                    {
                        pass_lines_to_all = true;

                        o.Recalculate_set_VertexArray_for_distance_circle_lines
                        (list_circle_points, tab_VertexArray_points_in_circle, p_distance_calculation_circle);

                        SLEEP(100);
                    }
                    else
                    {
                        pass_lines_to_all = false;
                        SLEEP(100);
                    }
                }
            }
            // Distances to all points
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                if (p_distance_calculation_circle)
                {
                    if (!pass_distance_to_all)
                    {
                        pass_distance_to_all = true;

                        o.Recalcutale_set_text_distance_circle(list_length_of_distances_dcl, list_circle_points, 
                            p_distance_calculation_circle , font);

                        SLEEP(100);
                    }
                    else
                    {
                        pass_distance_to_all = false;
                        list_length_of_distances_dcl.clear();
                        SLEEP(100);
                    }
                }
            }


            // DEBUG
            // Wspó³rzêdne kó³ek z listy
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            {
                LOG("Circle poitns from list");
                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++) LOG(o.Middle_of_a_circle(*it).x << ". " << o.Middle_of_a_circle(*it).y);
                SLEEP(200);
            }
            // adresy kó³ek z drzewa
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            {
                if (p_main_tree != nullptr)
                {
                    LOG("Tree structure");
                    p_main_tree->Show_tree_structure();

                    Show_Tree(p_main_tree->Get_root());
                    Show_Tree_comparator(p_main_tree->Get_root());
                    SLEEP(200);
                }
            }
            // ~DEBUG


            // Modes
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                chosen = 1;
                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++) (*it)->setFillColor(Color::Green);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                chosen = 2;
                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++) (*it)->setFillColor(Color::Green);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            {
                chosen = 3;
                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++) (*it)->setFillColor(Color::Green);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
            {
                chosen = 4;
                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++) (*it)->setFillColor(Color::Green);
            }


            // exit
            if (event.type == Event::Closed) {

                window.close();
            }
        }


        window.clear();
        {
            // p_distance_calculation_circle - Yellow
            if (p_distance_calculation_circle != nullptr) 
                window.draw(*p_distance_calculation_circle);

            // point inside circles
            if (pass_connecting_points) 
                window.draw(tab_VertexArray_points_in_circle[0]);

            // lines to all points
            if (pass_lines_to_all && tab_VertexArray_points_in_circle[1].getVertexCount() != 0)                
                window.draw(tab_VertexArray_points_in_circle[1]);

            // distances to all points list_length_of_distances_dcl
            if (pass_distance_to_all) 
                for (list<Text>::iterator it = list_length_of_distances_dcl.begin(); it != list_length_of_distances_dcl.end(); it++)
                    window.draw(*it);

            // mesh lines
            if (pass_mesh) 
                for (list<VertexArray>::iterator it = list_mesh_blocks.begin(); it != list_mesh_blocks.end(); it++) 
                    window.draw(*it);

            // menu staff + circles
            {
                // circles
                int c = 1;
                for (list<CircleShape*>::iterator it = list_circle_points.begin(); it != list_circle_points.end(); it++)
                {
                    window.draw(**it);

                    // Drawing numbers
                    {
                        Vector2f position = (*it)->getPosition();
                        position = o.Middle_of_a_circle(*it);
                        position.x += 3;
                        position.y += 3;

                        sf::Text text_numbers_of_points;
                        text_numbers_of_points.setFont(font);
                        text_numbers_of_points.setString(to_string(c));
                        text_numbers_of_points.setCharacterSize(20); // in pixels, not points!
                        text_numbers_of_points.setFillColor(sf::Color::White);
                        //text_numbers_of_points.setStyle(sf::Text::Bold);
                        text_numbers_of_points.setPosition(position);
                        window.draw(text_numbers_of_points);

                        c++;
                    }
                }

                window.draw(text);
                window.draw(signal_circle);
            }
        }
        window.display();
    }

    return 0;
}
