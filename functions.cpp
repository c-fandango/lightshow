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

vector<vector<int>> ant_class::initialise(int num){
  vector<vector<int>> output(size,vector<int>(size,0));
  for (int i=size/2 -num/2; i<size/2 +num/2 +1; ++i){
    for (int j=size/2 -num/2; j<size/2 +num/2 +1;++j){
        output[j][i] = rand()%2;
    }
  }
  return output;
}

ant_class ant_class::next_frame(ant_class input){
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
  if (input.xpos==-1 || input.xpos==64 || input.ypos==-1 || input.ypos==64 ){
    input.col_r=rand()%256;
    input.col_g=rand()%256;
    input.col_b=rand()%256;
  }
  input.xpos= (input.xpos +size) %size;
  input.ypos= (input.ypos +size) %size;
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
  
  return input;  
}

sand_class sand_class::next_grain(sand_class input){
    int i, j;
    if (input.wild){
        i = rand()%3 + size/2 -1;
        j = rand()%3 + size/2 -1;
    }
    else{
        i = size/2;
        j = size/2;
    }
    
    ++input.frame[i][j];
    if (input.frame[i][j] >3){
        input.stable=false;
    }
   
  return input;
}

sand_class sand_class::evolve(sand_class input){
  sand_class output= input;
  output.stable=true;
  for (int i=1; i<size-1;++i){
    for (int j=1; j<size-1;++j){
      if (input.frame[i][j]>3){
        output.stable=false;
        ++output.frame[i+1][j];
        ++output.frame[i][j+1];
        ++output.frame[i-1][j];
        ++output.frame[i][j-1];
        output.frame[i][j]=0;
        
      }
    }
  }
  
  return output;
}

vector<ball_class> bounce_class::initialise(int num){
  ball_class start;
  vector<ball_class> output(num,start);
  
  for (int i=0; i<num; ++i){
    output[i].pos={rand()%size,rand()%size};
    output[i].vel = {rand()%5-2,rand()%5-2};
    output[i].col = {rand()%256,rand()%256,rand()%256};
  }
  return output;
}

vector<ball_class> bounce_class::next_frame(vector<ball_class> input){
  for(int i=0; i<input.size();++i){
    input[i].pos[0]+=input[i].vel[0];
    input[i].pos[1]+=input[i].vel[1];
    if(input[i].pos[0]>63 ){
      input[i].pos[0] = 63-input[i].pos[0]%size;
      input[i].vel[0] *= -1;
    }
    else if(input[i].pos[0]<0){
      input[i].pos[0] = 0-input[i].pos[0]%size;
      input[i].vel[0] *= -1;
    }
    if(input[i].pos[1]>63 ){
      input[i].pos[1] = 63-input[i].pos[1]%size;
      input[i].vel[1] *= -1;
    }
    else if(input[i].pos[1]<0){
      input[i].pos[1] = 0-input[i].pos[1]%size;
      input[i].vel[1] *= -1;
    }
  }
  for(int i=0; i<input.size()-1;++i){
    for(int j=i+1;j<input.size();++j){
      if(input[i].pos==input[j].pos){
        swap(input[i].vel, input[j].vel);
        if(rand()%2==0){
          input[i].col=input[j].col;
        }
        else{
          input[j].col=input[i].col;
        }
      }
    }
  }
return input;
}

vector<int> bounce_class::mode_func(vector<ball_class> input){
  vector<int> no_value(3,-1);
  vector<vector<int>> elements(input.size(),no_value);
  vector<int> count(input.size(),0);
  int max=0;
  for(int i=0; i<input.size(); ++i){
    for(int j=0; j<elements.size(); ++j){
      if(input[i].col==elements[j]){
        ++count[j];
        if(count[j]>count[max]){
          max= j;
        }
        break;
      }
      else if(elements[j] == no_value){
        elements[j]=input[i].col;
        ++count[j];
        break;
      }
    }
  }
  return elements[max];
}

scatter_class scatter_class::initialise(scatter_class start){
  ball_class initial;
  
  for (int i=1;i<start.num_rows;++i){
    for (int j=1; j<i+1;++j){
      if (true){
        start.pins[i*(i-1)/2 +j-1][0]=i*2;
        start.pins[i*(i-1)/2 +j-1][1]=size/2 + 2*(2*j -i -1);
      }
    }
  }
  if(start.num_streams==1){
    start.num_balls=150;
    start.balls=vector<ball_class>(start.num_balls,initial);
    for (int i=0; i<start.num_balls; ++i){
      start.balls[i].pos[1]=size/2;
      start.balls[i].pos[0]=-2*i -1;
      start.balls[i].col=start.ball_col1;
    }
  }
  else if (start.num_streams==2){
    start.num_balls=200;
    start.balls=vector<ball_class>(start.num_balls,initial);
    for (int i=0; i<start.num_balls; ++i){
      if (i%2==0){
        start.balls[i].pos[1]=3*size/8;
        start.balls[i].col=start.ball_col2_1;
      }
      else{
        start.balls[i].pos[1]=5*size/8;
        start.balls[i].col=start.ball_col2_2;
      }
      start.balls[i].pos[0]=-2*i -1;
    }
  }
return start;
}

scatter_class scatter_class::evolve(scatter_class input){
  int end=input.balls.size();

  for(int i=0; i<end;++i){
    for (int j=0; j<end; ++j){
      if((input.balls[i].pos[1] == input.balls[j].pos[1] && input.balls[i].pos[0]+1 == input.balls[j].pos[0]) || input.balls[i].pos[0]==size-1 ){
        --input.balls[i].pos[0];
        break;
      }
    }
    ++input.balls[i].pos[0];
    for(int j=0; j<input.pins.size();++j){
      if(input.balls[i].pos[0]==input.pins[j][0] && input.balls[i].pos[1]==input.pins[j][1]){
        input.balls[i].vel[0] = ((rand()%2)*2 -1);
        break;
      }
    }
    
    input.balls[i].pos[1] += input.balls[i].vel[0];
    if (input.balls[i].pos[0]== input.num_rows*2-1){
      input.balls[i].vel[0]=0;
    }
  }
  return input;
}
