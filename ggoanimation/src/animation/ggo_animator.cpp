#include "ggo_animator.h"
#include "ggo_animate_abc.h"

//////////////////////////////////////////////////////////////
void ggo::animator::clear()
{
  _animates.clear();
}

//////////////////////////////////////////////////////////////
void ggo::animator::add_animate(ggo::animate_abc * animate)
{
  _animates.push_back(std::unique_ptr<ggo::animate_abc>(animate));
}

//////////////////////////////////////////////////////////////
void ggo::animator::insert_animate(ggo::animate_abc * animate, int pos)
{
  _animates.insert(_animates.begin() + pos, std::unique_ptr<ggo::animate_abc>(animate));
}

//////////////////////////////////////////////////////////////
void ggo::animator::update()
{
  ggo::remove_if(_animates, [&](std::unique_ptr<ggo::animate_abc> & animate)
  {
    return animate->update() == false;
  });
}

//////////////////////////////////////////////////////////////
void ggo::animator::render(void * buffer, int width, int height, int line_step, ggo::image_format format) const
{
  for (const auto & animate : _animates)
  {
    animate->render(buffer, width, height, line_step, format);
  }
}

