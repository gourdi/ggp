#define DEBUG_MEAN_BOX_FILTER 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_mean_box_filter1d.h>
#include <queue>

/////////////////////////////////////////////////////////////////////
GGO_TEST(mean_box_filter1d, toto)
{
  float data[8] = { 1, 7, 3, 2, 5, 0, 1, 0 };
  std::queue<float> fifo;

 // auto read = 
}


