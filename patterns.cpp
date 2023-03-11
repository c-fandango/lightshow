#include "../include/led-matrix.h"
#include <stdlib.h>
#include <time.h>
#include "../include/functions.h"
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <string>
using namespace std;
using namespace rgb_matrix;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

string play_flag="1";

string read_file(string path) {
    string line, flag;
    ifstream file(path);
    cout<<file.good()<<endl;
    if (file.good() && file.is_open()) {
        while (getline(file,line))
        {
            flag=line;
        }
    }
    else {
	flag="0";
        cout<< "file does not exist or failed to open file";
    }
    file.close();
    return flag;
}
void trd(string path) {

    while (true) {
        play_flag=read_file(path);
        usleep(100*1000);
    }
}

int main(int argc, char * argv[]) {
    cout<<argc<<endl;
    cout<<"initialising..."<<endl;
    srand(time(0));

    std::thread thread_obj(trd,"var/play_flag");
    RGBMatrix::Options defaults;
    defaults.hardware_mapping = "regular";
    defaults.rows = 64;
    defaults.cols=64;
    defaults.chain_length = 1;
    defaults.parallel = 1;
    defaults.show_refresh_rate = false;
    defaults.scan_mode=0;
    rgb_matrix::RuntimeOptions run_defaults;
    run_defaults.gpio_slowdown = 2;
    run_defaults.daemon=false;
    run_defaults.drop_privileges=false;
    //defaults.limit_refresh_rate_hz =75;

    bool reset=false;

    while(true) {

        reset=false;
        while(play_flag!="0") {
            usleep(50*1000);
        }
        Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults, &run_defaults );
        conway_class con;
        ant_class ant;
        sand_class sand;
        bounce_class bounce;
        scatter_class scatter;
        rain_class rain;
        star_class star;
        virtual_frame_class vframe;

        cout<<"starting conway"<<endl;
        con.frame_num=3000;
        con.col = {rand()%256, rand()%256, rand()%256};
        con.initialise();
        for(int n=0; n<con.frame_num; ++n) {
            con.next_frame();
            for (int i=0; i<con.size; ++i) {
                for(int j=0; j<con.size; ++j) {
                    if(con.frame[i][j]==1) {
                        vframe.frame[i][j]=con.col;
                    }
                }
            }
            vframe.wild(con.wild);
            for (int i=0; i< vframe.size; ++i) {
                for (int j=0; j<vframe.size; ++j) {
                    canvas->SetPixel(i,j, vframe.frame[i][j][0], vframe.frame[i][j][1], vframe.frame[i][j][2]);
                }
            }
            usleep(110*1000);
            vframe.clear();
            if(con.stable) {
                break;
            }
            if (play_flag=="1") {
                reset =true;
                break;
            }
        }
        if(reset) {
            cout<<"spotify flag is detected, killing patterns"<<endl;
            delete canvas;
            continue;
        }
        cout<<"starting ant pattern"<<endl;
        ant.frame_num=4000;
        ant.start_num=2;
        ant.trail_col = {rand()%256,rand()%256,rand()%256};
        ant.col = {255,255,255};
        ant.initialise();

        for(int n=0; n<ant.frame_num; ++n) {
            ant.next_frame();
            for (int i=0; i<ant.size; ++i) {
                for(int j=0; j<ant.size; ++j) {
                    if(ant.frame[i][j]==1) {
                        vframe.frame[i][j]= ant.trail_col;
                    }
                }
            }
            vframe.frame[ant.ypos][ant.xpos] = ant.col;
            for (int i=0; i< vframe.size; ++i) {
                for (int j=0; j<vframe.size; ++j) {
                    canvas->SetPixel(i,j, vframe.frame[i][j][0], vframe.frame[i][j][1], vframe.frame[i][j][2]);
                }
            }

            usleep(40*1000);
            vframe.clear();
            if (play_flag=="1") {
                break;
            }
        }
        if(reset) {
            cout<<"spotify flag is detected, killing patterns"<<endl;
            delete canvas;
            continue;
        }
        cout<<"starting sand pattern"<<endl;
        canvas->Clear();
        sand.col_1r = 0;
        sand.col_1g = 0;
        sand.col_1b = 80;
        sand.col_2r = 0;
        sand.col_2g = 80, sand.col_2b = 40;
        sand.col_3r = 100;
        sand.col_3g = 100;
        sand.col_3b = 0;
        sand.col_4r = 255;
        sand.col_4g=0;
        sand.col_4b=0;
        if (rand()%3==0) {
            sand.wild=false;
        }
        while(sand.pile[sand.size/4][sand.size/2] == 0) {
            if (sand.stable) {
                sand.next_grain();
            }
            else {
                sand.evolve();
            }
            for (int i=0; i<sand.size; ++i) {
                for (int j=0; j<sand.size; ++j) {
                    switch(sand.pile[i][j]) {
                    case 0:
                        canvas-> SetPixel(i,j,0,0,0);
                        break;
                    case 1:
                        canvas-> SetPixel(i,j,sand.col_1r,sand.col_1g,sand.col_1b);
                        break;
                    case 2:
                        canvas-> SetPixel(i,j,sand.col_2r,sand.col_2g,sand.col_2b);
                        break;
                    case 3:
                        canvas-> SetPixel(i,j,sand.col_3r,sand.col_3g,sand.col_3b);
                        break;
                    default:
                        canvas-> SetPixel(i,j,sand.col_4r,sand.col_4g,sand.col_4b);
                        break;
                    }
                }
            }
            usleep(20*1000);
            if (play_flag=="1") {
                break;
            }
        }
        if(reset) {
            cout<<"spotify flag is detected, killing patterns"<<endl;
            delete canvas;
            continue;
        }
        cout<<"starting ball pattern"<<endl;
        bounce.num_balls=200;
        bounce.initialise();

        for(int i=0; i<4000; i++) {
            bounce.evolve();
            bounce.mode_func();
            vframe.create(bounce.balls);
            for (int i=bounce.size-4; i<bounce.size; ++i) {
                for (int j=bounce.size-4; j<bounce.size; ++j) {
                    vframe.frame[i][j] = bounce.mode;
                }
            }

            for (int i=0; i< vframe.size; ++i) {
                for (int j=0; j<vframe.size; ++j) {
                    canvas->SetPixel(i,j, vframe.frame[i][j][0], vframe.frame[i][j][1], vframe.frame[i][j][2]);
                }
            }

            usleep(60*1000);
            vframe.clear();
            if (play_flag=="1") {
                break;
            }
        }
        if(reset) {
            cout<<"spotify flag is detected, killing patterns"<<endl;
            delete canvas;
            continue;
        }

        cout<<"starting scatter pattern"<<endl;
        scatter.ball_col1= {255,0,0};
        scatter.ball_col2_1= {0,130,100};
        scatter.ball_col2_1= {120,190,21};
        scatter.ball_col2_2= {0,180,20};
        scatter.pin_col= {0,0,150};

        for (int k=0; k<3; ++k) {
            if (rand()%2==0) {
                scatter.num_streams=1;
            }
            else {
                scatter.num_streams=2;
            }

            scatter.initialise();

            for (int n=0; n<scatter.num_balls*2 +scatter.size+2; ++n) {
                scatter.evolve();
                vframe.create(scatter.balls);
                vframe.create(scatter.pins);
                for (int i=0; i<scatter.size; ++i) {
                    for (int j=0; j<scatter.size; ++j) {
                        canvas->SetPixel(i,j,vframe.frame[i][j][0],vframe.frame[i][j][1],vframe.frame[i][j][2]);
                    }
                }
                usleep(90*1000);
                vframe.clear();
                if (play_flag=="1") {
                    reset=true;
                    break;
                }
            }
            if (reset) {
                break;
            }
        }
        if(reset) {
            cout<<"spotify flag is detected, killing patterns"<<endl;
            delete canvas;
            continue;
        }
        cout<<"starting rain pattern"<<endl;
        rain.surface_col= {0,10,20};
        rain.water_col= {0,0,16};
        rain.raise_level=250;
        rain.initialise();
        while(rain.water_level != 1) {
            rain.evolve();
            for (int i =0; i<rain.size; ++i) {
                for (int j=rain.size; j>rain.water_level+rain.water_surface; --j) {
                    if ( j>-1 && j<64) {
                        vframe.frame[i][j] = rain.water_col;
                    }
                }
                for (int j=rain.water_level; j<rain.water_level+rain.water_surface+1; ++j) {
                    if (j>-1 && j<64) {
                        vframe.frame[i][j] = rain.surface_col;
                    }
                }
            }
            for (int i=0; i<rain.max_drops; ++i) {
                if(rain.rain[i].action) {
                    vframe.frame[rain.rain[i].pos[1]-1][rain.rain[i].pos[0]-1] = rain.rain[i].col;
                    vframe.frame[rain.rain[i].pos[1]-1][rain.rain[i].pos[0]+1] = rain.rain[i].col;
                    vframe.frame[rain.rain[i].pos[1]+1][rain.rain[i].pos[0]-1] = rain.rain[i].col;
                    vframe.frame[rain.rain[i].pos[1]+1][rain.rain[i].pos[0]+1] = rain.rain[i].col;
                }
                else if(rain.rain[i].pos[0]>-1) {
                    vframe.frame[rain.rain[i].pos[1]][rain.rain[i].pos[0]] = rain.rain[i].col;
                    vframe.frame[rain.rain[i].pos[1]][rain.rain[i].pos[0]-1] = {int(rain.rain[i].col[0]*0.5),int(rain.rain[i].col[1]*0.5),int(rain.rain[i].col[2]*0.5)};
                    vframe.frame[rain.rain[i].pos[1]][rain.rain[i].pos[0]-1] = {int(rain.rain[i].col[0]*0.2),int(rain.rain[i].col[1]*0.2),int(rain.rain[i].col[2]*0.2)};
                }
            }

            for (int i=0; i<rain.size; ++i) {
                for (int j=0; j<rain.size; ++j) {
                    canvas->SetPixel(i,j,vframe.frame[i][j][0],vframe.frame[i][j][1],vframe.frame[i][j][2]);
                }
            }
            usleep(12*1000);
            vframe.clear();
            if (play_flag=="1") {
                cout<<"spotify flag is detected, killing patterns"<<endl;
                reset=true;
                break;
            }
        }
        if(reset) {
            cout<<"spotify flag is detected, killing patterns"<<endl;
            delete canvas;
            continue;
        }
        canvas->Clear();
        star.decay_prob = 10;
        star.max_brightness = 180;
        star.num = 160;
        star.size = 64;
	star.frame_num=40000;
        star.initialise();
        for (int n=0; n<star.frame_num; n++) {
            star.evolve();
            for (int i=0; i<star.num; ++i) {
                canvas-> SetPixel(star.stars[i].pos[0], star.stars[i].pos[1], star.stars[i].col[0], star.stars[i].col[1], star.stars[i].col[2]);
            }
            usleep(15*1000);
            if (play_flag=="1") {
                cout<<"spotify flag is detected, killing patterns"<<endl;
                reset=true;
                break;
            }
        }

        delete canvas;
        cout<<"end of patterns, restarting"<<endl;
    }
    return 0;
}
