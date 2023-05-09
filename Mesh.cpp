#pragma once
#include "header.h"

/// TRZEBA JESZCZE MODYFIKOWAÆ POZYCJE Z UWAGI NA ŒRODEK PUNKTU
void Mesh::Save_points_to_file(list<CircleShape*>& circle_list, float how_many_x_slices, float how_many_y_slices)
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

    ofstream FILE("_input/_mesh/mesh.txt");

    for (float i = bottom_left.x; i <= upper_right.x; i += x_jumper)
    {
        for (float j = bottom_left.y; j <= upper_right.y; j += y_jumper)
        {
            FILE << i << " " << j << endl;
        }
    }
}
// save in blocks        
void Mesh::Save_points_to_file_blocks(const list<CircleShape*>& circle_list, float how_many_x_slices, float how_many_y_slices)
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

    ofstream FILE("_input/_mesh/list_mesh_blocks.txt");

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

bool Mesh::approximatelyEqual(float a, float b, float epsilon)
{
    return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}
void Mesh::Visualize_Mesh(list<VertexArray>& list_mesh_blocks)
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

    ifstream FILE("_input/_mesh/list_mesh_blocks.txt");

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