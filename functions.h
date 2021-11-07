
#include <array>
#include <vector>
using namespace std;


const int size=64;
class conway_class{
  public:
    int frame_num;
    int col_r , col_g, col_b;
    array<array<int,size>,size> frame= {{}};
    array<array<int,size>,size> frame_prev= {{}};
    array<array<int,size>,size> frame_prev_2= {{}};
    array<array<int,size+2>,size+2> frame_bc= {{}};
    array<array<int,size>,size> initialise(array<array<int,size>,size> input);

    array<array<int,size+2>,size+2> wrapper(array<array<int,size>,size> input);

    array<array<int,size>,size> next_frame(array<array<int,size+2>,size+2> input);
};

class ant_class{
  public:
    int xpos, ypos, orientation,frame_num, col_r, col_g,col_b;
    vector<vector<int>> frame;
    vector<vector<int>> initialise(int num);

    ant_class next_frame(ant_class input);
};

class sand_class{
  public:
    bool wild, stable;
    int col_1r, col_1g,col_1b, col_2r, col_2g,col_2b, col_3r, col_3g,col_3b, col_4r, col_4g,col_4b;
    vector<vector<int>> frame;
    sand_class next_grain(sand_class input);
    sand_class evolve(sand_class input);

};

