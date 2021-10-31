// -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
// Small example how to use the library.
// For more examples, look at demo-main.cc
//
// This code is public domain
// (but note, that the led-matrix library this depends on is GPL v2)

#include "/home/pi/Code/light-show/libs/led-matrix.h"
#include <stdlib.h>
#include <array>
#include <time.h>
#include<unistd.h>

using namespace std;
using namespace rgb_matrix;
using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;

int const size=64;

array<array<int,size>,size> initialise(array<array<int,size>,size> input){
  for(int i=0;i<size; ++i){
    for (int j=0; j<size;++j){
      input[i][j]= rand()%2;

    }
  }
  return input;
}

array<array<int,size+2>,size+2> wrapper(array<array<int,size>,size> input){
  array<array<int,size+2>,size+2> output={{}};
  for(int i=0;i<size;++i){
    for(int j=0; j<size; ++j){
      output[i+1][j+1]=input[i][j];
    }
    output[0][i+1]=input[size-1][i];
    output[size+1][i+1]=input[0][i];
    output[i+1][0]=input[i][size-1];
    output[i+1][size+1]=input[i][0];
  }
  output[0][0]=input[size-1][size-1];
  output[size+1][0]=input[0][size-1];
  output[0][size+1]=input[size-1][0];
  output[size+1][size+1]=input[0][0];
  return output;
}

array<array<int,size>,size> next_frame(array<array<int,size+2>,size+2> input){
  int a=0, b=0,c=0,sum=0;
  array<array<int,size>,size> output={{}};
  for(int i=0;i<size;++i){
    b=input[i][0]+input[i+1][0]+input[i+2][0];
    c=input[i][1]+input[i+1][1]+input[i+2][1];

    for(int j=0;j<size;++j){
      a=b;
      b=c;
      c=input[i][j+2]+input[i+1][j+2]+input[i+2][j+2];
      sum=a+b+c;
      if(sum==3){
        output[i][j]=1;
      }
      else if(sum==4 && input[i+1][j+1]==1){
        output[i][j]=1;
      }
      
    }
  

  }
  return output;

}

int main(int argc, char * argv[]){
  
  array<array<int,size>,size> frame= {{}};
  array<array<int,size>,size> frame_prev= {{}};
  array<array<int,size>,size> frame_prev_2= {{}};
  array<array<int,size+2>,size+2> frame_bc= {{}};
  int frame_num=3000;
  srand(time(0));

  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "regular";
  defaults.rows = size;
  defaults.cols=size;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = true;
   Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults);

  frame_bc=wrapper(initialise(frame));
  for(int n=0;n<frame_num;++n){
    frame=next_frame(frame_bc);
    for (int i=0;i<size;++i){
      for(int j=0;j<size;++j){
        if(frame[i][j]==1){
          canvas-> SetPixel(i,j,255,0,0);
        }
        
      }

    }
    frame_bc=wrapper(frame);
    if(frame==frame_prev_2){
      return 0;
    }
    usleep(110*1000);
    canvas->Clear();
    frame_prev_2=frame_prev;
    frame_prev=frame;
    
  }


  return 0;


}