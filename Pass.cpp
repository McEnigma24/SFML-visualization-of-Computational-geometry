#pragma once
#include "header.h"


    // Token //

    Pass::Token::Token()
    {
        token_tab.fill(false);
    }
    void Pass::Token::Clear_Tokes()
    {
        for (bool& i : token_tab)
            i = false;
    }

    // ~Token //


Pass::Pass(list<CircleShape*>* p_list_circle_points, VertexArray** p_tab_VertexArray_points_in_circle,
    list<VertexArray>* p_list_mesh_blocks, int x_sl, int y_sl,
    CircleShape** ppp_chosen_circle_for_moving, CircleShape** ppp_distance_calculation_circle, Font* p_font,
    list<Text>* p_list_length_of_distances_dcl, 
    CircleShape** pp_circum_circle, Triangle** pp_circum_triangle,
    list<Triangle>* p_list_triangulation
)
{
    // properties
    prop_drawing_circles = true;
    prop_show_menu = true;
    prop_connecting_activated = true;

    // recalculations
    pass_mesh = false;
    pass_connecting_points = false;
    pass_lines_to_all = false;
    pass_distance_to_all = false;
    pass_closest_point = false;
    pass_convex_shell = false;
    pass_circum_circle = false;
    pass_triangulation = false;



    // Points
    list_circle_points = p_list_circle_points;

    // VertexArray
    tab_VertexArray_points_in_circle = p_tab_VertexArray_points_in_circle;

    // Mesh
    list_mesh_blocks = p_list_mesh_blocks; x_slices = x_sl; y_slices = y_sl;

    // Special points
    p_chosen_circle_for_moving = ppp_chosen_circle_for_moving;
    p_distance_calculation_circle = ppp_distance_calculation_circle;    
    p_circum_circle = pp_circum_circle;
    p_circum_triangle = pp_circum_triangle;

    // Properties
    font = p_font;
    list_length_of_distances_dcl = p_list_length_of_distances_dcl;

    // Triangulation
    list_triangulation = p_list_triangulation;
}

void Pass::Token_Mesh() { if (pass_mesh) tokens.token_tab[0] = true; }
void Pass::Token_Lines() { if (pass_connecting_points) tokens.token_tab[1] = true; }
void Pass::Token_Distance_line() { if (pass_lines_to_all) tokens.token_tab[2] = true; }
void Pass::Token_Distance_text() { if (pass_distance_to_all) tokens.token_tab[3] = true; }
void Pass::Token_Convex_shell() { if (pass_convex_shell) tokens.token_tab[4] = true; }
void Pass::Token_Circum_Circle() { if (pass_circum_circle) tokens.token_tab[5] = true; }
void Pass::tokenTriangulation() { if (pass_triangulation) tokens.token_tab[6] = true; }

void Pass::Recalculating()
{
    // Tokens //
    // token_tab[0] == mesh_token
    // token_tab[1] == mesh_token
    // token_tab[2] == mesh_token
    // token_tab[3] == mesh_token
    // token_tab[4] == convex_shell
    // token_tab[5] == circum_circle
    // token_tab[6] == triangulation


    if (pass_mesh && tokens.token_tab[0]) recalculateMesh((*list_mesh_blocks), *list_circle_points, x_slices, y_slices);
    if (pass_connecting_points && tokens.token_tab[1]) recalculateLinesVertexArrayBasedOnExistingCircleList(*list_circle_points, *tab_VertexArray_points_in_circle, sf::LinesStrip, prop_connecting_activated);
    if (pass_lines_to_all && *p_distance_calculation_circle != nullptr && tokens.token_tab[2]) recalculateSetVertexArrayForDistanceCircleLines(*list_circle_points, *tab_VertexArray_points_in_circle, *p_distance_calculation_circle);
    if (pass_distance_to_all && *p_distance_calculation_circle != nullptr && tokens.token_tab[3]) recalculateSetTextDistanceCircle(*list_length_of_distances_dcl, *list_circle_points, *p_distance_calculation_circle, *font);
    if (pass_convex_shell && list_circle_points->size() > 2 && tokens.token_tab[4])
    {
        All_Points_Green(*list_circle_points);
        recalculateConvexShell(*list_circle_points, *tab_VertexArray_points_in_circle);
    }

    if (pass_circum_circle && tokens.token_tab[5]) recalculate3pointsCircle(*list_circle_points, p_circum_circle, Color::Yellow, p_circum_triangle);
    if (pass_triangulation && tokens.token_tab[6])
    {
        BEN(system("cls");)
        recalculateTriangulation(*list_circle_points, *list_triangulation);
    }


    tokens.Clear_Tokes();
}