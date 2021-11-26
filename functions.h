
#include <vector>
using namespace std;

class conway_class{
  public:
    int size=64;
    int frame_num;
    int col_r , col_g, col_b;
    vector<vector<int>> frame= vector<vector<int>>(size,vector<int>(size,0));
    vector<vector<int>> frame_prev= vector<vector<int>>(size,vector<int>(size,0));
    vector<vector<int>> frame_prev_2= vector<vector<int>>(size,vector<int>(size,0));
    vector<vector<int>> frame_bc= vector<vector<int>>(size+2,vector<int>(size+2,0));
    vector<vector<int>> initialise(vector<vector<int>> input);
    vector<vector<int>> wrapper(vector<vector<int>> input);
    vector<vector<int>> next_frame(vector<vector<int>> input);
};

class ant_class{
  public:
    int size=64;
    int frame_num, col_r, col_g,col_b;
    int xpos=size/2;  int ypos=size/2;
    int orientation=0;
    vector<vector<int>> frame;
    vector<vector<int>> initialise(int num);

    ant_class next_frame(ant_class input);
};

class sand_class{
  public:
    int size=64;  
    bool wild=true; 
    bool stable=true;
    int col_1r, col_1g,col_1b, col_2r, col_2g,col_2b, col_3r, col_3g,col_3b, col_4r, col_4g,col_4b;
    vector<vector<int>> frame=vector<vector<int>>(size,vector<int>(size,0));;
    sand_class next_grain(sand_class input);
    sand_class evolve(sand_class input);
};

class particle_class{
  public:
    vector<int> pos={-1,0};
    vector<int> vel={0,0};
    vector<int> col={255,0,0};
    bool action =false;
};

class bounce_class{
  public:
    int size=64;  
    int num_balls;
    vector<int> mode;
    particle_class ball;
    vector<particle_class> initialise(int num);
    vector<particle_class> next_frame(vector<particle_class> input);
    vector<int> mode_func(vector<particle_class> input);
};

class scatter_class{
  public:
    int size=64;  
    int num_rows=17;
    int num_balls;
    int num_streams;
  
    vector<int> ball_col1={255,0,0};
    vector<int> ball_col2_1={30,120,35};
    vector<int> ball_col2_2={0,200,40};

    vector<int> pin_col;
    vector<vector<int>> pins=vector<vector<int>>(num_rows*(num_rows+1)/2,vector<int>(2,0));
    vector<particle_class> balls;
    scatter_class initialise(scatter_class start);
    scatter_class evolve(scatter_class input);
};

class rain_class{
   public:
    int size=64;   
    int spawn_prob=80;
    int water_level=size-1;
    int water_surface_ideal=8;
    int water_surface=8;
        int max_drops=550;
    int counter=0;
    int raise_level=150;
    vector<int> rain_col={90,120,190};
    vector<int> water_col={0,0,14};
    vector<int> surface_col={0,20,20};
    particle_class drop;
    vector<particle_class> rain= vector<particle_class>(max_drops,drop);
    rain_class evolve(rain_class input);
  };
