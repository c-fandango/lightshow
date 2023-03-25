#include "../include/led-matrix.h"
#include "../include/functions.h"
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
using namespace std;
using namespace std::chrono;
using namespace rgb_matrix;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

int PLAY_FLAG = 1;

int read_file(string path) {
    int flag;
    ifstream file(path);
    if (file.good() && file.is_open()) {
	file >> flag;
	if (flag == 1){
	    return flag;
	}
    }
    return 0;
}
void msleep(int interval) {
    if (interval > 0) {
        this_thread::sleep_for(milliseconds(interval));
    }
}

void trd(string path, int interval) {
    while (true) {
        PLAY_FLAG = read_file(path);
        msleep(interval);
    }
}

class RunConway:public Conway {
    public: 
        void run (void *canvas_arg) override {
	    Canvas *canvas = (Canvas *)canvas_arg;

            int sum_time = 0;
            int sum_time2 = 0;
            for(int n=0; n<frame_num; ++n) {
    	        auto start = high_resolution_clock::now();

                evolve();
    	        auto start2 = high_resolution_clock::now();
                    for (int i=0; i<size; ++i) {
                        for (int j=0; j<size; ++j) {
    	        	    if (frame.sframe[i][j] && !old_frames[0][i][j]) {
                                canvas -> SetPixel(i, j, global_col[0], global_col[1], global_col[2]);
    	        	    }
    	                    else if (!frame.sframe[i][j] && old_frames[0][i][j]) {
                                canvas -> SetPixel(i, j, 0, 0, 0); 
    	            	    }
                        }
                    }

    	        auto stop = high_resolution_clock::now();
    	        auto duration = duration_cast<milliseconds>(stop - start);
    	        auto duration2 = duration_cast<milliseconds>(stop - start2);
    	        sum_time += duration.count();
    	        sum_time2 += duration2.count();
    
                msleep(interval-duration.count());
    	        if (stable || PLAY_FLAG){
                        cout<< sum_time/n<<endl;
                        cout<< sum_time2/n<<endl;
    	            return;
    	        }
            }
            cout<<"conway time"<<endl;
            cout<< sum_time/frame_num<<endl;
            cout<< sum_time2/frame_num<<endl;
        }
};

class RunAnt:public Ant {
    public:
        void run (void * canvas_arg) override {
	    Canvas *canvas = (Canvas *)canvas_arg;
            vector<vector<int>> prev_frame = frame.blank_sframe;
            int sum_time = 0;
            int sum_time2 = 0;
            for(int n=0; n<frame_num; ++n) {
        	auto start = high_resolution_clock::now();
                evolve();
        	auto start2 = high_resolution_clock::now();
                for (int i=0; i<size; ++i) {
                    for (int j=0; j<size; ++j) {
        		if (frame.sframe[i][j] && !prev_frame[i][j]) {
                            canvas -> SetPixel(i, j, global_col[0], global_col[1], global_col[2]);
        		}
        		else if (prev_frame[i][j] && !frame.sframe[i][j]) {
                            canvas -> SetPixel(i,j, 0, 0, 0); 
        		}
                    }
                }
                canvas -> SetPixel(particle.pos[1], particle.pos[0], particle.col[0], particle.col[1], particle.col[2]);
        	prev_frame = frame.sframe;
        	auto stop = high_resolution_clock::now();
        	auto duration = duration_cast<milliseconds>(stop - start);
        	auto duration2 = duration_cast<milliseconds>(stop - start2);
        	sum_time += duration.count();
        	sum_time2 += duration2.count();
        
                msleep(interval-duration.count());
                if (PLAY_FLAG) {
                    return;
                }
            }
            cout<<"ant time"<<endl;
            cout<< sum_time/frame_num<<endl;
            cout<< sum_time2/frame_num<<endl;
        }
};

class RunSand:public Sand {
    public:
        void run (void *canvas_arg) override {
	    Canvas *canvas = (Canvas *)canvas_arg;
            int height, prev_height;
            vector<vector<int>> prev_frame = frame.blank_sframe;
            int count = 0;
            int sum_time = 0;
            int sum_time2 = 0;
            while(!frame.sframe[size/4][size/2]) {
        	++count;
        	auto start = high_resolution_clock::now();
                if (stable) {
                    next_grain();
                }
                else {
                    evolve();
                }
        	auto start2 = high_resolution_clock::now();
                for (int i=0; i<size; ++i) {
                    for (int j=0; j<size; ++j) {
        		height = min(frame.sframe[i][j], 4);
        		prev_height = min(prev_frame[i][j], 4);
        		if (height != prev_height) {
                            canvas -> SetPixel(i, j, cols[height][0], cols[height][1], cols[height][2]);
        		}
                    }
                }
        	prev_frame = frame.sframe;
        	auto stop = high_resolution_clock::now();
        	auto duration = duration_cast<milliseconds>(stop - start);
        	auto duration2 = duration_cast<milliseconds>(stop - start2);
        	sum_time += duration.count();
        	sum_time2 += duration2.count();
                msleep(interval-duration.count());
                if (PLAY_FLAG) {
                    return;
                }
            }
            cout<<"sand time"<<endl;
            cout<< sum_time/count<<endl;
            cout<< sum_time2/count<<endl;
    }
};

class RunBounce:public Bounce {
    public:
        void run (void *canvas_arg) override {
	    Canvas *canvas = (Canvas *)canvas_arg;
            vector<vector<vector<int>>> prev_frame = frame.blank_frame;
            int sum_time = 0;
            int sum_time2 = 0;
            for(int i=0; i<frame_num; i++) {
        	auto start = high_resolution_clock::now();
                evolve();
                frame.create(particles);
            
        	auto start2 = high_resolution_clock::now();
                for (int i=size-4; i<size; ++i) {
                    for (int j=size-4; j<size; ++j) {
                        frame.frame[i][j] = mode_col;
                    }
                }
            
                for (int i=0; i<size; ++i) {
                    for (int j=0; j<size; ++j) {
        		if (frame.frame[i][j] != prev_frame[i][j]) {
                            canvas -> SetPixel(i,j, frame.frame[i][j][0], frame.frame[i][j][1], frame.frame[i][j][2]);
        		}
                    }
                }
        
        	prev_frame = frame.frame;
        	auto stop = high_resolution_clock::now();
        	auto duration = duration_cast<milliseconds>(stop - start);
        	auto duration2 = duration_cast<milliseconds>(stop - start2);
        	sum_time += duration.count();
        	sum_time2 += duration2.count();
                 
                msleep(interval - duration.count());
                frame.clear();
                if (PLAY_FLAG) {
                    return;
                }
            }
            cout<<"bounce time"<<endl;
            cout<< sum_time/frame_num<<endl;
            cout<< sum_time2/frame_num<<endl;
        }
};

class RunScatter:public Scatter {
    public:
        void run (void * canvas_arg) override {
	    Canvas *canvas = (Canvas *)canvas_arg;
            vector<vector<vector<int>>> prev_frame = frame.blank_frame;
            vector<vector<vector<int>>> prev_static_frame = frame.blank_frame;
            int count = 0;
            int sum_time = 0;
            int sum_time2 = 0;
            for (int i=0; i<size; ++i) {
                for (int j=0; j<size; ++j) {
                    if (pin_frame[j][i]) {
                        canvas -> SetPixel(i, j, pin_col[0], pin_col[1], pin_col[2]); 
                    }
        	}
            }
            while (balls.size()) {
        	++count;
        	auto start = high_resolution_clock::now();
                evolve();
                frame.create(balls);
        	auto start2 = high_resolution_clock::now();
            
                for (int i=0; i<size; ++i) {
                    for (int j=0; j<size; ++j) {
                	if (static_balls_frame[i][j] != prev_static_frame[i][j]) {
                            canvas -> SetPixel(i, j, static_balls_frame[i][j][0], static_balls_frame[i][j][1], static_balls_frame[i][j][2]);
                	}
                	else if (frame.frame[i][j] != prev_frame[i][j]) {
                            canvas -> SetPixel(i, j, frame.frame[i][j][0], frame.frame[i][j][1], frame.frame[i][j][2]);
                	}
                    }
                }
        	prev_frame = frame.frame;
        	prev_static_frame = static_balls_frame;
        	auto stop = high_resolution_clock::now();
        	auto duration = duration_cast<milliseconds>(stop - start);
        	auto duration2 = duration_cast<milliseconds>(stop - start2);
        	sum_time += duration.count();
        	sum_time2 += duration2.count();
                msleep(interval-duration.count());
                frame.clear();
                if (PLAY_FLAG) {
                    return;
                }
            }
            cout<<"scatter time"<<endl;
            cout<< sum_time/count <<endl;
            cout<< sum_time2/count <<endl;
        }
};

class RunStar:public Star {
    public:
        void run (void *canvas_arg) override {
	    Canvas *canvas = (Canvas *)canvas_arg;
            int sum_time = 0;
            int sum_time2 = 0;
            for (int n=0; n<frame_num; n++) {
        	auto start = high_resolution_clock::now();
                evolve();
        	auto start2 = high_resolution_clock::now();
                for (int i=0; i<stars.size(); ++i) {
                    canvas -> SetPixel(stars[i].pos[0], stars[i].pos[1], stars[i].col[0], stars[i].col[1], stars[i].col[2]);
                }
        	auto stop = high_resolution_clock::now();
        	auto duration = duration_cast<milliseconds>(stop - start);
        	auto duration2 = duration_cast<milliseconds>(stop - start2);
        	sum_time += duration.count();
        	sum_time2 += duration2.count();
                msleep(interval-duration.count());
                if (PLAY_FLAG) {
                    return;
                }
            }
            cout<<"bounce time"<<endl;
            cout<< sum_time/frame_num<<endl;
            cout<< sum_time2/frame_num<<endl;
        }
};

int main(int argc, char * argv[]) {
    cout << argc << endl;
    cout << "initialising..." << endl;
    srand(time(0));

    thread thread_obj(trd, "var/play_flag", 1000);
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular";
    defaults.rows = 64;
    defaults.cols = 64;
    defaults.chain_length = 1;
    defaults.parallel = 1;
    defaults.show_refresh_rate = false;
    defaults.scan_mode = 0;
    rgb_matrix::RuntimeOptions run_defaults;
    run_defaults.gpio_slowdown = 0;
    run_defaults.daemon = false;
    run_defaults.drop_privileges = false;
    //defaults.limit_refresh_rate_hz = 75;
    
    RunConway conway;
    conway.frame_num = 3000;
    conway.interval = 110;
    RunAnt ant;
    ant.frame_num = 4000;
    ant.interval = 50;
    RunSand sand;
    sand.interval = 20;
    RunBounce bounce;
    bounce.interval = 70;
    bounce.num_balls = 200;
    bounce.frame_num = 4000;
    RunScatter scatter;
    scatter.interval = 90;
    RunStar star;
    star.interval = 15;
    star.decay_prob = 10;
    star.max_brightness = 180;
    star.num_stars = 160;
    star.frame_num = 10000;

    vector<Pattern*> patterns = {&conway, &ant, &sand, &bounce, &scatter, &star};

    while (true) {
        while(PLAY_FLAG) {
            msleep(1000);
        }
        Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults, &run_defaults);
	scatter.num_streams = 1 + rand()%2;
	sand.wild = (rand()%2 == 0);


	for (int i=0; i<patterns.size(); ++i) {
            cout << "starting new pattern" << endl;
	    patterns[i] -> initialise();
	    patterns[i] -> run(canvas);
            canvas -> Clear();
            if(PLAY_FLAG) {
                cout << "spotify flag is detected, killing patterns" << endl;
                break;
            }
	}
        delete canvas;
    }
    return 0;
}
