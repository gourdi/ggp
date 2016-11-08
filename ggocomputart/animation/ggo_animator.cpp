#include "ggo_animator.h"
#include "ggo_animate_abc.h"

//////////////////////////////////////////////////////////////
void ggo_animator::clear()
{
  _animates.clear();
}

//////////////////////////////////////////////////////////////
void ggo_animator::add_animate(ggo_animate_abc * animate)
{
  _animates.push_back(std::unique_ptr<ggo_animate_abc>(animate));
}

//////////////////////////////////////////////////////////////
void ggo_animator::insert_animate(ggo_animate_abc * animate, int pos)
{
  _animates.insert(_animates.begin() + pos, std::unique_ptr<ggo_animate_abc>(animate));
}

//////////////////////////////////////////////////////////////
void ggo_animator::update(void * buffer, int width, int height)
{
  ggo::remove_if(_animates, [&](std::unique_ptr<ggo_animate_abc> & animate)
  {
    return animate->update(buffer, width, height) == false;
  });
}
