
#include <array>
using namespace std;


const int size=64;
class conway_class{
  public:
    int frame_num;
    int col_r , col_g, col_b;
    array<array<int,size>,size> frame= {{}};
    array<array<int,size>,size> frame_prev= {{}};
    array<array<int,size>,size> frame_prev_2= {{}};
    array<array<int,size+2>,size+2> frame_bc= {{}};
    array<array<int,size>,size> initialise(array<array<int,size>,size> input);

    array<array<int,size+2>,size+2> wrapper(array<array<int,size>,size> input);

    array<array<int,size>,size> next_frame(array<array<int,size+2>,size+2> input);
};