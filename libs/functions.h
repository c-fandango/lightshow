
#include <vector>
using namespace std;

class particle_class {
public:
    vector<int> pos= {-1,0};
    vector<int> vel= {0,0};
    vector<int> col= {255,0,0};
    bool action =false;
};

class virtual_frame_class {
public:
    int size =64;
    vector<vector<vector<int>>> blank_frame = vector<vector<vector<int>>> (size, vector<vector<int>>(size, vector<int> (3,0)));
    vector<vector<vector<int>>> frame = blank_frame;
    void wild(bool input);
    void create (vector<particle_class> input);
    void clear ();
};

class conway_class {
private:
    vector<vector<int>> frame_bc;
    vector<vector<int>> frame_prev, frame_prev_2;
    void wrapper();
public:
    int size=64;
    int frame_num;
    int wild_prob = 100;
    bool stable =false;
    bool wild =false;
    vector<int> col;
    vector<vector<int>> frame;
    void initialise();
    void next_frame();
};

class ant_class {
private:
    int orientation=0;
public:
    int start_num=0;
    int size=64;
    int frame_num;
    int xpos=size/2;
    int ypos=size/2;
    vector<int> trail_col;
    vector<int> col;
    vector<vector<int>> frame;
    void initialise();
    void next_frame();
};

class sand_class {
private:
    int xpos,ypos;
public:
    int size=64;
    bool wild=true;
    bool stable=true;
    int col_1r, col_1g,col_1b, col_2r, col_2g,col_2b, col_3r, col_3g,col_3b, col_4r, col_4g,col_4b;
    vector<vector<int>> pile=vector<vector<int>>(size,vector<int>(size,0));
    vector<vector<int>> new_pile;
    void next_grain();
    void evolve();
};

class bounce_class {
private:
    vector<vector<int>> elements;
    vector<int> count;
    int max;
public:
    int size=64;
    int num_balls;
    vector<particle_class> balls;
    void initialise();
    void evolve();
    vector<int> mode;
    void mode_func();
};

class scatter_class {
public:
    int size=64;
    int num_rows=17;
    int num_balls;
    int num_streams;
    particle_class pin;
    particle_class ball;
    vector<int> pin_col  = {0,0,150};

    vector<int> ball_col1= {255,0,0};
    vector<int> ball_col2_1= {30,120,35};
    vector<int> ball_col2_2= {0,200,40};

    vector<particle_class> pins;
    vector<particle_class> balls;
    void initialise();
    void evolve();
};

class rain_class {
public:
    int size=64;
    int spawn_prob=80;
    int water_level=size-2;
    int water_surface=8;
    int max_drops=550;
    int counter=0;
    int raise_level=150;
    int start_pos;
    vector<int> rain_col= {90,120,190};
    vector<int> water_col= {0,0,14};
    vector<int> surface_col= {0,20,20};
    particle_class drop;
    vector<particle_class> rain;
    void initialise();
    void evolve();
};

class star_class {
private:
    int col=0;
    vector<int> pos = {0,0};
    particle_class initial_star;
public:
    int size=64;
    int num;
    int decay_prob = 2000;
    vector<particle_class> stars;
    void initialise();
    void evolve();
};
