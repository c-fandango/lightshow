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

struct ant_struct{
  int xpos, ypos, orientation, col_r, col_g,col_b;
  vector<vector<int>> frame;
};

vector<vector<int>> initialise(int num){
  vector<vector<int>> output(size,vector<int>(size,0));
  for (int i=size/2 -num/2; i<size/2 +num/2 +1; ++i){
    for (int j=size/2 -num/2; j<size/2 +num/2 +1;++j){
        output[j][i] = rand()%2;
  
    }
  }
  return output;
}

ant_struct next_frame(ant_struct input){
  switch(input.orientation){
    case 0:
      ++input.ypos;
      break;
    case 1:
      ++input.xpos;
      break;
    case 2:
      --input.ypos;
      break;
    case 3:
      --input.xpos;
      break;
  }
 
  switch(input.frame[input.ypos][input.xpos]){
    case 1:
      input.orientation = (input.orientation+3)%4;
      
      break;
    case 0:
      input.orientation = (input.orientation+5)%4;
      break;
  }
  ++input.frame[input.ypos][input.xpos];
  input.frame[input.ypos][input.xpos] %= 2 ;
  if (input.xpos==-1 || input.xpos==64 || input.ypos==-1 || input.ypos==64 ){
    input.col_r=rand()%256;
    input.col_g=rand()%256;
    input.col_b=rand()%256;
  }
  input.xpos= (input.xpos +size) %size;
  input.ypos= (input.ypos +size) %size;
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
  defaults.show_refresh_rate = false;
  Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);

  conway_class con;
  int frame_num=4000;
  ant_struct ant;
  ant.xpos=32;
  ant.ypos=32;
  ant.orientation=0;
  ant.col_r = rand()%256;
  ant.col_g = rand()%256;
  ant.col_b = rand()%256;

  ant.frame = initialise(4);
  
  for(int n=0;n<frame_num;++n){
    ant=next_frame(ant);
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