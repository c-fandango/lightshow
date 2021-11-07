#include "functions.h"
#include <array>
using namespace std;



array<array<int,size>,size> conway_class::initialise(array<array<int,size>,size> input){
  for(int i=0;i<size; ++i){
    for (int j=0; j<size;++j){
      input[i][j]= rand()%2;
    }
  }
  return input;
}

array<array<int,size+2>,size+2> conway_class::wrapper(array<array<int,size>,size> input){
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

array<array<int,size>,size> conway_class::next_frame(array<array<int,size+2>,size+2> input){
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
