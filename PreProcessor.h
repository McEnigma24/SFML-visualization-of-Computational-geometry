#pragma once
#include <string>

using namespace std;
using namespace sf;

// Properties //
# define PI          3.141592653589793238462643383279502884

#define WIDTH 1000
#define HEIGHT 1000
#define VERTEX_ARRAY_SIZE 3
#define CIRCLE_RAD 10

// BENCHMARKING //
namespace ben
{
    static string path_benchmark = "_benchmarking/";
    static string trinagulation = path_benchmark + "triangulation.txt";
}

// Actives //
#define ACTIVE_LOG 0
#define BENCHMARKING 1
#define BENCHMARKING_FILE_SAVE 1
#define QUAD_TREE_ACTIVE 0
#define SLEEP_CLS 0



// LOG
#if ACTIVE_LOG == 1
#define LOG(x) cout << x << endl
#else
#define LOG(x)
#endif

//BEN
#if BENCHMARKING == 1
#define BEN(x) x;
#else
#define BEN(x)
#endif














// Mesh //
const int x_slices = 15;
const int y_slices = 15;

// QuadTree //
#define QUAD_LIMIT 1




// trik
#define endl "\n"

// Macros //
#define SLEEP(x) std::this_thread::sleep_for(std::chrono::milliseconds(x))

#define SQ(x) (x * x)
#define REC(y) (HEIGHT - y)
#define V(x,y) (x, (HEIGHT - y))

#define CLEAR() \
{\
for (auto& p : list_circle_points) delete p;\
list_circle_points.clear();\
list_length_of_distances_dcl.clear();\
list_mesh_blocks.clear();\
delete[] tab_VertexArray_points_in_circle;\
tab_VertexArray_points_in_circle = new VertexArray[VERTEX_ARRAY_SIZE];\
p_chosen_circle_for_moving = nullptr;\
if (p_distance_calculation_circle != nullptr) delete p_distance_calculation_circle; p_distance_calculation_circle = nullptr;\
}
// ~Macros //