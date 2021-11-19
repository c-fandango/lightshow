#include "/home/pi/Code/light-show/libs/led-matrix.h"
#include <stdlib.h>
#include <array>
#include<cmath>
#include <time.h>
#include "functions.h"
#include<unistd.h>
#include <vector>
#include <iostream>
using namespace std;
using namespace rgb_matrix;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
/*twinkling stars?*/

 class rain_class{
   public:
    int spawn_prob=80;
    int water_level=size-1;
    int water_surface_ideal=8;
    int water_surface=8;
    vector<int> rain_col={110,150,180};
    vector<int> water_col={0,0,14};
    vector<int> surface_col={0,20,20};
    int max_drops=550;
    int counter=0;
    int raise_level=50;
    particle_class drop;
   
    vector<particle_class> rain= vector<particle_class>(max_drops,drop);
    rain_class evolve(rain_class input);
  };

rain_class rain_class::evolve(rain_class input){
  int start_pos;
  
  for (int i =0; i<input.max_drops; ++i){
    if (input.rain[i].pos[0]==-1 && rand()%spawn_prob==0){
      start_pos=rand()%size;
      for(int j=0; j<input.max_drops; ++j){
        if (input.rain[j].pos[0]>-1 &&  input.rain[j].pos[0]<4 && abs(input.rain[j].pos[1] -start_pos)<2){
          break;
        }
        else if (j==input.max_drops-1){
          input.rain[i].pos={0,start_pos};
        } 
      }
    }
  }
  for (int i=0; i<input.max_drops;++i){
    if (input.rain[i].pos[0] > -1 && !(input.rain[i].action) && input.rain[i].pos[0]!=input.water_level+input.water_surface){
      ++input.rain[i].pos[0];
    }
    if (input.rain[i].action){
      input.rain[i].pos={-1,0};
      input.rain[i].action=false;
    }
    else if (input.rain[i].pos[0]==-1){
      continue;
    }
    else if (input.rain[i].pos[0]<input.water_level){
      continue;
    }
    else if (input.rain[i].pos[0]==size-1 || input.rain[i].pos[0]==input.water_level+input.water_surface){
      input.rain[i].action =true;
      ++input.counter;
    }
    else if(input.rain[i].pos[0]<input.water_level+input.water_surface && rand()%(input.water_surface-3)==0){
      input.rain[i].action=true;
      ++input.counter;
    }
  }
  if (input.counter>input.raise_level){
    --input.water_level;
    input.counter=0;
  }

  return input;

}

int main(int argc, char * argv[]){
  srand(time(0));
  
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";
  defaults.rows = size;
  defaults.cols=size;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;
  Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);

  conway_class con;
  ant_class ant;
  sand_class sand;
  bounce_class bounce;
  scatter_class scatter;
  rain_class rain;

  rain.surface_col={0,20,20};
  rain.water_col={0,0,16};
  
  while(rain.water_level != 0){
    rain=rain.evolve(rain);
    
    for (int i =0; i<size;++i){
      
      for (int j=size; j>rain.water_level+rain.water_surface;--j){
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
    usleep(10*1000);
    canvas->Clear();
  }



  scatter.ball_col1={255,0,0};
  scatter.ball_col2_1={0,130,100};
  scatter.ball_col2_2={0,180,20};
  scatter.pin_col={0,0,150};

  if (rand()%2==0){
    scatter.num_streams=1;
  }
  else{
    scatter.num_streams=2;
  }

  scatter=scatter.initialise(scatter);
  
  for (int n=0;n<scatter.num_balls*2 +size+2;++n){
    scatter=scatter.evolve(scatter);
    for (int i=0; i<scatter.balls.size();++i){
      if(scatter.balls[i].pos[0]>-1){
      canvas->SetPixel(scatter.balls[i].pos[1],scatter.balls[i].pos[0],scatter.balls[i].col[0],scatter.balls[i].col[1],scatter.balls[i].col[2]);
      }
    }
    for (int i=0; i<scatter.pins.size();++i){

      canvas->SetPixel(scatter.pins[i][1],scatter.pins[i][0],scatter.pin_col[0],scatter.pin_col[1],scatter.pin_col[2]);
    }
    usleep(100*1000);
    canvas->Clear();
  }

  bounce.num_balls=200;
  
  vector<particle_class> balls=bounce.initialise(bounce.num_balls);
  
  for(int i=0;i<4000;i++){
    balls=bounce.next_frame(balls);
    bounce.mode=bounce.mode_func(balls);
    for(int i=0; i<balls.size();++i){
      canvas->SetPixel(balls[i].pos[0],balls[i].pos[1],balls[i].col[0],balls[i].col[1],balls[i].col[2]);
    }
    for (int i=size-4; i<size;++i){
      for (int j=size-4; j<size;++j){
        canvas->SetPixel(i,j,bounce.mode[0],bounce.mode[1],bounce.mode[2]);
      }
    }
    usleep(80*1000);
    canvas->Clear();
  }

  sand.col_1r = 0; sand.col_1g = 0; sand.col_1b = 60; 
  sand.col_2r = 0; sand.col_2g = 60, sand.col_2b = 40;
  sand.col_3r = 100; sand.col_3g = 0; sand.col_3b = 130;
  sand.col_4r = 255; sand.col_4g=0; sand.col_4b=0;
  if (rand()%3==0){
    sand.wild=false;
  }
  while(sand.frame[size/4][size/2] == 0){
    if (sand.stable){
        sand = sand.next_grain(sand);
    }
    else{
      sand=sand.evolve(sand);
    }
    for (int i=0;i<size;++i){
       for (int j=0;j<size;++j){ 
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
    usleep(30*1000);
    canvas->Clear();
  }

  ant.frame_num=4000;
  ant.col_r = rand()%256;
  ant.col_g = rand()%256;
  ant.col_b = rand()%256;

  ant.frame = ant.initialise(4);
  
  for(int n=0;n<ant.frame_num;++n){
    ant=ant.next_frame(ant);
    for (int i=0;i<size;++i){
      for(int j=0;j<size;++j){
        if(ant.frame[i][j]==1){
          canvas-> SetPixel(i,j,ant.col_r,ant.col_g,ant.col_b);
        }
      }
    }
    canvas-> SetPixel(ant.ypos,ant.xpos,255,255,255);
    usleep(40*1000);
    canvas->Clear();
  }
  
  con.frame_num=3000;
  con.col_r = rand()%256, con.col_g = rand()%256, con.col_b=rand()%256;
  con.frame_bc=con.wrapper(con.initialise(con.frame));

  for(int n=0;n<con.frame_num;++n){
    con.frame=con.next_frame(con.frame_bc);
    for (int i=0;i<size;++i){
      for(int j=0;j<size;++j){
        if(con.frame[i][j]==1){
          canvas-> SetPixel(i,j,con.col_r,con.col_g,con.col_b);
        }
      }
    }
    con.frame_bc=con.wrapper(con.frame);
    if(con.frame==con.frame_prev_2){
      return 0;
    }
    usleep(110*1000);
    canvas->Clear();
    con.frame_prev_2=con.frame_prev;
    con.frame_prev=con.frame;
  }
  return 0;
}