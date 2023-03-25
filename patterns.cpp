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

void run_conway(Conway con, Canvas * canvas, int interval) {
    int sum_time =0;
    int sum_time2 =0;
    for(int n=0; n<con.frame_num; ++n) {
	auto start = high_resolution_clock::now();
        con.evolve();
	auto start2 = high_resolution_clock::now();
        for (int i=0; i<con.size; ++i) {
            for (int j=0; j<con.size; ++j) {
		if (con.frame.sframe[i][j] && !con.old_frames[0][i][j]) {
                    canvas -> SetPixel(i, j, con.global_col[0], con.global_col[1], con.global_col[2]);
		}
		else if (!con.frame.sframe[i][j] && con.old_frames[0][i][j]) {
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
	if (con.stable || PLAY_FLAG){
            cout<< sum_time/n<<endl;
            cout<< sum_time2/n<<endl;
	    return;
	}
    }
    cout<<"conway time"<<endl;
    cout<< sum_time/con.frame_num<<endl;
    cout<< sum_time2/con.frame_num<<endl;
}

void run_ant (Ant ant, Canvas * canvas, int interval) {
    vector<vector<int>> prev_frame = ant.frame.blank_sframe;
    int sum_time =0;
    int sum_time2 =0;
    for(int n=0; n<ant.frame_num; ++n) {
	auto start = high_resolution_clock::now();
        ant.evolve();
	auto start2 = high_resolution_clock::now();
        for (int i=0; i<ant.size; ++i) {
            for (int j=0; j<ant.size; ++j) {
		if (ant.frame.sframe[i][j] && !prev_frame[i][j]) {
                    canvas -> SetPixel(i, j, ant.global_col[0], ant.global_col[1], ant.global_col[2]);
		}
		else if (prev_frame[i][j] && !ant.frame.sframe[i][j]) {
                    canvas -> SetPixel(i,j, 0, 0, 0); 
		}
            }
        }
        canvas -> SetPixel(ant.particle.pos[1], ant.particle.pos[0], ant.particle.col[0], ant.particle.col[1], ant.particle.col[2]);
	prev_frame = ant.frame.sframe;
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
    cout<< sum_time/ant.frame_num<<endl;
    cout<< sum_time2/ant.frame_num<<endl;
}

void run_sand (Sand sand, Canvas * canvas, int interval) {
    int height, prev_height;
    vector<vector<int>> prev_frame = sand.frame.blank_sframe;
    int count =0;
    int sum_time =0;
    int sum_time2 =0;
    while(!sand.frame.sframe[sand.size/4][sand.size/2]) {
	++count;
	auto start = high_resolution_clock::now();
        if (sand.stable) {
            sand.next_grain();
        }
        else {
            sand.evolve();
        }
	auto start2 = high_resolution_clock::now();
        for (int i=0; i<sand.size; ++i) {
            for (int j=0; j<sand.size; ++j) {
		height = min(sand.frame.sframe[i][j], 4);
		prev_height = min(prev_frame[i][j], 4);
		if (height != prev_height) {
                    canvas -> SetPixel(i, j, sand.cols[height][0], sand.cols[height][1], sand.cols[height][2]);
		}
            }
        }
	prev_frame = sand.frame.sframe;
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

void run_bounce (Bounce bounce, Canvas * canvas, int interval) {
    vector<vector<vector<int>>> prev_frame = bounce.frame.blank_frame;
    int sum_time =0;
    int sum_time2 =0;
    for(int i=0; i<bounce.frame_num; i++) {
	auto start = high_resolution_clock::now();
        bounce.evolve();
        bounce.frame.create(bounce.particles);
    
	auto start2 = high_resolution_clock::now();
        for (int i=bounce.size-4; i<bounce.size; ++i) {
            for (int j=bounce.size-4; j<bounce.size; ++j) {
                bounce.frame.frame[i][j] = bounce.mode_col;
            }
        }
    
        for (int i=0; i<bounce.size; ++i) {
            for (int j=0; j<bounce.size; ++j) {
		if (bounce.frame.frame[i][j] != prev_frame[i][j]) {
                    canvas -> SetPixel(i,j, bounce.frame.frame[i][j][0], bounce.frame.frame[i][j][1], bounce.frame.frame[i][j][2]);
		}
            }
        }

	prev_frame = bounce.frame.frame;
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	auto duration2 = duration_cast<milliseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();
         
        msleep(interval - duration.count());
        bounce.frame.clear();
        if (PLAY_FLAG) {
            return;
        }
    }
    cout<<"bounce time"<<endl;
    cout<< sum_time/bounce.frame_num<<endl;
    cout<< sum_time2/bounce.frame_num<<endl;
}

void run_scatter (Scatter scatter, Canvas * canvas, int interval) {
    vector<vector<vector<int>>> prev_frame = scatter.frame.blank_frame;
    vector<vector<vector<int>>> prev_static_frame = scatter.frame.blank_frame;
    int count =0;
    int sum_time =0;
    int sum_time2 =0;
    for (int i=0; i<scatter.size; ++i) {
        for (int j=0; j<scatter.size; ++j) {
            if (scatter.pin_frame[j][i]) {
                canvas -> SetPixel(i, j, scatter.pin_col[0], scatter.pin_col[1], scatter.pin_col[2]); 
            }
	}
    }
    while (scatter.balls.size()) {
	++count;
	auto start = high_resolution_clock::now();
        scatter.evolve();
        scatter.frame.create(scatter.balls);
	auto start2 = high_resolution_clock::now();
    
        for (int i=0; i<scatter.size; ++i) {
            for (int j=0; j<scatter.size; ++j) {
        	if (scatter.static_balls_frame[i][j] != prev_static_frame[i][j]) {
                    canvas -> SetPixel(i, j, scatter.static_balls_frame[i][j][0], scatter.static_balls_frame[i][j][1], scatter.static_balls_frame[i][j][2]);
        	}
        	else if (scatter.frame.frame[i][j] != prev_frame[i][j]) {
                    canvas -> SetPixel(i, j, scatter.frame.frame[i][j][0], scatter.frame.frame[i][j][1], scatter.frame.frame[i][j][2]);
        	}
            }
        }
	prev_frame = scatter.frame.frame;
	prev_static_frame = scatter.static_balls_frame;
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	auto duration2 = duration_cast<milliseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();
        msleep(interval-duration.count());
        scatter.frame.clear();
        if (PLAY_FLAG) {
            return;
        }
    }
    cout<<"scatter time"<<endl;
    cout<< sum_time/count<<endl;
    cout<< sum_time2/count<<endl;
}
void run_star (Star star, Canvas * canvas, int interval){
    int sum_time =0;
    int sum_time2 =0;
    for (int n=0; n<star.frame_num; n++) {
	auto start = high_resolution_clock::now();
        star.evolve();
	auto start2 = high_resolution_clock::now();
        for (int i=0; i<star.stars.size(); ++i) {
            canvas -> SetPixel(star.stars[i].pos[0], star.stars[i].pos[1], star.stars[i].col[0], star.stars[i].col[1], star.stars[i].col[2]);
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
    cout<< sum_time/star.frame_num<<endl;
    cout<< sum_time2/star.frame_num<<endl;
}

int main(int argc, char * argv[]) {
    cout << argc << endl;
    cout << "initialising..." << endl;
    srand(time(0));

    thread thread_obj(trd, "var/play_flag", 100);
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
    
    Conway conway;
    conway.frame_num = 3000;
    Ant ant;
    ant.frame_num = 4000;
    Sand sand;
    Bounce bounce;
    bounce.num_balls = 200;
    bounce.frame_num = 4000;
    Scatter scatter;
    Star star;
    star.decay_prob = 10;
    star.max_brightness = 180;
    star.num_stars = 160;
    star.frame_num = 10000;

    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults, &run_defaults);

    while (true) {
        while(PLAY_FLAG) {
            msleep(50);
        }

        cout << "starting conway" << endl;
        conway.initialise();

        run_conway(conway, canvas, 110);
        if(PLAY_FLAG) {
            cout << "spotify flag is detected, killing patterns" << endl;
            continue;
        }

        canvas -> Clear();

        cout << "starting ant pattern" << endl;
        ant.initialise();

	run_ant(ant, canvas, 50);

        if(PLAY_FLAG) {
            cout << "spotify flag is detected, killing patterns" << endl;
            continue;
        }

        cout << "starting sand pattern" << endl;
        canvas -> Clear();

        sand.wild = (rand()%3 == 0);

	run_sand(sand, canvas, 40);
        if (PLAY_FLAG) {
            cout << "spotify flag is detected, killing patterns" << endl;
            continue;
        }

        canvas -> Clear();

        cout << "starting ball pattern" << endl;
        bounce.initialise();
	run_bounce(bounce, canvas, 60);
        if(PLAY_FLAG) {
            cout << "spotify flag is detected, killing patterns" << endl;
            continue;
        }

        canvas -> Clear();

        cout << "starting scatter pattern" << endl;
        for (int k=0; k<3; ++k) {
            scatter.initialise(1 + rand()%2);
	    run_scatter(scatter, canvas, 90);
	    canvas -> Clear();
            if(PLAY_FLAG) {
                cout << "spotify flag is detected, killing patterns" << endl;
                continue;
            }
	}

        canvas -> Clear();
	
        cout << "starting star pattern" << endl;
        star.initialise();
	run_star(star, canvas, 15);
        if(PLAY_FLAG) {
            cout << "spotify flag is detected, killing patterns" << endl;
            continue;
        }

        cout << "end of patterns, restarting" << endl;
    }
    return 0;
}
