#include "ggo_distorsion_animation_artist.h"
#include <kernel/ggo_kernel.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::distorsion_animation_artist::distorsion_animation_artist(int width, int height, int line_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, pixel_type, memory_lines_order, 300),
_transforms(32)
{
  _hue = ggo::rand<float>();

  for (auto & transform : _transforms)
  {
    transform._center_start.x() = ggo::rand<float>(-0.25, 1.25) * width;
    transform._center_start.y() = ggo::rand<float>(-0.25, 1.25) * height;
    transform._center_end.x() = ggo::rand<float>(-0.25, 1.25) * width;
    transform._center_end.y() = ggo::rand<float>(-0.25, 1.25) * height;
    transform._variance = 0.05f * ggo::square(min_size());
    transform._disp = 0.5f * min_size() * ggo::pos2_f::from_angle(ggo::rand<float>(0, 2 * ggo::pi<float>()));
  }

  float x = 0;
  while (true)
  {
    ggo::distorsion_animation_artist::colored_stripe colored_stripe;

    x += ggo::rand<float>(0.01f, 0.06f) * min_size();

    colored_stripe._x_sup = x;
    colored_stripe._color = ggo::from_hsv<ggo::rgb_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());

    _stripes.push_back(colored_stripe);

    if (x > width)
    {
      break;
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::distorsion_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
{
  std::vector<fixed_transform> transforms;
  for (auto & transform : _transforms)
  {
    transforms.emplace_back(
      ggo::ease_inout(frame_index, frames_count(), transform._center_start, transform._center_end),
      ggo::ease_inout(frame_index, frames_count(), ggo::vec2_f(0.f, 0.f), transform._disp),
      transform._variance);
  }

  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, transforms);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, transforms);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, transforms);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::distorsion_animation_artist::render_t(void * buffer, const std::vector<ggo::distorsion_animation_artist::fixed_transform> & transforms) const
{
  if (buffer == nullptr)
  {
    return;
  }

  image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  for (int y = 0; y < height(); ++y)
  {
    const float y1 = y - 0.375f;
    const float y2 = y - 0.125f;
    const float y3 = y + 0.125f;
    const float y4 = y + 0.375f;

    for (int x = 0; x < width(); ++x)
    {
      const float x1 = x - 0.375f;
      const float x2 = x - 0.125f;
      const float x3 = x + 0.125f;
      const float x4 = x + 0.375f;

      const auto stripe1 = get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y1, transforms));
      const auto stripe2 = get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y4, transforms));
      const auto stripe3 = get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y1, transforms));
      const auto stripe4 = get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y4, transforms));

      // 4 samples are of the same color, we don't go further.
      if (stripe1 == stripe2 && stripe2 == stripe3 && stripe3 == stripe4)
      {
        img.write_pixel(x, y, stripe1->_color);
      }
      // Full processing.
      else
      {
        ggo::color_accumulator<ggo::rgb_8u> acc;

        acc.add(stripe1->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y2, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y3, transforms))->_color);
        acc.add(stripe2->_color);

        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y1, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y2, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y3, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y4, transforms))->_color);

        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y1, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y2, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y3, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y4, transforms))->_color);

        acc.add(stripe3->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y2, transforms))->_color);
        acc.add(get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y3, transforms))->_color);
        acc.add(stripe4->_color);

        img.write_pixel(x, y, acc.div<16>());
      }
    }
  }
}

//////////////////////////////////////////////////////////////
float ggo::distorsion_animation_artist::transform(float x, float y, const std::vector<fixed_transform> & transforms)
{
  float result = x;
  
  for (const auto & transform : transforms)
  {
    float dx = x - transform._center.x();
    float dy = y - transform._center.y();
    
    float influence = std::exp(-(dx * dx + dy * dy) / transform._variance);
      
    result += influence * transform._disp.x();
  }
    
  return result;
}

//////////////////////////////////////////////////////////////
std::vector<ggo::distorsion_animation_artist::colored_stripe>::const_iterator ggo::distorsion_animation_artist::get_stripe_at(float x) const
{
  x = ggo::pos_mod(x, static_cast<float>(width()));
  
  std::vector<colored_stripe>::const_iterator it = _stripes.begin();
  for (; it != _stripes.end(); ++it)
  {
    if (x <= it->_x_sup)
    {
      break;
    }
  }
  
  GGO_ASSERT(it != _stripes.end());
  
  return it;
}

