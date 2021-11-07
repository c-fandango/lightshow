#include "/home/pi/Code/light-show/libs/led-matrix.h"
#include <stdlib.h>
#include <array>
#include <time.h>
#include "functions.h"
#include<unistd.h>
#include <vector>
#include <iostream>
using namespace std;
using namespace rgb_matrix;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;





int main(int argc, char * argv[]){
  srand(time(0));
  
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";
  defaults.rows = size;
  defaults.cols=size;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = false;
  Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);

  conway_class con;
  ant_class ant;
  sand_class sand;

  
  sand.frame = vector<vector<int>>(size,vector<int>(size,0));
  sand.wild=true;
  sand.stable=true;
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
  ant.xpos=32;
  ant.ypos=32;
  ant.orientation=0;
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