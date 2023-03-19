#include "../include/led-matrix.h"
#include "../include/functions.h"
#include <stdlib.h>
//#include <unistd.h>
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
void usleep(int interval) {
    this_thread::sleep_for(milliseconds(interval));
}

void trd(string path, int interval) {
    while (true) {
        PLAY_FLAG = read_file(path);
        usleep(interval);
    }
}

bool run_conway(Conway con, Canvas * canvas, int interval) {
    int sum_time =0;
    int sum_time2 =0;
    for(int n=0; n<con.frame_num; ++n) {
	auto start = high_resolution_clock::now();
        con.evolve();
	auto start2 = high_resolution_clock::now();
        for (int i=0; i<con.size; ++i) {
            for (int j=0; j<con.size; ++j) {
		if (con.frame.sframe[i][j]) {
                    canvas -> SetPixel(i, j, con.global_col[0], con.global_col[1], con.global_col[2]);
		}
		else {
                    canvas -> SetPixel(i, j, 0, 0, 0); 
		}
            }
        }
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	auto duration2 = duration_cast<microseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();

        usleep(interval);
	if (con.stable){
            cout<< sum_time/n<<endl;
            cout<< sum_time2/n<<endl;
	    return false;
	}
	else if(PLAY_FLAG) {
            return true;
        }
    }
    cout<<"conway time"<<endl;
    cout<< sum_time/con.frame_num<<endl;
    cout<< sum_time2/con.frame_num<<endl;
    return false;
}

bool run_ant (Ant ant, Canvas * canvas, int interval) {
    int sum_time =0;
    int sum_time2 =0;
    for(int n=0; n<ant.frame_num; ++n) {
	auto start = high_resolution_clock::now();
        ant.evolve();
	auto start2 = high_resolution_clock::now();
        for (int i=0; i<ant.size; ++i) {
            for (int j=0; j<ant.size; ++j) {
		if (ant.frame.sframe[i][j]) {
                    canvas -> SetPixel(i, j, ant.global_col[0], ant.global_col[1], ant.global_col[2]);
		}
		else {
                    canvas -> SetPixel(i,j, 0, 0, 0); 
		}
            }
        }
        canvas -> SetPixel(ant.particle.pos[1], ant.particle.pos[0], ant.particle.col[0], ant.particle.col[1], ant.particle.col[2]);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	auto duration2 = duration_cast<microseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();

        usleep(interval);
        if (PLAY_FLAG) {
            return true;
        }
    }
    cout<<"ant time"<<endl;
    cout<< sum_time/ant.frame_num<<endl;
    cout<< sum_time2/ant.frame_num<<endl;
    return false;
}
bool run_sand (Sand sand, Canvas * canvas, int interval) {
    int height;
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
                canvas -> SetPixel(i, j, sand.cols[height][0], sand.cols[height][1], sand.cols[height][2]);
            }
        }
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	auto duration2 = duration_cast<microseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();
        usleep(interval);
        if (PLAY_FLAG) {
            return true;
        }
    }
    cout<<"sand time"<<endl;
    cout<< sum_time/count<<endl;
    cout<< sum_time2/count<<endl;
    return false;
}

bool run_bounce (Bounce bounce, Canvas * canvas, int interval) {
    vector<int> mode;
    int sum_time =0;
    int sum_time2 =0;
    for(int i=0; i<bounce.frame_num; i++) {
	auto start = high_resolution_clock::now();
        bounce.evolve();
        mode = bounce.mode(bounce.particles);
        bounce.frame.create(bounce.particles);
    
	auto start2 = high_resolution_clock::now();
        for (int i=bounce.size-4; i<bounce.size; ++i) {
            for (int j=bounce.size-4; j<bounce.size; ++j) {
                bounce.frame.frame[i][j] = mode;
            }
        }
    
        for (int i=0; i<bounce.size; ++i) {
            for (int j=0; j<bounce.size; ++j) {
                canvas -> SetPixel(i,j, bounce.frame.frame[i][j][0], bounce.frame.frame[i][j][1], bounce.frame.frame[i][j][2]);
            }
        }
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	auto duration2 = duration_cast<microseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();
    
        usleep(interval);
        bounce.frame.clear();
        if (PLAY_FLAG) {
            return true;
        }
    }
    cout<<"bounce time"<<endl;
    cout<< sum_time/bounce.frame_num<<endl;
    cout<< sum_time2/bounce.frame_num<<endl;
    return false;
}

bool run_scatter (Scatter scatter, Canvas * canvas, int interval) {
    int count =0;
    int sum_time =0;
    int sum_time2 =0;
    while (scatter.balls.size()) {
	++count;
	auto start = high_resolution_clock::now();
        scatter.evolve();
        scatter.frame.create(scatter.balls);
	auto start2 = high_resolution_clock::now();
    
        for (int i=0; i<scatter.size; ++i) {
            for (int j=0; j<scatter.size; ++j) {
        	if (scatter.pin_frame[j][i]) {
                    canvas -> SetPixel(i, j, scatter.pin_col[0], scatter.pin_col[1], scatter.pin_col[2]); 
        	}
        	else if (scatter.static_balls_frame[i][j] != scatter.frame.black) {
                    canvas -> SetPixel(i, j, scatter.static_balls_frame[i][j][0], scatter.static_balls_frame[i][j][1], scatter.static_balls_frame[i][j][2]);
        	}
        	else {
                    canvas -> SetPixel(i, j, scatter.frame.frame[i][j][0], scatter.frame.frame[i][j][1], scatter.frame.frame[i][j][2]);
        	}
            }
        }
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	auto duration2 = duration_cast<microseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();
        usleep(interval);
        scatter.frame.clear();
        if (PLAY_FLAG) {
            return true;
        }
    }
    cout<<"scatter time"<<endl;
    cout<< sum_time/count<<endl;
    cout<< sum_time2/count<<endl;
    return false;
}
bool run_star (Star star, Canvas * canvas, int interval){
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
	auto duration = duration_cast<microseconds>(stop - start);
	auto duration2 = duration_cast<microseconds>(stop - start2);
	sum_time += duration.count();
	sum_time2 += duration2.count();
        usleep(interval);
        if (PLAY_FLAG) {
            return true;
        }
    }
    cout<<"bounce time"<<endl;
    cout<< sum_time/star.frame_num<<endl;
    cout<< sum_time2/star.frame_num<<endl;
    return false;
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
    run_defaults.gpio_slowdown = 2;
    run_defaults.daemon = false;
    run_defaults.drop_privileges = false;
    //defaults.limit_refresh_rate_hz = 75;

    while (true) {
        while(PLAY_FLAG) {
            usleep(50);
        }
        Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults, &run_defaults);
        Conway conway;
        Ant ant;
        Sand sand;
        Bounce bounce;
        Scatter scatter;
        Star star;
        Frame vframe;

        cout << "starting conway" << endl;
        conway.frame_num = 3000;
        conway.initialise();

	bool tst = run_conway(conway, canvas, 110);
        if(tst) {
            cout << "spotify flag is detected, killing patterns" << endl;
            delete canvas;
            continue;
        }

        cout << "starting ant pattern" << endl;
        ant.frame_num = 4000;
        ant.initialise();

	bool tst1 = run_ant(ant, canvas, 50);

        if(tst1) {
            cout << "spotify flag is detected, killing patterns" << endl;
            delete canvas;
            continue;
        }

        cout << "starting sand pattern" << endl;
        canvas -> Clear();

        sand.wild = (rand()%3 == 0);

	bool tst2 = run_sand(sand, canvas, 40);
        if (tst2) {
            cout << "spotify flag is detected, killing patterns" << endl;
            delete canvas;
            continue;
        }

        cout << "starting ball pattern" << endl;
        bounce.num_balls = 200;
	bounce.frame_num = 4000;
        bounce.initialise();
	bool tst3 = run_bounce(bounce, canvas, 60);
        if(tst3) {
            cout << "spotify flag is detected, killing patterns" << endl;
            delete canvas;
            continue;
        }

	bool tst4;
        cout << "starting scatter pattern" << endl;
        for (int k=0; k<3; ++k) {
            scatter.initialise(1 + rand()%2);
	    tst4 = run_scatter(scatter, canvas, 90);
	    canvas -> Clear();
            if(tst4) {
                cout << "spotify flag is detected, killing patterns" << endl;
                delete canvas;
                continue;
            }
	}
	
        cout << "starting star pattern" << endl;
        star.decay_prob = 10;
        star.max_brightness = 180;
        star.num_stars = 160;
	star.frame_num = 10000;
        star.initialise();
	bool tst5 = run_star(star, canvas, 15);
        if(tst5) {
            cout << "spotify flag is detected, killing patterns" << endl;
            delete canvas;
            continue;
        }


        delete canvas;
        cout << "end of patterns, restarting" << endl;
    }
    return 0;
}
