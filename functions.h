
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
    int frame_num, col_r, col_g,col_b;
    int xpos=size/2;  int ypos=size/2;
    int orientation=0;
    vector<vector<int>> frame;
    vector<vector<int>> initialise(int num);

    ant_class next_frame(ant_class input);
};

class sand_class{
  public:
    bool wild=true; 
    bool stable=true;
    int col_1r, col_1g,col_1b, col_2r, col_2g,col_2b, col_3r, col_3g,col_3b, col_4r, col_4g,col_4b;
    vector<vector<int>> frame=vector<vector<int>>(size,vector<int>(size,0));;
    sand_class next_grain(sand_class input);
    sand_class evolve(sand_class input);
};

class ball_class{
  public:
    vector<int> pos={0,0};
    vector<int> vel={0,0};
    vector<int> col={255,0,0};
};


class bounce_class{
  public:
    int num_balls;
    vector<int> mode;
    ball_class ball;
    vector<ball_class> initialise(int num);
    vector<ball_class> next_frame(vector<ball_class> input);
    vector<int> mode_func(vector<ball_class> input);
};

class scatter_class{
  public:
    int num_rows=17;
    int num_balls;
    int num_streams;
  
    vector<int> ball_col1={255,0,0};
    vector<int> ball_col2_1={30,120,35};
    vector<int> ball_col2_2={0,200,40};

    vector<int> pin_col;
    vector<vector<int>> pins=vector<vector<int>>(num_rows*(num_rows+1)/2,vector<int>(2,0));
    vector<ball_class> balls;
    scatter_class initialise(scatter_class start);
    scatter_class evolve(scatter_class input);
};
