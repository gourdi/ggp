#ifndef __GGO_PATH_ABC__
#define __GGO_PATH_ABC__

#include <ggo_vec.h>

namespace ggo
{
  class path_abc
  {
  public:

    virtual 			     ~path_abc() {}

    virtual	ggo::pos2f  get_pos(int counter) = 0;
  };
}

#endif