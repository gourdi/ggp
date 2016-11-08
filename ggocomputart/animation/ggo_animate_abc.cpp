#include "ggo_animate_abc.h"
#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
bool ggo_animate_abc::update(void * buffer, int width, int height)
{
  if (_start_offset > 0)
  {
    --_start_offset;
    return true;
  }
  else
  {
    return update(buffer, width, height, _counter++);
  }
}

//////////////////////////////////////////////////////////////
bool ggo_position_animate_abc::update(void * buffer, int width, int height, int counter)
{
  return update(buffer, width, height, counter, _pos);
}

//////////////////////////////////////////////////////////////
bool ggo_path_animate_abc::update(void * buffer, int width, int height, int counter)
{
  GGO_ASSERT(_path != nullptr);

  return update(buffer, width, height, counter, _pos + _path->get_pos(counter));
}
