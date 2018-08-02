#include <vivelle/ggo_threadpool.h>

/////////////////////////////////////////////////////////////////////
ggo::threadpool & ggo::global_threadpool()
{
  //static ggo::threadpool pool(std::max(2u, std::thread::hardware_concurrency()));
  static ggo::threadpool pool(1);

  return pool;
}
