#include "ggo_cabrel_animation_artist.h"
#include <ggo_gaussian_blur.h>

//////////////////////////////////////////////////////////////
ggo_cabrel_animation_artist::ggo_cabrel_animation_artist(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_image_float(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_cabrel_animation_artist::init_sub()
{
  _curve.reset(new ggo_cabrel_curve<6>());
  _image_float.fill(ggo::color::BLACK);
}

//////////////////////////////////////////////////////////////
bool ggo_cabrel_animation_artist::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (frame_index > 300)
  {
    return false;
  }

  float stddev = 0.0008f * get_render_min_size();
  ggo::gaussian_blur_2d_mirror<float, float, 3, 3>(_image_float.data() + 0, _image_float.data() + 0, get_render_width(), get_render_height(), stddev, 0.0001f);
  ggo::gaussian_blur_2d_mirror<float, float, 3, 3>(_image_float.data() + 1, _image_float.data() + 1, get_render_width(), get_render_height(), stddev, 0.0001f);
  ggo::gaussian_blur_2d_mirror<float, float, 3, 3>(_image_float.data() + 2, _image_float.data() + 2, get_render_width(), get_render_height(), stddev, 0.0001f);

  _image_float.for_each_pixel([&](int x, int y)
  {
    ggo::color c = _image_float.get(x, y);
    _image_float.set(x, y, 0.995f * c);
  });

  _curve->update();
  _curve->paint(_image_float);

  auto image = make_image_buffer(buffer);
  image.from(_image_float);

  return true;
}
