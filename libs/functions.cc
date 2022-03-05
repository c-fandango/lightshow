#include "../include/functions.h"
#include <cstdlib>
#include<unistd.h>
using namespace std;

void virtual_frame_class::create(vector<particle_class> input) {
    for(int i=0; i<input.size(); ++i) {
        if ( input[i].pos[0]<0 || input[i].pos[1]< 0 || input[i].pos[0]>input.size()-1 || input[i].pos[1]>input.size()-1) {
            continue;
        }
        frame[input[i].pos[1]][input[i].pos[0]][0] = input[i].col[0];
        frame[input[i].pos[1]][input[i].pos[0]][1] = input[i].col[1];
        frame[input[i].pos[1]][input[i].pos[0]][2] = input[i].col[2];
    }
}

void virtual_frame_class::clear() {
    frame = blank_frame;
}

void virtual_frame_class::wild(bool input) {
    vector<int> zero = {0,0,0};
    if (input) {
        for(int i=0; i<frame.size(); ++i) {
            for(int j=0; j<frame.size(); ++j) {
                if(frame[i][j] != zero) {
                    frame[i][j] = {rand()%256,rand()%256,rand()%256};
                }
            }
        }
    }
}


void conway_class::wrapper() {
    for(int i=0; i<size; ++i) {
        for(int j=0; j<size; ++j) {
            frame_bc[i+1][j+1]=frame_prev[i][j];
        }
        frame_bc[0][i+1]=frame_prev[size-1][i];
        frame_bc[size+1][i+1]=frame_prev[0][i];
        frame_bc[i+1][0]=frame_prev[i][size-1];
        frame_bc[i+1][size+1]=frame_prev[i][0];
    }
    frame_bc[0][0]=frame_prev[size-1][size-1];
    frame_bc[size+1][0]=frame_prev[0][size-1];
    frame_bc[0][size+1]=frame_prev[size-1][0];
    frame_bc[size+1][size+1]=frame_prev[0][0];
}

void conway_class::initialise() {
    frame = vector<vector<int>>(size,vector<int>(size,0));
    frame_bc = vector<vector<int>>(size+2,vector<int>(size+2,0));
    frame_prev = frame;
    frame_prev_2 = frame;
    for(int i=0; i<size; ++i) {
        for (int j=0; j<size; ++j) {
            frame[i][j]= rand()%2;
        }
    }
    if(rand()%wild_prob==0) {
        wild = true;
    }
}


void conway_class::next_frame() {
    frame_prev_2=frame_prev;
    frame_prev = frame;
    wrapper();
    int a=0, b=0,c=0,sum=0;
    for(int i=0; i<size; ++i) {
        b=frame_bc[i][0]+frame_bc[i+1][0]+frame_bc[i+2][0];
        c=frame_bc[i][1]+frame_bc[i+1][1]+frame_bc[i+2][1];
        for(int j=0; j<size; ++j) {
            a=b;
            b=c;
            c=frame_bc[i][j+2]+frame_bc[i+1][j+2]+frame_bc[i+2][j+2];
            sum=a+b+c;
            if(sum==3) {
                frame[i][j]=1;
            }
            else if(sum==4 && frame_bc[i+1][j+1]==1) {
                frame[i][j]=1;
            }
            else {
                frame[i][j] =0;
            }
        }
    }
    if (frame == frame_prev_2) {
        stable = true;
    }
}

void ant_class::initialise() {
    frame = vector<vector<int>>(size,vector<int>(size,0));
    for (int i=size/2 -start_num/2; i<size/2 + start_num/2 +1; ++i) {
        for (int j=size/2 -start_num/2; j<size/2 +start_num/2 +1; ++j) {
            frame[j][i] = rand()%2;
        }
    }
}

void ant_class::next_frame() {
    switch(orientation) {
    case 0:
        ++ypos;
        break;
    case 1:
        ++xpos;
        break;
    case 2:
        --ypos;
        break;
    case 3:
        --xpos;
        break;
    }
    if (xpos==-1 || xpos==64 || ypos==-1 || ypos==64 ) {
        trail_col = {rand()%256,rand()%256,rand()%256};
    }
    xpos= (xpos +size) %size;
    ypos= (ypos +size) %size;
    switch(frame[ypos][xpos]) {
    case 1:
        orientation = (orientation+3)%4;

        break;
    case 0:
        orientation = (orientation+5)%4;
        break;
    }
    ++frame[ypos][xpos];
    frame[ypos][xpos] %= 2 ;
}

void sand_class::next_grain() {
    if (wild) {
        xpos = rand()%3 + size/2 -1;
        ypos = rand()%3 + size/2 -1;
    }
    else {
        xpos = size/2;
        ypos = size/2;
    }

    ++pile[xpos][ypos];
    if (pile[xpos][ypos] >3) {
        stable=false;
    }
}

void sand_class::evolve() {
    new_pile= pile;
    stable=true;
    for (int i=1; i<size-1; ++i) {
        for (int j=1; j<size-1; ++j) {
            if (pile[i][j]>3) {
                stable=false;
                ++new_pile[i+1][j];
                ++new_pile[i][j+1];
                ++new_pile[i-1][j];
                ++new_pile[i][j-1];
                new_pile[i][j]-=4;
            }
        }
    }

    pile = new_pile;
}

void bounce_class::initialise() {
    particle_class ball;
    balls =  vector<particle_class>(num_balls,ball);
    for (int i=0; i<num_balls; ++i) {
        balls[i].pos= {rand()%size,rand()%size};
        balls[i].vel = {rand()%5-2,rand()%5-2};
        balls[i].col = {rand()%256,rand()%256,rand()%256};
    }
}

void bounce_class::evolve() {
    for(int i=0; i<num_balls; ++i) {
        balls[i].pos[0]+=balls[i].vel[0];
        balls[i].pos[1]+=balls[i].vel[1];
        if(balls[i].pos[0]>63 ) {
            balls[i].pos[0] = 63-balls[i].pos[0]%size;
            balls[i].vel[0] *= -1;
        }
        else if(balls[i].pos[0]<0) {
            balls[i].pos[0] = 0-balls[i].pos[0]%size;
            balls[i].vel[0] *= -1;
        }
        if(balls[i].pos[1]>63 ) {
            balls[i].pos[1] = 63-balls[i].pos[1]%size;
            balls[i].vel[1] *= -1;
        }
        else if(balls[i].pos[1]<0) {
            balls[i].pos[1] = 0-balls[i].pos[1]%size;
            balls[i].vel[1] *= -1;
        }
    }
    for(int i=0; i<num_balls-1; ++i) {
        for(int j=i+1; j<num_balls; ++j) {
            if(balls[i].pos==balls[j].pos) {
                swap(balls[i].vel, balls[j].vel);
                if(rand()%2==0) {
                    balls[i].col=balls[j].col;
                }
                else {
                    balls[j].col=balls[i].col;
                }
            }
        }
    }
}

void bounce_class::mode_func() {
    elements = vector<vector<int>> (num_balls,vector<int>(3,-1));
    count = vector<int>(num_balls,0);
    max=0;
    for(int i=0; i<num_balls; ++i) {
        for(int j=0; j<num_balls; ++j) {
            if(balls[i].col==elements[j]) {
                ++count[j];
                if(count[j]>count[max]) {
                    max= j;
                }
                break;
            }
            else if(elements[j] == vector<int>(3,-1)) {
                elements[j]=balls[i].col;
                ++count[j];
                break;
            }
        }
    }
    mode = elements[max];
}

void scatter_class::initialise() {
    pin.col = pin_col;
    pins = vector<particle_class>(num_rows*(num_rows+1)/2,pin);

    for (int i=1; i<num_rows; ++i) {
        for (int j=1; j<i+1; ++j) {
            if (true) {
                pins[i*(i-1)/2 +j-1].pos[0]=i*2;
                pins[i*(i-1)/2 +j-1].pos[1]=size/2 + 2*(2*j -i -1);
            }
        }
    }
    if(num_streams==1) {
        num_balls=150;
        balls=vector<particle_class>(num_balls,ball);
        for (int i=0; i<num_balls; ++i) {
            balls[i].pos[1]=size/2;
            balls[i].pos[0]=-2*i -1;
            balls[i].col=ball_col1;
        }
    }
    else if (num_streams==2) {
        num_balls=200;
        balls=vector<particle_class>(num_balls,ball);
        for (int i=0; i<num_balls; ++i) {
            if (i%2==0) {
                balls[i].pos[1]=3*size/8;
                balls[i].col=ball_col2_1;
            }
            else {
                balls[i].pos[1]=5*size/8;
                balls[i].col=ball_col2_2;
            }
            balls[i].pos[0]=-2*i -1;
        }
    }
}

void scatter_class::evolve() {
    int end=balls.size();

    for(int i=0; i<end; ++i) {
        for (int j=0; j<end; ++j) {
            if((balls[i].pos[1] == balls[j].pos[1] && balls[i].pos[0]+1 == balls[j].pos[0]) || balls[i].pos[0]==size-1 ) {
                --balls[i].pos[0];
                break;
            }
        }
        ++balls[i].pos[0];
        for(int j=0; j<pins.size(); ++j) {
            if(balls[i].pos[0]==pins[j].pos[0] && balls[i].pos[1]==pins[j].pos[1]) {
                balls[i].vel[0] = ((rand()%2)*2 -1);
                break;
            }
        }

        balls[i].pos[1] += balls[i].vel[0];
        if (balls[i].pos[0]== num_rows*2-1) {
            balls[i].vel[0]=0;
        }
    }
}


void rain_class::initialise() {
    drop.col = rain_col;
    rain = vector<particle_class>(max_drops+10,drop);
}

void rain_class::evolve() {
    for (int i =0; i<max_drops; ++i) {
        if (rain[i].pos[0]==-1 && rand()%spawn_prob==0) {
            start_pos=rand()%(size-2)+1;
            for(int j=0; j<max_drops; ++j) {
                if (rain[j].pos[0]>-1 &&  rain[j].pos[0]<4 && abs(rain[j].pos[1] -start_pos)<2) {
                    break;
                }
                else if (j==max_drops-1) {
                    rain[i].pos= {0,start_pos};
                    rain[i].action = false;
                }
            }
        }
    }
    for (int i=0; i<max_drops; ++i) {
        if (rain[i].pos[0] > -1 && !(rain[i].action) && rain[i].pos[0]!=water_level+water_surface) {
            ++rain[i].pos[0];
        }
        if (rain[i].action) {
            rain[i].pos= {-1,0};
            rain[i].action=false;
        }
        else if (rain[i].pos[0]==-1) {
            continue;
        }
        else if (rain[i].pos[0]<water_level) {
            continue;
        }
        else if (rain[i].pos[0]==size-2 || rain[i].pos[0]==water_level+water_surface) {
            rain[i].action =true;
            ++counter;
        }
        else if(rain[i].pos[0]<water_level+water_surface && rand()%(water_surface-3)==0) {
            rain[i].action=true;
            ++counter;
        }
    }
    if (counter>raise_level) {
        --water_level;
        counter=0;
    }
}
void star_class::initialise() {
    stars= vector<particle_class>(num,initial_star);
    int i=0;
    while (i<num-1) {
        pos = {rand()%size,rand()%size};
        for (int j=i+1; j<num; ++j) {
            if (abs(stars[j].pos[0] - pos[0]) < 2 && abs(stars[j].pos[1] - pos[1]) <2) {
                break;
            }
            else if (j==num-1) {
                stars[i].pos=pos;
                stars[i].vel[0] = -1 + 2*rand()%2;
                col = rand()%256;
                stars[i].col[0] = col;
                stars[i].col[1] = col;
                stars[i].col[2] = col;
                ++i;
            }
        }
    }
}

void star_class::evolve() {
    for (int i=0; i<num; ++i) {
        if (stars[i].col[0] == 0 && rand()%decay_prob == 0) {
            col = rand()%40;
            stars[i].col = {col,col,col};
            for (int j=0; j<num; ++j) {
                pos = {rand()%size,rand()%size};
                if (abs(stars[j].pos[0] - pos[0]) < 2 && abs(stars[j].pos[1] - pos[1]) < 2) {
                    break;
                }
                else if (j==num-1) {
                    stars[i].pos = pos;
                    stars[i].vel[0] = -1 + 2*rand()%2;
                    col = rand()%256;
                    stars[i].col = {col,col,col};
                }
            }
        }
        if (stars[i].col[0] ==0 || stars[i].col[0] == 255) {
            stars[i].vel[0] *= -1;
        }
        stars[i].col[0] += stars[i].vel[0];
        stars[i].col[1] += stars[i].vel[0];
        stars[i].col[2] += stars[i].vel[0];
    }
}
