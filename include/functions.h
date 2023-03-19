
#include <vector>
using namespace std;


class Particle {
    public:
        vector<int> pos = {-1, 0};
        vector<int> vel = {0, 0};
        vector<int> col = {255, 255, 255};
        int orientation = 0;
	int value = 0;
        bool action = false;
};

class Frame {
    public:
        int size = 64;
	vector<int> black = {0,0,0};
        vector<vector<vector<int>>> blank_frame = vector<vector<vector<int>>> (size, vector<vector<int>>(size, black));
        vector<vector<int>> sframe = vector<vector<int>> (size, vector<int>(size, 0));
        vector<vector<vector<int>>> frame = blank_frame; 
        void wild(bool input);
        void create(vector<Particle> input);
        void clear();
};

class Pattern {
    public:
	vector<Particle> particles;
	Particle particle;
	Frame frame;
        int size = 64;
        int frame_num, interval;
        int prob = 60;
	bool wild = false;
	bool stable = false;
        vector<int> global_col;
};

class Conway:public Pattern {
    private:
        vector<vector<int>> wrap_frame(vector<vector<int>> input), frame_prev, frame_prev_2;
    public:
        void initialise();
        void evolve();
};

class Ant:public Pattern {
    private:
        int start_num = 2;
    public:
        void initialise();
        void evolve();
};

class Sand:public Pattern {
    public:
	vector<vector<int>> cols = {{0,0,0}, {0,0,80}, {0,80,40}, {100,100,0}, {255,0,0}};
        void next_grain();
        void evolve();
};

class Bounce:public Pattern {
    public:
        int num_balls;
        void initialise();
        void evolve();
        vector<int> mode(vector<Particle> balls);
};

class Scatter:public Pattern {
    public:
        int num_rows = 17;

        vector<int> pin_col = {0,0,150};
        vector<vector<int>> ball_cols = {{255,0,0}, {30,120,35}, {0,200,40}};
        vector<Particle> balls;
	vector<vector<vector<int>>> static_balls_frame; 
	vector<vector<int>> pin_frame = frame.sframe; 
    
        void initialise(int num_streams);
        void evolve();
};

class Star:public Pattern {
    private:
	Particle create_star(vector<Particle> input, int col);
    public:
        int max_brightness;
        int num_stars = 200;
        int twinkle_prob = 1000;
        int decay_prob = 2000;
        vector<Particle> stars;
        void initialise();
        void evolve();
};
