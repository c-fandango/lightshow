#include "../include/functions.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <map>
using namespace std;

void Frame::create(vector<Particle> input) {
    for(int i=0; i<input.size(); ++i) {
        if ( input[i].pos[0] < 0 || input[i].pos[1] < 0 || input[i].pos[0] > size - 1 || input[i].pos[1] > size - 1) {
            continue;
        }
        frame[input[i].pos[1]][input[i].pos[0]][0] = input[i].col[0];
        frame[input[i].pos[1]][input[i].pos[0]][1] = input[i].col[1];
        frame[input[i].pos[1]][input[i].pos[0]][2] = input[i].col[2];
    }
}

void Frame::clear() {
    frame = blank_frame; 
}

void Frame::wild(bool input) {
    if (!input) {
	return;
    }
   for(int i=0; i<frame.size(); ++i) {
       for(int j=0; j<frame.size(); ++j) {
           if(frame[i][j] != black) {
               frame[i][j] = {rand()%256,rand()%256,rand()%256};
           }
       }
   }
}

vector<vector<int>> Conway::wrap_frame(vector<vector<int>> input) {
    vector<vector<int>> output = vector<vector<int>>(size+2, vector<int>(size+2, 0));
    for(int i=0; i<size; ++i) {
        for(int j=0; j<size; ++j) {
            output[i+1][j+1] = input[i][j];
        }
        output[0][i+1] = input[size-1][i];
        output[size+1][i+1] = input[0][i];
        output[i+1][0] = input[i][size-1];
        output[i+1][size+1] = input[i][0];
    }
    output[0][0] = input[size-1][size-1];
    output[size+1][0] = input[0][size-1];
    output[0][size+1] = input[size-1][0];
    output[size+1][size+1] = input[0][0];
    return output;
}

void Conway::initialise() {
    for(int i=0; i<size; ++i) {
        for (int j=0; j<size; ++j) {
            frame.sframe[i][j] = rand()%2;
        }
    }
    old_frames = {frame.blank_sframe, frame.blank_sframe};
    global_col = {rand()%256, rand()%256, rand()%256};
    wild = (rand()%prob == 0);
}

void Conway::evolve() {
    old_frames[1] = old_frames[0];
    old_frames[0] = frame.sframe;
    vector<vector<int>> frame_bc = wrap_frame(frame.sframe);
    int a = 0, b = 0, c = 0, sum = 0;
    for(int i=0; i<size; ++i) {
        b = frame_bc[i][0] + frame_bc[i+1][0] + frame_bc[i+2][0];
        c = frame_bc[i][1] + frame_bc[i+1][1] + frame_bc[i+2][1];
        for(int j=0; j<size; ++j) {
            a = b;
            b = c;
            c = frame_bc[i][j+2] + frame_bc[i+1][j+2] + frame_bc[i+2][j+2];
            sum = a + b + c;
            if (sum == 3 || (sum == 4 && frame_bc[i+1][j+1])) {
                frame.sframe[i][j] = 1;
            }
            else {
                frame.sframe[i][j] = 0;
            }
        }
    }
    stable = (frame.sframe == old_frames[1]);
}

void Ant::initialise() {
    for (int i=size/2 - start_num/2; i<size/2 + start_num/2 + 1; ++i) {
        for (int j=size/2 - start_num/2; j<size/2 + start_num/2 + 1; ++j) {
            frame.sframe[j][i] = rand()%2;
        }
    }
    global_col = {rand()%256, rand()%256, rand()%256};
    particle.pos = {size/2, size/2};
}

void Ant::evolve() {
    particle.pos[0] = (particle.pos[0] + size)%size;  //because mod in cpp is weird
    particle.pos[1] = (particle.pos[1] + size)%size;
    particle.orientation += 1 + 2*(frame.sframe[particle.pos[1]][particle.pos[0]]);
    particle.orientation %= 4;
    ++frame.sframe[particle.pos[1]][particle.pos[0]];
    frame.sframe[particle.pos[1]][particle.pos[0]] %= 2;
    switch(particle.orientation) {
    case 0:
        ++particle.pos[1];
        break;
    case 1:
        ++particle.pos[0];
        break;
    case 2:
        --particle.pos[1];
        break;
    case 3:
        --particle.pos[0];
        break;
    }
}

void Sand::next_grain() {
    vector<int> pos = {size/2, size/2};

    if (wild) {
        pos = {rand()%3 + size/2 - 1, rand()%3 + size/2 - 1};
    }

    ++frame.sframe[pos[0]][pos[1]];
    if (frame.sframe[pos[0]][pos[1]] > 3) {
        stable = false;
    }
}

void Sand::evolve() {
    vector<vector<int>> new_pile = frame.sframe;
    stable = true;
    for (int i=1; i<size-1; ++i) {
        for (int j=1; j<size-1; ++j) {
            if (frame.sframe[i][j] > 3) {
                stable = false;
                ++new_pile[i+1][j];
                ++new_pile[i][j+1];
                ++new_pile[i-1][j];
                ++new_pile[i][j-1];
                new_pile[i][j] -= 4;
            }
        }
    }
    frame.sframe = new_pile;
}

void Bounce::initialise() {
    vector<Particle> balls;
    for (int i=0; i<num_balls; ++i) {
	Particle ball;
        ball.pos= {rand()%size, rand()%size};
        ball.vel = {rand()%5 - 2, rand()%5 - 2};
        ball.col = {rand()%256, rand()%256, rand()%256};
        balls.push_back(ball);
    }
    particles = balls;
}

void Bounce::evolve() {
    Particle ball;
    map <vector<int>, int> col_counts;
    int max_col_count = 0;
    // this is ugly but it's fast
    for(int i=0; i<particles.size(); ++i) {
	ball = particles[i];
	
        ball.pos[0] += ball.vel[0];
        ball.pos[1] += ball.vel[1];
        if ( ball.pos[0] < 0 || ball.pos[0] > size - 1 ) {
            ball.pos[0] -= 2*(ball.pos[0]%(size-1));
            ball.vel[0] *= -1;
        }
        if ( ball.pos[1] < 1 || ball.pos[1] > size - 1 ) {
            ball.pos[1] -= 2*(ball.pos[1]%(size-1));
            ball.vel[1] *= -1;
        }
	int j = i-1;
	// insertion sort list for time save - create gap
	while (j>=0 && (particles[j].pos[0] > ball.pos[0] || (particles[j].pos[0] == ball.pos[0] && particles[j].pos[1] > ball.pos[1]))) {
	    particles[j+1] = particles[j];
	    --j;
	}
	// collision
	if (j>=0 && particles[j].pos == ball.pos) {

           swap(ball.vel, particles[j].vel);
           if(rand()%2) {
               ball.col = particles[j].col;
           }
           else {
               particles[j].col = ball.col;
           }
       }
       //insert ball into gap
       particles[j+1] = ball;

       // update mode
       ++col_counts[ball.col];
       max_col_count = max(max_col_count, col_counts[ball.col]);
    }

    //calculate mode
    for ( auto k = col_counts.begin(); k != col_counts.end(); ++k) {
        if (k -> second == max_col_count) {
	    mode_col = k -> first;
	    return;
	}
    }
}

void Scatter::initialise(int num_streams) {
    Particle ball_one;
    Particle ball_two;
    vector<Particle> ball_opts;
    static_balls_frame = frame.blank_frame;
    balls = {};
    int num_balls = 100 + 50*num_streams;

    for (int i=1; i<num_rows; ++i) {
        for (int j=1; j<i+1; ++j) {
	    pin_frame[i*2][size/2 + 2*(2*j - i - 1)] = 1;
        }
    }

    if(num_streams - 1){
	ball_one.col = ball_cols[1];
	ball_one.pos[1] = 3*size/8;
	ball_two.col = ball_cols[2];
	ball_two.pos[1] = 5*size/8;
    }
    else {
	ball_one.col = ball_cols[0];
	ball_one.pos[1] = size/2;
	ball_two = ball_one;
    }

    ball_opts = {ball_one, ball_two};

    for (int i=0; i<num_balls; ++i) {
	ball_opts[i%2].pos[0] = -2*i - 1;
	balls.push_back(ball_opts[i%2]);
    }
}

void Scatter::evolve() {
    int xpos, ypos;

    for(int i=0; i<balls.size(); ++i) {
        ++balls[i].pos[0];
	xpos = balls[i].pos[1];
	ypos = balls[i].pos[0];
	if (ypos < 0) {
            continue;
	}
	if (ypos == size || static_balls_frame[xpos][ypos] != frame.black) {
	    static_balls_frame[xpos][ypos-1] = balls[i].col;
	    balls.erase(balls.begin() + i);
	    --i;
	    continue;
	}
	if (pin_frame[ypos][xpos]){
            balls[i].vel[1] = -1 + (rand()%2)*2;
	}

        balls[i].pos[1] += balls[i].vel[1];
        if (balls[i].pos[0] == num_rows*2-1) {
            balls[i].vel[1] = 0;
        }
    }
}

Particle Star::create_star(vector<Particle> input, int col) {
    vector<int> pos;
    Particle output;
    output.col = {col, col, col};
    output.vel[0] = -1 + 2*rand()%2;
    if (input.empty()){
        output.pos = {rand()%size,rand()%size};
	return output;
    }
    while(true) {
        pos = {rand()%size,rand()%size};
        for (int j=0; j<input.size(); ++j) {
            if (input[j].pos[0] == pos[0] && input[j].pos[1] == pos[1]) {
                break;
            }
            else if (j == input.size() - 1) {
                output.pos = pos;
		return output; 
            }
        }
    }
}

void Star::initialise() {
    stars = {};
    int col;
    for (int i=0; i<num_stars; ++i) {
        int col = 1 + rand()%(max_brightness-1);
	stars.push_back(create_star(stars, col)); 
    }
}


void Star::evolve() {
    int col;
    for (int i=0; i<num_stars; ++i) {
        if (stars[i].col == frame.black && rand()%decay_prob == 0) {
            col = 1 + rand()%30;
	    stars[i] = create_star(stars, col);
        }
        if (stars[i].col[0]%max_brightness == 0 || rand()%twinkle_prob == 0) {
            stars[i].vel[0] *= -1;
        }
	col = stars[i].col[0] + stars[i].vel[0];
        stars[i].col = {col, col, col};
    }
}
