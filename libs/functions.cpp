#include "functions.h"
#include <cstdlib>
#include<unistd.h>
using namespace std;

vector<vector<int>> conway_class::initialise(vector<vector<int>> input){
  for(int i=0;i<size; ++i){
    for (int j=0; j<size;++j){
      input[i][j]= rand()%2;
    }
  }
  return input;
}

vector<vector<int>> conway_class::wrapper(vector<vector<int>> input){
  vector<vector<int>> output=vector<vector<int>>(size+2,vector<int>(size+2,0));
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

vector<vector<int>> conway_class::next_frame(vector<vector<int>> input){
  int a=0, b=0,c=0,sum=0;
  vector<vector<int>> output=vector<vector<int>>(size,vector<int>(size,0));
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
        output.frame[i][j]-=4;
      }
    }
  }
  
  return output;
}

vector<particle_class> bounce_class::initialise(int num){
  particle_class start;
  vector<particle_class> output(num,start);
  
  for (int i=0; i<num; ++i){
    output[i].pos={rand()%size,rand()%size};
    output[i].vel = {rand()%5-2,rand()%5-2};
    output[i].col = {rand()%256,rand()%256,rand()%256};
  }
  return output;
}

vector<particle_class> bounce_class::next_frame(vector<particle_class> input){
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

vector<int> bounce_class::mode_func(vector<particle_class> input){
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
  particle_class initial;
  
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
    start.balls=vector<particle_class>(start.num_balls,initial);
    for (int i=0; i<start.num_balls; ++i){
      start.balls[i].pos[1]=size/2;
      start.balls[i].pos[0]=-2*i -1;
      start.balls[i].col=start.ball_col1;
    }
  }
  else if (start.num_streams==2){
    start.num_balls=200;
    start.balls=vector<particle_class>(start.num_balls,initial);
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
vector<particle_class> star_class::initialise(int num, int size){
    particle_class initial_star;
    vector<particle_class> output= vector<particle_class>(num,initial_star);
    int i=0;
    int col;
    vector<int> pos = vector<int>(2,0); 
    while (i<num-1){
        pos[0] =rand()%size;
        pos[1] =rand()%size;
	for (int j=i+1; j<num; ++j){
	    if (abs(output[j].pos[0] - pos[0]) < 2 && abs(output[j].pos[1] - pos[1]) <2){
	        break;
	    }
	    else if (j==num-1){
	        output[i].pos=pos;
		output[i].vel[0] = -1 + 2*rand()%2;
		col = rand()%256;
		output[i].col[0] = col;
		output[i].col[1] = col;
		output[i].col[2] = col;
		++i;
	    }
	}
    }
    return output;
}

vector<particle_class> star_class::evolve(int size, int num, vector<particle_class> output){
    int col;
    int xpos, ypos;   
    for (int i=0; i<num; ++i){
        if (rand()%3000==0){
	    col = rand()%256;
            output[i].col[0] = col;
	    output[i].col[1] = col;	
            output[i].col[2] = col;
	    for (int j=0; j<num; ++j){
		xpos = rand()%size;
		ypos = rand()%size;
	        if (abs(output[j].pos[0] - xpos) < 2 && abs(output[j].pos[1] - ypos) < 2){
	            break;
	        }
	        else if (j==num-1){
	            output[i].pos[0] = xpos;
		    output[i].pos[1] = ypos;
		    output[i].vel[0] = -1 + 2*rand()%2;
		    col = rand()%256;
		    output[i].col[0] = col;
		    output[i].col[1] = col;
		    output[i].col[2] = col;
		}
	    }
        }
	if (output[i].col[0] ==0 || output[i].col[0] == 255){
            output[i].vel[0] *= -1;
	}
	output[i].col[0] += output[i].vel[0];
        output[i].col[1] += output[i].vel[0];
	output[i].col[2] += output[i].vel[0];
    }
    return output;
}
