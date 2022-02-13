#include "./libs/led-matrix.h"
#include <stdlib.h>
#include <time.h>
#include "./libs/functions.h"
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
/*twinkling stars?*/

string read_file(string path){
  string line, flag;
  ifstream file(path);
  if (file.is_open()){
    while (getline(file,line))
    {
      flag=line;
      cout<<flag<<'\n';
    }
    file.close();
  }
  else{
    cout<< "cant open file";
  }
  return flag;
}
void trd(string path){
  
  while (true){
    play_flag=read_file(path);
    usleep(100*1000);
  }
}

int main(int argc, char * argv[]){
  srand(time(0));
  
  std::thread thread_obj(trd,"/home/pi/Code/light-show/play_flag.txt");
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";
  defaults.rows = 64;
  defaults.cols=64;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;
  rgb_matrix::RuntimeOptions run_defaults;
  //run_defaults.daemon=true;
  run_defaults.drop_privileges=false;
  
  bool reset=false;

  while(true){
    
    reset=false;
    while(play_flag!="0"){
      usleep(50*1000);
    }
    Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults, &run_defaults );
    conway_class con;
    ant_class ant;
    sand_class sand;
    bounce_class bounce;
    scatter_class scatter;
    rain_class rain;

    con.frame_num=3000;
    con.col_r = rand()%256, con.col_g = rand()%256, con.col_b=rand()%256;
    con.frame_bc=con.wrapper(con.initialise(con.frame));

    for(int n=0;n<con.frame_num;++n){
      con.frame=con.next_frame(con.frame_bc);
      for (int i=0;i<con.size;++i){
        for(int j=0;j<con.size;++j){
          if(con.frame[i][j]==1){
            canvas-> SetPixel(i,j,con.col_r,con.col_g,con.col_b);
          }
        }
      }
      con.frame_bc=con.wrapper(con.frame);
      if(con.frame==con.frame_prev_2){
        break;
      }
      usleep(110*1000);
      canvas->Clear();
      con.frame_prev_2=con.frame_prev;
      con.frame_prev=con.frame;
      if (play_flag=="1"){
        reset =true;
        break;
      }
    }
    if(reset){
      delete canvas;
      continue;
    }

    ant.frame_num=4000;
    ant.col_r = rand()%256;
    ant.col_g = rand()%256;
    ant.col_b = rand()%256;

    ant.frame = ant.initialise(2);
    
    for(int n=0;n<ant.frame_num;++n){
      ant=ant.next_frame(ant);
      for (int i=0;i<ant.size;++i){
        for(int j=0;j<ant.size;++j){
          if(ant.frame[i][j]==1){
            canvas-> SetPixel(i,j,ant.col_r,ant.col_g,ant.col_b);
          }
        }
      }
      canvas-> SetPixel(ant.ypos,ant.xpos,255,255,255);
      usleep(40*1000);
      canvas->Clear();
      if (play_flag=="1"){
        break;
      }
    }
    if(reset){
      delete canvas;
      continue;
    }

    sand.col_1r = 0; sand.col_1g = 0; sand.col_1b = 80; 
    sand.col_2r = 0; sand.col_2g = 80, sand.col_2b = 40;
    sand.col_3r = 100; sand.col_3g = 100; sand.col_3b = 0;
    sand.col_4r = 255; sand.col_4g=0; sand.col_4b=0;
    if (rand()%3==0){
      sand.wild=false;
    }
    while(sand.frame[sand.size/4][sand.size/2] == 0){
      if (sand.stable){
          sand = sand.next_grain(sand);
      }
      else{
        sand=sand.evolve(sand);
      }
      for (int i=0;i<sand.size;++i){
        for (int j=0;j<sand.size;++j){ 
          switch(sand.frame[i][j]){
            case 0:
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
      canvas->Clear();
      if (play_flag=="1"){
        break;
      }
    }
    if(reset){
      delete canvas;
      continue;
    }

    bounce.num_balls=200;
    
    vector<particle_class> balls=bounce.initialise(bounce.num_balls);
    
    for(int i=0;i<4000;i++){
      balls=bounce.next_frame(balls);
      bounce.mode=bounce.mode_func(balls);
      for(int i=0; i<balls.size();++i){
        canvas->SetPixel(balls[i].pos[0],balls[i].pos[1],balls[i].col[0],balls[i].col[1],balls[i].col[2]);
      }
      for (int i=bounce.size-4; i<bounce.size;++i){
        for (int j=bounce.size-4; j<bounce.size;++j){
          canvas->SetPixel(i,j,bounce.mode[0],bounce.mode[1],bounce.mode[2]);
        }
      }
      usleep(60*1000);
      canvas->Clear();
      if (play_flag=="1"){
        break;
      }
    }
    if(reset){
      delete canvas;
      continue;
    }
    scatter.ball_col1={255,0,0};
    scatter.ball_col2_1={0,130,100};
    scatter.ball_col2_2={0,180,20};
    scatter.pin_col={0,0,150};

    for (int k=0; k<3;++k){
      if (rand()%2==0){
        scatter.num_streams=1;
      }
      else{
        scatter.num_streams=2;
      }

      scatter=scatter.initialise(scatter);
      
      for (int n=0;n<scatter.num_balls*2 +scatter.size+2;++n){
        scatter=scatter.evolve(scatter);
        for (int i=0; i<scatter.balls.size();++i){
          if(scatter.balls[i].pos[0]>-1){
          canvas->SetPixel(scatter.balls[i].pos[1],scatter.balls[i].pos[0],scatter.balls[i].col[0],scatter.balls[i].col[1],scatter.balls[i].col[2]);
          }
        }
        for (int i=0; i<scatter.pins.size();++i){

          canvas->SetPixel(scatter.pins[i][1],scatter.pins[i][0],scatter.pin_col[0],scatter.pin_col[1],scatter.pin_col[2]);
        }
        usleep(90*1000);
        canvas->Clear();
        if (play_flag=="1"){
        reset=true;
        break;
        }
      }
      if (reset){
        break;
      }
    }
    if(reset){
      delete canvas;
      continue;
    }

    rain.surface_col={0,10,20};
    rain.water_col={0,0,16};
    rain.raise_level=250;

    while(rain.water_level != 0){
      rain=rain.evolve(rain);
      
      for (int i =0; i<rain.size;++i){
        
        for (int j=rain.size; j>rain.water_level+rain.water_surface;--j){
          canvas->SetPixel(i,j,rain.water_col[0],rain.water_col[1],rain.water_col[2]);
        }
        for (int j=rain.water_level; j<rain.water_level+rain.water_surface+1;++j){
          canvas->SetPixel(i,j,rain.surface_col[0],rain.surface_col[1],rain.surface_col[2]);
        }
      }
      for (int i=0; i<rain.max_drops;++i){
        if(rain.rain[i].action){
          canvas->SetPixel(rain.rain[i].pos[1]-1,rain.rain[i].pos[0]-1,rain.rain_col[0],rain.rain_col[1],rain.rain_col[2]);
          canvas->SetPixel(rain.rain[i].pos[1]-1,rain.rain[i].pos[0]+1,rain.rain_col[0],rain.rain_col[1],rain.rain_col[2]);
          canvas->SetPixel(rain.rain[i].pos[1]+1,rain.rain[i].pos[0]-1,rain.rain_col[0],rain.rain_col[1],rain.rain_col[2]);
          canvas->SetPixel(rain.rain[i].pos[1]+1,rain.rain[i].pos[0]+1,rain.rain_col[0],rain.rain_col[1],rain.rain_col[2]);
        }
        else if(rain.rain[i].pos[0]>-1){
          canvas->SetPixel(rain.rain[i].pos[1],rain.rain[i].pos[0],rain.rain_col[0],rain.rain_col[1],rain.rain_col[2]);
          canvas->SetPixel(rain.rain[i].pos[1],rain.rain[i].pos[0]-1,int(rain.rain_col[0]*0.5),int(rain.rain_col[1]*0.5),int(rain.rain_col[2]*0.5));
          canvas->SetPixel(rain.rain[i].pos[1],rain.rain[i].pos[0]-2,int(rain.rain_col[0]*0.2),int(rain.rain_col[1]*0.2),int(rain.rain_col[2]*0.2));
        }
      }
      usleep(12*1000);
      canvas->Clear();
      if (play_flag=="1"){
        reset=true;
        break;
      }
    }

    delete canvas;
  }
  return 0;
}
