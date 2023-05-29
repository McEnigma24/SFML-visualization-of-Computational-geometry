// change the target to x64
// Project -> Properties (last on the list)
// Configurations: Debug -> Debugging -> Environment -> PATH=S:\Programy Visual Studio\aaa SFML - for Visualization\SFML-2.5.1\bin
// PATH= gdziekolwiek jest folder bin plików SFML
// apply -> ok
#include "Header.h"
#include "MainVariables.h"

int main()
{
    srand((unsigned int)time(NULL));

    // fajne wyœwietlanie trójk¹ta  
    /*
    {
        cout << "Is it in triangle? function" << endl;
        Vector2f tab[3];
        tab[0] = Vector2f(2, 2);
        tab[1] = Vector2f(3, 13);
        tab[2] = Vector2f(17, 6);
        Triangulation trian;
        for (float y = 1; y < 15; y += 0.25)
        {
            for (float x = 1; x < 20; x += 0.25)
            {
                Vector2f point(x, y);

                // cout << point.x << " " << point.y << endl;
                cout << trian.checkIfPointIsInTriangle(tab, point) << " ";
            }
            cout << endl;
        }
    }
    */

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "C++ is the best", sf::Style::Default, settings);
    //RenderWindow window(VideoMode(WIDTH, HEIGHT), "C++ is the best");
    window.setFramerateLimit(160);

    // Napis //
    sf::Text text;
    sf::Text chosen_mode;
    sf::Font font;
    {
        font.loadFromFile("./raleway/Raleway-Italic.ttf");
        text.setFont(font);
        string input = "";
        input += "1. new points\n- right to add\n- left to connect\n- middle to add special point\n- c to close connections\n";
        input += "2. select and move\n";

        input += "\n";
        input += "S - Mesh\n";
        input += "D - distances to all from special point\n";
        input += "Space - Lines from all points to special point\n";
        input += "\nR clear\n";

        text.setString(input);
        text.setCharacterSize(15); // in pixels, not points!
        text.setFillColor(sf::Color::White);
        text.setStyle(sf::Text::Bold);


        chosen_mode.setString("1");
        chosen_mode.setFont(font);
        chosen_mode.setCharacterSize(20); // in pixels, not points!
        chosen_mode.setFillColor(sf::Color::White);
        chosen_mode.setStyle(sf::Text::Bold);
        chosen_mode.setPosition(161, 1);
    }
    // ~Napis //

    // signal circle
    CircleShape signal_circle;
    {
        signal_circle.setRadius(15);
        signal_circle.setFillColor(sf::Color::Green);
        signal_circle.setPosition(150, 0);
    }
    // ~signal circle

    // properties //
    int chosen = 1;    
    int circle_moving_speed = 1;
    float local_rad = CIRCLE_RAD;
    Vector2i previous_mouse_position = (Vector2i(0, 0));
    // ~properties //  


    // FILE INPUT //
    const float file_rad = 10;
    string file_input_path = path::points + "points.txt";
    // ~FILE INPUT //


    // BENCHMARKING deleting previous files //    
    // Triangulation
    {
        try {
            remove(ben::trinagulation.c_str());
        }
        catch (...) {
            cout << "file error" << endl;
        }
    }
    // ~BENCHMARKING //
    

    // Main Structures //
    VertexArrayFunctions vfunc;
    Operations o;
    Recalculating recal;
    Triangulation trian;    
    //ConvexShell otoczka;

    int id_for_circle = 1;
    vector<myCircle> vec_circle_points;             vec_circle_points.reserve(125);
    CircleShape* p_chosen_circle_for_moving = nullptr;
    CircleShape* p_distance_calculation_circle = nullptr;
    CircleShape* circum_circle = nullptr;
    Triangle* circum_triangle = nullptr;

    // teraz zawsze robiæ na vector, a nie list
    list<VertexArray> list_mesh_blocks;
    list<Text> list_length_of_distances_dcl;
    VertexArray* tab_VertexArray_points_in_circle = new VertexArray[VERTEX_ARRAY_SIZE];
    // [0] - connecting circles
    // [1] - lines to all points from dcl
    // [2] - convex shell lines

    list<Triangle> list_triangulaton_triangles;

    // ~Main Structures //


    // Passing of Calculations //
    Pass Big_Pass(&vec_circle_points,
        &tab_VertexArray_points_in_circle,
        &list_mesh_blocks, x_slices, y_slices,
        &p_chosen_circle_for_moving, &p_distance_calculation_circle,
        &font, &list_length_of_distances_dcl,
        &circum_circle, &circum_triangle,
        &list_triangulaton_triangles
    );
    // ~Passing of Calculations //

    // dla siatki strukturalnej
    #if QUAD_TREE_ACTIVE == 1
        //Quad Tree serducho.txt go_.txt
        QuadTree q(path::quadtree + "siatka na projekt go 2.txt", 100, 300); // not recalculating y
        list<VertexArray> quadtree_list;
        list<QuadTree::quad_coordinets_to_file> points_from_quadtree;

        #if QUAD_SEARCHING_FOR_OPTIMAL == 1
            int optimal_quad_limit = 1;
            bool optimum_reached = false;
        #else 
            int quad_limit = 1;

            q.Start_Quadding(quad_limit);
            quadtree_list = q.getListForVisualization();
            points_from_quadtree = q.getListOfCoordinets();

            // rozrzedzamy listê
            {
                list<QuadTree::quad_coordinets_to_file> tmp;

                int index;
                int co_ = 1;

                while (points_from_quadtree.size() > 100)
                {
                    index = 0;      co_++;

                    for (auto& iti : points_from_quadtree)
                    {
                        if ((index % co_ == 0) && (iti.down_right.x - iti.down_left.x == quad_limit))  tmp.push_back(iti);
                        index++;
                    }
                    points_from_quadtree = tmp;
                    tmp.clear();
                }

                cout << points_from_quadtree.size() << " " << co_ << endl;
            }

            // dodajemy z points_from_quadtree do vec_circle_points
            {
                for (auto& quad_point : points_from_quadtree)
                {
                    myVector myvec = quad_point.up_left;    myvec.recalExisting();
                    Vector2f position = *myvec;
                    position.x += 100;
                    position.y += 300;

                    myCircle mycircle;
                    mycircle.m_id = id_for_circle;                  id_for_circle++;
                    mycircle.m_circle->setRadius(local_rad);
                    mycircle.m_circle->setFillColor(sf::Color::Red);

                    mycircle.m_circle->setPosition(position);
                    mycircle.m_circle->setPosition(mycircle.setMiddle());

                    // dodanie do listy                            
                    vec_circle_points.push_back(mycircle);
                }
            }
        #endif
    #endif

    // Fraktale //    
    {
        // rekurencja krzy¿
        InterestingVisualizations in;
        //list<VertexArray> cross_list = in.Recursion_with_Cross_in_Rectangle(500, 500, 250, 250, 4);

        //list<VertexArray> cross_list = in.Recursion_with_Cross_in_Rectangle(500, 500, 250, 250, 1, 1, 1, 1);
        list<VertexArray> cross_list = in.Recursion_with_Cross_in_Rectangle(500, 500, 250, 250, 1, 5, 5, 1);
        // nak³adanie jednego na drugie
        //list<VertexArray> tmp_opposite_site = in.Recursion_with_Cross_in_Rectangle(500, 500, 250, 250, 5, 1, 1, 5);
        //for (auto& op : tmp_opposite_site) cross_list.push_back(op);

        // Triangle for recursions
        Triangle t(*myVector(0, 0, true), *myVector(500, 1000, true), *myVector(1000, 0, true));
        offSet off(100, 100);
        //list<VertexArray> triangle_list = in.RecursionWithTriangles(t, off, 1);

        int whole = 2;
        list<VertexArray> triangle_list = in.RecursionWithTriangles(t, off, whole, whole, whole, 10);
    }
    // ~Fraktale //
    
    // Linia next point
    /*
    Line linaaa(Vector2f(500, 700), Vector2f(800, 900));

    Line laaaaaa(Vector2f(500, 700), Vector2f(300, 900));
    Vector2f randy = laaaaaa.NextPointAfter(laaaaaa.MiddleOfLine(), 0.5, true);    
    */

    // Triangulation
    /*
    list<Triangle> tr = trian.triangulationDelaunay(vec_circle_points);
    cout << "done" << endl;
    cout << tr.size() << endl;


    int iiiii = 1;
    for (auto& t : tr)
    {
        cout << iiiii << endl; iiiii++;
        t.infoAboutPosition();
    }
    */





    // Rolling thought Big_Pass bools //
    vector<std::reference_wrapper<bool>> rolling_bools;
    {
        // Properties //
        rolling_bools.push_back(Big_Pass.prop_drawing_circles);          // [0]        
        rolling_bools.push_back(Big_Pass.prop_show_menu);                // [1]
        rolling_bools.push_back(Big_Pass.prop_connecting_activated);     // [2]
        rolling_bools.push_back(Big_Pass.prop_random_point_movement);    // [3]

        // Passes //
        rolling_bools.push_back(Big_Pass.pass_mesh);                     // [4]
        rolling_bools.push_back(Big_Pass.pass_connecting_points);        // [5]
        rolling_bools.push_back(Big_Pass.pass_lines_to_all);             // [6]
        rolling_bools.push_back(Big_Pass.pass_distance_to_all);          // [7]
        rolling_bools.push_back(Big_Pass.pass_closest_point);            // [8]
        rolling_bools.push_back(Big_Pass.pass_quadtree);                 // [9]
        rolling_bools.push_back(Big_Pass.pass_convex_shell);             // [10]
        rolling_bools.push_back(Big_Pass.pass_circum_circle);            // [11]
        rolling_bools.push_back(Big_Pass.pass_triangulation);            // [12]        
    }
    
    vector<vector<int>> custom_configurations;
    // konfiguracje - numery indexów boolów, które zostan¹ w³¹czone
    {
        // pierwsza konfiguracja
        if (1)
        {
            custom_configurations.push_back({ 9 });         // quadtree
            // save quadtree //
            custom_configurations.push_back({ 0, 9 });      // quadtree + draw_points
            custom_configurations.push_back({ 0 });         // draw_points
            custom_configurations.push_back({ 0, 12 });     // draw_points + triangulation
            custom_configurations.push_back({ 12 });        // triangulation
            // save triangulation //
        }

        // inna konfiguracja
        if (0)
        {

        }


        rolling_bools::confing_upper_limit = static_cast<int>(custom_configurations.size()) - 1;
    }

    // Pre-Set //
    if (false)
    {
        Big_Pass.pass_quadtree = true;
        Big_Pass.prop_show_menu = false;
        Big_Pass.prop_drawing_circles = false;
    }
    // applying first configuration
    else if(!custom_configurations.empty())
    {
        vector<int>& configuration = custom_configurations[0];
        {
            // wy³¹czenie wszystkich pozosta³ych
            for (auto& r : rolling_bools)    r.get() = false;

            // w³¹czanie wybranych
            for (auto& pass : configuration)
            {
                rolling_bools[pass].get() = true;

                switch (pass)
                {
                case 4:
                {
                    Big_Pass.tokenMesh();
                    break;
                }

                case 5:
                {
                    Big_Pass.tokenLines();
                    break;
                }

                case 6:
                {
                    Big_Pass.tokenDistanceLine();
                    break;
                }

                case 7:
                {
                    Big_Pass.tokenDistanceText();
                    break;
                }

                case 8:
                {

                    break;
                }

                case 9:
                {

                    break;
                }

                case 10:
                {
                    Big_Pass.tokenConvexShell();
                    break;
                }

                case 11:
                {
                    if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                    break;
                }

                case 12:
                {
                    if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                    break;
                }


                default:
                    break;
                }
            }
        }
    }

    while (window.isOpen())
    {
        Event event;
        window.pollEvent(event);
        //while (window.pollEvent(event))
        //{
            // Modes
            switch (chosen)
            {
                // tworzenie punktów
                case 1:
                {
                    Vector2i localPosition = sf::Mouse::getPosition(window);

                    // nowy punkt                                                   // ---> NEW RECALCULATION <--- //
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (!(localPosition.x < 0 || localPosition.x > WIDTH || localPosition.y < 0 || localPosition.y > HEIGHT))
                        {
                            o.All_Points_Green(vec_circle_points);

                            // CircleShape circle;
                            // circle.setRadius(local_rad);
                            // circle.setFillColor(sf::Color::Red);
                            // circle.setPosition(localPosition.x - local_rad, localPosition.y - local_rad);

                            myCircle mycircle;
                            mycircle.m_id = id_for_circle;                  id_for_circle++;
                            mycircle.m_circle->setRadius(local_rad);
                            mycircle.m_circle->setFillColor(sf::Color::Red);
                            mycircle.m_circle->setPosition(localPosition.x - local_rad, localPosition.y - local_rad);

                            // dodanie do listy                            
                            vec_circle_points.push_back(mycircle);


                            // Passes //
                            {
                                Big_Pass.tokenMesh();
                                Big_Pass.tokenLines();
                                Big_Pass.tokenDistanceLine();
                                Big_Pass.tokenDistanceText();
                                Big_Pass.tokenConvexShell();
                                if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                                if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                            }

                            SLEEP(100);
                        }
                    }

                    // ³aczymy kó³ka liniami biegn¹cymi do œrodka kó³ka, czyœci vertex_list i wstawia tam nowy VertexArray z wykalkulowanymi œrokami kó³ek
                    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    {
                        if (!Big_Pass.pass_connecting_points)
                        {
                            Big_Pass.pass_connecting_points = true;
                            int list_size = (int)distance(vec_circle_points.begin(), vec_circle_points.end());
                            if (list_size > 0)
                            {
                                if (Big_Pass.prop_connecting_activated) list_size++;

                                VertexArray lines(sf::LinesStrip, list_size);
                                int i = 0;
                                Vector2f tmp;
                                float rad;

                                for (auto& it : vec_circle_points)
                                {
                                    rad = it.m_circle->getRadius();
                                    tmp = it.m_circle->getPosition();
                                    tmp.x = tmp.x + rad;
                                    tmp.y = tmp.y + rad;

                                    lines[i] = tmp;
                                    i++;
                                }
                                if (Big_Pass.prop_connecting_activated)
                                {
                                    rad = (*vec_circle_points.begin()).m_circle->getRadius();
                                    tmp = (*vec_circle_points.begin()).m_circle->getPosition();
                                    tmp.x = tmp.x + rad;
                                    tmp.y = tmp.y + rad;

                                    lines[list_size - 1] = tmp;
                                }

                                tab_VertexArray_points_in_circle[0] = lines;
                            }
                            SLEEP(100);
                        }
                        else
                        {
                            Big_Pass.pass_connecting_points = false;
                            SLEEP(100);
                        }
                    }

                    // punkt do znalezienia najbli¿szego z innych
                    else if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
                    {
                        if (!(localPosition.x < 0 || localPosition.x > WIDTH || localPosition.y < 0 || localPosition.y > HEIGHT))
                        {
                            o.All_Points_Green(vec_circle_points);

                            if (p_distance_calculation_circle != nullptr)
                                delete p_distance_calculation_circle;

                            CircleShape* circle = new CircleShape();
                            circle->setRadius(local_rad);
                            circle->setFillColor(sf::Color::Yellow);
                            circle->setPosition(localPosition.x - local_rad, localPosition.y - local_rad);

                            p_distance_calculation_circle = circle;                            

                            // Passes //
                            {
                                // Nie potrzebne rekalkulacje                                
                                
                                Big_Pass.tokenDistanceLine();
                                Big_Pass.tokenDistanceText();                                
                            }

                            //SLEEP(100);
                        }
                    }

                    // zmieniamy konfiguracje
                    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)))
                    {
                        if (!custom_configurations.empty())
                        {
                            cout << rolling_bools::chosen_configuration_index << " -> ";

                            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))               rolling_bools::chosen_configuration_index++;
                            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))        rolling_bools::chosen_configuration_index--;


                            if (rolling_bools::chosen_configuration_index == rolling_bools::confing_lower_limit - 1)
                                rolling_bools::chosen_configuration_index = rolling_bools::confing_upper_limit;

                            else if (rolling_bools::chosen_configuration_index == rolling_bools::confing_upper_limit + 1)
                                rolling_bools::chosen_configuration_index = rolling_bools::confing_lower_limit;

                            cout << rolling_bools::chosen_configuration_index << endl;

                            SLEEP(200);
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
                        for (auto& it : vec_circle_points)
                        {
                            tmp = it.m_circle->getPosition();
                            radious = it.m_circle->getRadius();
                            x = tmp.x + radious / 2;
                            y = tmp.y + radious / 2;

                            if ((float)radious >= o.Get_length((float)localPosition.x, (float)localPosition.y, x, y)) p_chosen_circle_for_moving = &*it.m_circle;
                        }
                    }
                    
                    // lewym nim rusza                                               // ---> NEW RECALCULATION <--- //
                    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if (p_chosen_circle_for_moving != nullptr)
                        {
                            Vector2f tmp = p_chosen_circle_for_moving->getPosition();

                            Vector2f circle_position;
                            if (p_chosen_circle_for_moving != nullptr)
                            {
                                circle_position = p_chosen_circle_for_moving->getPosition();
                                circle_position.x += (localPosition.x - previous.x);
                                circle_position.y += (localPosition.y - previous.y);

                                p_chosen_circle_for_moving->setPosition(circle_position);
                            }

                            // Passes //
                            {
                                Big_Pass.tokenMesh();
                                Big_Pass.tokenLines();
                                Big_Pass.tokenDistanceLine();
                                Big_Pass.tokenDistanceText();
                                Big_Pass.tokenConvexShell();
                                if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                                if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                            }
                        }
                    }
                    
                    // zmiana rozmiarów punktów                                     // ---> NEW RECALCULATION <--- //
                    else if (event.type == sf::Event::MouseWheelMoved)
                    {
                        // LOG("delta: " << event.mouseWheel.delta);
                        if (event.mouseWheel.delta > 0) local_rad++;
                        else local_rad--;

                        // Circle update
                        for (auto& it : vec_circle_points)
                            it.m_circle->setRadius(local_rad);
                        if (p_distance_calculation_circle) (p_distance_calculation_circle)->setRadius(local_rad);

                        // Passes //
                        {
                            Big_Pass.tokenMesh();
                            Big_Pass.tokenLines();
                            Big_Pass.tokenDistanceLine();
                            Big_Pass.tokenDistanceText();
                            Big_Pass.tokenConvexShell();
                            if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                            if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                        }
                    }
                                                                                    // ---> NEW RECALCULATION <--- //
                    // zmiana pozycji p_chosen_circle_for_moving + rekalkulacja vertex_list i VertexArray_tab
                    else if ((p_chosen_circle_for_moving != nullptr) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                        || sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
                    {
                        // tutaj dynamiczne obliczanie coordinet_list
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                        {
                            //LOG("up");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.y -= circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                        {
                            //LOG("down");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.y += circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }
                        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                        {
                            //LOG("left");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.x -= circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }
                        else
                        {
                            //LOG("right");

                            Vector2f previous;
                            previous = p_chosen_circle_for_moving->getPosition();
                            previous.x += circle_moving_speed;
                            p_chosen_circle_for_moving->setPosition(previous);
                        }

                        // Passes //
                        {
                            Big_Pass.tokenMesh();
                            Big_Pass.tokenLines();
                            Big_Pass.tokenDistanceLine();
                            Big_Pass.tokenDistanceText();
                            Big_Pass.tokenConvexShell();
                            if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                            if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                        }
                    }

                    break;
                }

                // empty //
                case 3:
                {

                    break;
                }

                // All recursion stuff // Triangulations
                case 4:
                {

                    break;
                }

                // Track mouse movement
                case 5:
                {
                    Vector2i localPosition = sf::Mouse::getPosition(window);
                    cout << localPosition.x << " " << localPosition.y << endl;
                    break;
                }

                default:
                {
                    break;
                }
            }

            // Apply Configuration switch //
            if(rolling_bools::previously_chosen_configuration_index != rolling_bools::chosen_configuration_index)
            {
                vector<int>& configuration = custom_configurations[rolling_bools::chosen_configuration_index];
                {
                    // wy³¹czenie wszystkich pozosta³ych
                    for (auto& r : rolling_bools)    r.get() = false;

                    // w³¹czanie wybranych
                    for (auto& pass : configuration)
                    {
                        rolling_bools[pass].get() = true;

                        switch (pass)
                        {
                            case 4:
                            {
                                Big_Pass.tokenMesh();
                                break;
                            }

                            case 5:
                            {
                                Big_Pass.tokenLines();
                                break;
                            }

                            case 6:
                            {
                                Big_Pass.tokenDistanceLine();
                                break;
                            }

                            case 7:
                            {
                                Big_Pass.tokenDistanceText();
                                break;
                            }

                            case 8:
                            {

                                break;
                            }

                            case 9:
                            {

                                break;
                            }

                            case 10:
                            {
                                Big_Pass.tokenConvexShell();
                                break;
                            }

                            case 11:
                            {
                                if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                                break;
                            }

                            case 12:
                            {
                                if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                                break;
                            }


                            default:
                                break;
                        }
                    }
                }

                rolling_bools::previously_chosen_configuration_index = rolling_bools::chosen_configuration_index;
            }



            // Properties //
            if(1)
            {
                // drawing circles
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                {
                    if (!Big_Pass.prop_drawing_circles)
                        Big_Pass.prop_drawing_circles = true;
                    else
                        Big_Pass.prop_drawing_circles = false;

                    SLEEP(100);
                }
                // change for // only number // number with coordinets // only coordinets // no info at all
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                {
                    Big_Pass.prop_drawing_coordinets++;
                    Big_Pass.prop_drawing_coordinets %= 4;

                    SLEEP(100);
                }

                // show menu
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    if (!Big_Pass.prop_show_menu)
                        Big_Pass.prop_show_menu = true;
                    else
                        Big_Pass.prop_show_menu = false;

                    SLEEP(100);
                }
                // clear
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    CLEAR();
                }
                // system("cls");
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
                {
                    system("cls");
                }
            }

            // Visualizations //                - - - dorobiæ Save Mesh i Save Triangulation
            if (1)
            {
                // add file input points
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                {
                    list<myCircle> file_input = vfunc.loadFileInputPoints(file_input_path, file_rad);
                    for (auto& f : file_input)
                    {
                        f.m_id = id_for_circle;         id_for_circle++;
                        vec_circle_points.push_back(f);
                    }

                    // Passes //
                    {
                        Big_Pass.tokenMesh();
                        Big_Pass.tokenLines();
                        Big_Pass.tokenDistanceLine();
                        Big_Pass.tokenDistanceText();
                        Big_Pass.tokenConvexShell();
                        if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                        if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                    }

                    SLEEP(300);
                }

                // save mesh or triangulation to file
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {
                    if (Big_Pass.pass_mesh) cout << "";
                    if (Big_Pass.pass_convex_shell) cout << "";
                    if (Big_Pass.pass_triangulation)
                    {
                        trian.saveToFile(path::triangulation + "nodes_elements.txt", list_triangulaton_triangles, vec_circle_points, q);
                        cout << "triangulation saved" << endl;
                    }
                    if (Big_Pass.pass_quadtree)
                    {
                        q.saveCoordinetsToFile(path::quadtree_structured_mesh + "nodes_elements.txt");
                        cout << "quadtree mesh saved" << endl;
                    }

                    SLEEP(100);
                }


                // Random Moving Poinst //
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    if (!Big_Pass.prop_random_point_movement)
                    {
                        Big_Pass.prop_random_point_movement = true;

                        BEN(system("cls");)
                            // nie ma tutaj od razu rekalkulacji, bo dla tej opcji rekalkulacja musi byæ ci¹gla
                           
                        SLEEP(100);
                    }
                    else
                    {
                        Big_Pass.prop_random_point_movement = false;
                        SLEEP(100);
                    }
                }


                // connect
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                {
                    if (Big_Pass.prop_connecting_activated)
                    {
                        Big_Pass.prop_connecting_activated = false;
                        signal_circle.setFillColor(sf::Color::Red);
                    }
                    else
                    {
                        Big_Pass.prop_connecting_activated = true;
                        signal_circle.setFillColor(sf::Color::Green);
                    }
                    SLEEP(100);
                }
                // nearest neighbour
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
                {
                    if (Big_Pass.pass_closest_point == false)
                    {
                        Big_Pass.pass_closest_point = true;
                    }
                    else
                    {
                        o.All_Points_Green(vec_circle_points);
                        Big_Pass.pass_closest_point = false;
                    }

                    SLEEP(150);
                }
                // Siatka prostok¹tna
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
                {
                    // before Big_Pass
                    /*
                    if (!pass_mesh && !vec_circle_points.empty())
                    {
                        pass_mesh = true;

                        // to nie s¹ ci¹g³e rekalkulacje tylko pojedynczy, ¿eby coœ pokaza³ ju¿ w momencie klikniêcia
                        recal.recalculateMesh(list_mesh_blocks, vec_circle_points, x_slices, y_slices);

                        LOG("Mesh complete");
                        SLEEP(100);
                    }
                    else
                    {
                        pass_mesh = false;
                        SLEEP(100);
                    }
                    */

                    if (!Big_Pass.pass_mesh && !vec_circle_points.empty())
                    {
                        Big_Pass.pass_mesh = true;

                        // to nie s¹ ci¹g³e rekalkulacje tylko pojedynczy, ¿eby coœ pokaza³ ju¿ w momencie klikniêcia
                        recal.recalculateMesh(list_mesh_blocks, vec_circle_points, x_slices, y_slices);

                        LOG("Mesh complete");
                        SLEEP(100);
                    }
                    else
                    {
                        Big_Pass.pass_mesh = false;
                        SLEEP(100);
                    }
                }
                // Lines to all points
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    if (p_distance_calculation_circle)
                    {
                        if (!Big_Pass.pass_lines_to_all)
                        {
                            Big_Pass.pass_lines_to_all = true;

                            // to nie s¹ ci¹g³e rekalkulacje tylko pojedynczy, ¿eby coœ pokaza³ ju¿ w momencie klikniêcia
                            recal.recalculateSetVertexArrayForDistanceCircleLines
                            (vec_circle_points, tab_VertexArray_points_in_circle, p_distance_calculation_circle);

                            SLEEP(100);
                        }
                        else
                        {
                            Big_Pass.pass_lines_to_all = false;
                            SLEEP(100);
                        }
                    }
                }
                // Distances to all points
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {
                    if (p_distance_calculation_circle)
                    {
                        if (!Big_Pass.pass_distance_to_all)
                        {
                            Big_Pass.pass_distance_to_all = true;

                            // to nie s¹ ci¹g³e rekalkulacje tylko pojedynczy, ¿eby coœ pokaza³ ju¿ w momencie klikniêcia
                            recal.recalculateSetTextDistanceCircle(list_length_of_distances_dcl, vec_circle_points,
                                p_distance_calculation_circle, font);

                            SLEEP(100);
                        }
                        else
                        {
                            Big_Pass.pass_distance_to_all = false;
                            list_length_of_distances_dcl.clear();
                            SLEEP(100);
                        }
                    }
                }
                // P bo projekt - show_quadtree 
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
                {                    
                    Big_Pass.pass_quadtree = !Big_Pass.pass_quadtree;
                    SLEEP(100);
                }
                // Otoczka wypuk³a
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
                {
                    if (!Big_Pass.pass_convex_shell)
                    {
                        Big_Pass.pass_convex_shell = true;

                        recal.recalculateConvexShell(vec_circle_points, tab_VertexArray_points_in_circle);

                        SLEEP(100);
                    }
                    else
                    {
                        Big_Pass.pass_convex_shell = false;
                        SLEEP(100);
                    }
                }
                // Circum Circle of 3 points
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
                {
                    if (!Big_Pass.pass_circum_circle)
                    {
                        Big_Pass.pass_circum_circle = true;

                        //circum_circle
                        if (vec_circle_points.size() == 3)
                            recal.recalculate3pointsCircle(vec_circle_points, &circum_circle, Color::Yellow, &circum_triangle);

                        SLEEP(100);
                    }
                    else
                    {
                        Big_Pass.pass_circum_circle = false;
                        SLEEP(100);
                    }
                }
                // Triangulation
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
                {
                    if (!Big_Pass.pass_triangulation)
                    {
                        Big_Pass.pass_triangulation = true;

                        BEN(system("cls");)
                        //circum_circle
                        if (vec_circle_points.size() > 2)
                            recal.recalculateTriangulation(vec_circle_points, list_triangulaton_triangles);

                        SLEEP(100);
                    }
                    else
                    {
                        Big_Pass.pass_triangulation = false;
                        SLEEP(100);
                    }
                }
            }

            // Modes //     only changing chosen + All_Points_Green
            if (1)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                {
                    chosen = 1; o.All_Points_Green(vec_circle_points); chosen_mode.setString("1");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                {
                    chosen = 2; o.All_Points_Green(vec_circle_points); chosen_mode.setString("2");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
                {
                    chosen = 3; o.All_Points_Green(vec_circle_points); chosen_mode.setString("3");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
                {
                    chosen = 4; o.All_Points_Green(vec_circle_points); chosen_mode.setString("4");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
                {
                    chosen = 5; o.All_Points_Green(vec_circle_points); chosen_mode.setString("5");
                }
            }           
            



            // exit
            if (event.type == Event::Closed) {
                window.close();
            }

            #if SLEEP_CLS == 1
                Sleep(1000);
                system("cls");
            #endif
            
            // random points movement
            if (Big_Pass.prop_random_point_movement)
            {
                recal.recalculateRandomPointPositions(vec_circle_points, 1);

                // Passes //
                {
                    Big_Pass.tokenMesh();
                    Big_Pass.tokenLines();
                    Big_Pass.tokenDistanceLine();
                    Big_Pass.tokenDistanceText();
                    Big_Pass.tokenConvexShell();
                    if (vec_circle_points.size() == 3) Big_Pass.tokenCircumCircle();
                    if (vec_circle_points.size() > 2)  Big_Pass.tokenTriangulation();
                }
            }

            // Recalculating //
            Big_Pass.Recalculating();


            // searching for optimal quad_limit
            #if QUAD_SEARCHING_FOR_OPTIMAL == 1

                if (!optimum_reached)
                {
                    window.clear();

                    #if QUAD_TREE_ACTIVE == 1
                        q.Start_Quadding(optimal_quad_limit);
                        quadtree_list = q.getListForVisualization();
                    #endif

                                    // quad tree
                    int quad_counter{};
                    #if QUAD_TREE_ACTIVE == 1
                        for (auto& obj : quadtree_list)
                        {
                            window.draw(obj);
                            quad_counter++;
                        }
                    #endif

                        if (quad_counter < 1000) optimum_reached = true;
                        else
                        {
                            optimal_quad_limit += 5;
                            q.Cleaning_quad_entry();
                        }

                    SLEEP(500);
                    window.display();
                }

                else
                {
            #endif

            window.clear();
            {
                // Checking line in between 2 lines
                /*
                if (0)
                {
                    if (vec_circle_points.size() == 3)
                    {
                        system("cls");

                        Line tmp1(vec_circle_points[0], vec_circle_points[2]);
                        Line tmp2(vec_circle_points[0], vec_circle_points[1]);

                        Line inBetween;
                        inBetween = inBetween.lineInTheMiddleOf2(tmp1, tmp2);

                        cout << "main" << endl;
                        cout << tmp1.angleToOXinDegrees() << endl;
                        cout << tmp2.angleToOXinDegrees() << endl;
                        cout << endl;

                        cout << "Line inBetween " << endl;
                        cout << "a " << inBetween.a << endl;
                        cout << "b " << inBetween.b << endl;
                        cout << "point1 " << inBetween.point1.x << " " << inBetween.point1.y << endl;
                        cout << "point2 " << inBetween.point2.x << " " << inBetween.point2.y << endl;

                        window.draw(vfunc.drawLine(inBetween, Color::Red));
                    }
                }
                */

                // tmp --- true false
                /*
                {
                    // jak u¿ywaæ myVector i Cross_with_lines
                    if (0)
                    {
                        myVector my(300, 250, true);
                        my.Print();


                        CircleShape c;
                        c.setRadius(250);
                        c.setPosition(*my);

                        // zmiana pozycji, tak ¿eby myVector to by³ œrodek okrêgu
                        c.setPosition(cir.setMiddle(c));
                        window.draw(c);


                        cout << "From c" << endl;
                        Vector2f from_circle = cir.getMiddle(c);
                        cout << "x: " << from_circle.x << "    y: " << from_circle.y << endl;

                        VertexArray lines = o.Cross_with_lines(*my);
                        window.draw(lines);
                    }

                    // Trójk¹t z punktami na koñcach

                    if (0)
                    {
                        Triangle tra(Vector2f(0, 0), Vector2f(150, 300), Vector2f(300, 0));

                        list<VertexArray> testing;      list<list<CircleShape>> middle_of_triangles;
                        testing.push_back(vfunc.drawTriangle(tra, Color::Red, offSet(300, 300)));
                        middle_of_triangles.push_back(vfunc.drawPointsOnEndOfTriangle(tra, Color::Red, offSet(300, 300), 10));

                        testing.push_back(vfunc.drawTriangle(tra.downLeft(), Color::Red, offSet(300, 300)));
                        middle_of_triangles.push_back(vfunc.drawPointsOnEndOfTriangle(tra.downLeft(), Color::Red, offSet(300, 300), 10));

                        testing.push_back(vfunc.drawTriangle(t.downRight(), Color::Red, offSet(300, 300)));
                        middle_of_triangles.push_back(vfunc.drawPointsOnEndOfTriangle(tra.downRight(), Color::Red, offSet(300, 300), 10));

                        testing.push_back(vfunc.drawTriangle(t.up(), Color::Red, offSet(300, 300)));
                        middle_of_triangles.push_back(vfunc.drawPointsOnEndOfTriangle(tra.up(), Color::Red, offSet(300, 300), 10));

                        testing.push_back(vfunc.drawTriangle(t.middle(), Color::Red, offSet(300, 300)));
                        middle_of_triangles.push_back(vfunc.drawPointsOnEndOfTriangle(tra.middle(), Color::Red, offSet(300, 300), 10));


                        //int a = 1;
                        for (auto& i : testing)
                        {
                            //cout << a << endl; a++;
                            window.draw(i);
                        }

                        for (auto& single_list : middle_of_triangles)
                        {
                            for (auto& single_circle : single_list)
                            {
                                window.draw(single_circle);
                            }
                        }
                    }


                    // testing line     with random point
                    if (0)
                    {
                        Line linus(Vector2f(500, 700), Vector2f(800, 900));

                        Vector2f rand = linus.RandomPointOnLine();

                        CircleShape circle_c;
                        circle_c.setRadius(10);
                        circle_c.setPosition(rand);
                        circle_c.setPosition(cir.setMiddle(circle_c));

                        window.draw(vfunc.drawLine(linus, Color::Red));
                        window.draw(circle_c);
                    }

                    // testing line     next position
                    if (0)
                    {
                        randy = laaaaaa.NextPointAfter(randy, 0.5, false);

                        CircleShape circle_c;
                        circle_c.setRadius(10);
                        circle_c.setPosition(randy);
                        circle_c.setPosition(cir.setMiddle(circle_c));

                        window.draw(vfunc.drawLine(laaaaaa, Color::Red));
                        window.draw(circle_c);
                    }


                    // rekurencja Fraktal z Trójk¹tów

                    if (0)
                        for (auto& obj : triangle_list)
                        {
                            window.draw(obj);
                        }

                }
                */


                // Show if Big_Pass
                if (1)
                {
                    // p_distance_calculation_circle - Yellow
                    if (p_distance_calculation_circle != nullptr)
                        window.draw(*p_distance_calculation_circle);

                    // point inside circles
                    if (Big_Pass.pass_connecting_points && tab_VertexArray_points_in_circle[0].getVertexCount() != 0)
                        window.draw(tab_VertexArray_points_in_circle[0]);

                    // lines to all points
                    if (Big_Pass.pass_lines_to_all && tab_VertexArray_points_in_circle[1].getVertexCount() != 0)
                        window.draw(tab_VertexArray_points_in_circle[1]);

                    // otoczka
                    if (Big_Pass.pass_convex_shell && tab_VertexArray_points_in_circle[2].getVertexCount() != 0)
                        window.draw(tab_VertexArray_points_in_circle[2]);

                    // distances to all points list_length_of_distances_dcl
                    if (Big_Pass.pass_distance_to_all)
                        for (list<Text>::iterator it = list_length_of_distances_dcl.begin(); it != list_length_of_distances_dcl.end(); it++)
                            window.draw(*it);

                    // marking closest point --- rekalkulacja nic nie zwraca, bo funkcja od razu zmienia kolor odpowiedniego punktu
                    if (Big_Pass.pass_closest_point && p_distance_calculation_circle)
                        recal.recalculateClosestNeightbour(vec_circle_points, p_distance_calculation_circle);

                    // mesh lines
                    if (Big_Pass.pass_mesh)
                        for (list<VertexArray>::iterator it = list_mesh_blocks.begin(); it != list_mesh_blocks.end(); it++)
                            window.draw(*it);

                    // quad tree
                    #if QUAD_TREE_ACTIVE == 1
                        if(Big_Pass.pass_quadtree)
                            for (auto& obj : quadtree_list)
                                window.draw(obj);
                    #endif

                    // circum circle
                    if (Big_Pass.pass_circum_circle && vec_circle_points.size() == 3)
                    {
                        if (circum_circle != nullptr) window.draw(*circum_circle);
                        else window.draw(vfunc.drawTriangle(*circum_triangle, Color::Red, offSet(0, 0)));
                    }

                    // triangulation
                    if (Big_Pass.pass_triangulation)
                    {
                        if (vec_circle_points.size() < 3)
                            list_triangulaton_triangles.clear();

                        sf::Text text_triangle_id;
                        for (auto& t : list_triangulaton_triangles)
                        {
                            window.draw(vfunc.drawTriangle(t, Color::Red, offSet(0, 0)));


                            text_triangle_id.setFont(font);
                            //text_triangle_id.setString(to_string(t.m_id_number));
                            text_triangle_id.setCharacterSize(20); // in pixels, not points!
                            text_triangle_id.setFillColor(sf::Color::White);
                            //text_numbers_of_points.setStyle(sf::Text::Bold);
                            text_triangle_id.setPosition(t.getPositionForNumber());
                            //window.draw(text_triangle_id);                        
                        }
                    }
                }

                // menu staff + circles
                if (1)
                {
                    // circles
                    if (Big_Pass.prop_drawing_circles)
                    {
                        for (auto& point : vec_circle_points)
                        {
                            window.draw(*point.m_circle);

                            // Drawing numbers of circles
                            {
                                if (Big_Pass.prop_drawing_coordinets != 3)
                                {
                                    Vector2f original_position = point.getMiddle();
                                    Vector2f position = original_position;

                                    sf::Text text_numbers_of_points;
                                    text_numbers_of_points.setFont(font);
                                    text_numbers_of_points.setCharacterSize(20); // in pixels, not points!
                                    text_numbers_of_points.setFillColor(sf::Color::White);

                                    switch (Big_Pass.prop_drawing_coordinets)
                                    {
                                        // tylko numeracja
                                    case 0:
                                    {
                                        position.x += 3;
                                        position.y += 3;

                                        text_numbers_of_points.setPosition(position);
                                        text_numbers_of_points.setString(to_string(point.m_id));


                                        window.draw(text_numbers_of_points);
                                        break;
                                    }

                                    // tylko wspó³rzêdne
                                    case 1:
                                    {
                                        myVector kartezius_position(original_position, true);
                                        Vector2f kartez = *kartezius_position;

                                        position.x -= 40;
                                        position.y += 10;
                                        text_numbers_of_points.setPosition(position);
                                        text_numbers_of_points.setString("(" + to_string((int)kartez.x) +
                                            ", " + to_string((int)kartez.y) + ")");


                                        window.draw(text_numbers_of_points);
                                        break;
                                    }

                                    // numbercja + wspó³rzêdne
                                    case 2:
                                    {
                                        position.x += 3;
                                        position.y += 3;

                                        text_numbers_of_points.setString(to_string(point.m_id));
                                        text_numbers_of_points.setPosition(position);
                                        window.draw(text_numbers_of_points);

                                        myVector kartezius_position(original_position, true);
                                        Vector2f kartez = *kartezius_position;

                                        text_numbers_of_points.setString("(" + to_string((int)kartez.x) +
                                            ", " + to_string((int)kartez.y) + ")");
                                        position.x -= 40;
                                        position.y += 20;
                                        text_numbers_of_points.setPosition(position);
                                        window.draw(text_numbers_of_points);

                                        break;
                                    }

                                    default: {  break; }
                                    }
                                }
                            }
                        }
                    }

                    // menu
                    if (Big_Pass.prop_show_menu)
                    {
                        window.draw(text);
                        window.draw(signal_circle); window.draw(chosen_mode);
                    }
                }
            }
            window.display();

            #if QUAD_SEARCHING_FOR_OPTIMAL == 1
            }

            #endif // QUAD_SEARCHING_FOR_OPTIMAL == 1


        //}

        // stare
        // Continuous Random Points Position
        /*
        window.clear();
        // Stuff that is continuous
        if(1)
        {
            // random points movement
            if (Big_Pass.prop_random_point_movement)
                recal.recalculateRandomPointPositions(vec_circle_points, 1);
        }

        // menu staff + circles
        {
            // circles
            if (Big_Pass.prop_drawing_circles)
            {
                int c = 1;
                for (list<CircleShape*>::iterator it = vec_circle_points.begin(); it != vec_circle_points.end(); it++)
                {
                    window.draw(**it);

                    // Drawing numbers of circles
                    {
                        Vector2f position = cir.getMiddle(*it);
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
            }

            // menu
            if (Big_Pass.prop_show_menu)
            {
                window.draw(text);
                window.draw(signal_circle); window.draw(chosen_mode);
            }
        }
        
        window.display();
        */
    }    

    return 0;
}