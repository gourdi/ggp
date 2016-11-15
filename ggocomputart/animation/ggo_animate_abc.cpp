#include "ggo_animate_abc.h"
#include <ggo_kernel.h>

//////////////////////////////////////////////////////////////
bool ggo::animate_abc::update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf)
{
  if (_start_offset > 0)
  {
    --_start_offset;
    return true;
  }
  else
  {
    return update(buffer, width, height, line_step, pbf, _counter++);
  }
}

//////////////////////////////////////////////////////////////
bool ggo::position_animate_abc::update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter)
{
  return update(buffer, width, height, line_step, pbf, counter, _pos);
}

//////////////////////////////////////////////////////////////
bool ggo::path_animate_abc::update(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int counter)
{
  GGO_ASSERT(_path != nullptr);

  return update(buffer, width, height, line_step, pbf, counter, _pos + _path->get_pos(counter));
}
