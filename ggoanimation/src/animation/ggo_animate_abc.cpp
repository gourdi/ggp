#include "ggo_animate_abc.h"
#include <kernel/ggo_kernel.h>

//////////////////////////////////////////////////////////////
bool ggo::animate_abc::update()
{
  _frame_index++;

  if (_start_offset > 0)
  {
    --_start_offset;
    return true;
  }
  else
  {
    return update(_frame_index);
  }
}

//////////////////////////////////////////////////////////////
void ggo::animate_abc::render(ggo::image & img, const ggo::rect_int & clipping) const
{
  render(img, clipping, _frame_index);
}

//////////////////////////////////////////////////////////////
bool ggo::position_animate_abc::update(int frame_index)
{
  return update(frame_index, _pos);
}

//////////////////////////////////////////////////////////////
void ggo::position_animate_abc::render(ggo::image & img, const ggo::rect_int & clipping, int frame_index) const
{
  render(img, clipping, frame_index, _pos);
}

//////////////////////////////////////////////////////////////
bool ggo::path_animate_abc::update(int frame_index)
{
  GGO_ASSERT(_path != nullptr);

  _cur_pos = _start_pos + _path->get_pos(frame_index);

  return update(frame_index, _cur_pos);
}

//////////////////////////////////////////////////////////////
void ggo::path_animate_abc::render(ggo::image & img, const ggo::rect_int & clipping, int frame_index) const
{
  render(img, clipping, frame_index, _cur_pos);
}
