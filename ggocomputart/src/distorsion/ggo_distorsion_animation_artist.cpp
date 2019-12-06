#include "ggo_distorsion_animation_artist.h"
#include <kernel/ggo_kernel.h>
#include <2d/fill/ggo_fill.h>

//////////////////////////////////////////////////////////////
ggo::distorsion_animation_artist::distorsion_animation_artist(
  int width, int height, int line_byte_step,
  ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
  ggo::ratio fps)
  :
  progress_animation_artist(width, height, line_byte_step, pixel_type, memory_lines_order, 12, fps)
{
  _artist.for_each_stripe([&](const ggo::distorsion_artist::stripe& stripe)
    {
      _stripes.emplace_back(stripe._x_sup * width, ggo::convert_color_to<ggo::rgb_8u>(stripe._color));
    });
}

//////////////////////////////////////////////////////////////
void ggo::distorsion_animation_artist::render_frame(void * buffer, float progress)
{
  progress = ggo::ease_inout(progress);

  std::vector<transform> transforms;
  _artist.for_each_transform(progress, [&](ggo::pos2_f center, float disp, float variance)
    {
      transforms.emplace_back(ggo::pos2_f(center.x() * width(), center.y() * height()), disp * min_size(), variance * ggo::square(min_size()));
    });


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
void ggo::distorsion_animation_artist::render_t(void * buffer, const std::vector<ggo::distorsion_animation_artist::transform> & transforms) const
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

      const auto stripe1 = get_stripe_at(apply_transforms(x1, y1, transforms));
      const auto stripe2 = get_stripe_at(apply_transforms(x1, y4, transforms));
      const auto stripe3 = get_stripe_at(apply_transforms(x4, y1, transforms));
      const auto stripe4 = get_stripe_at(apply_transforms(x4, y4, transforms));

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
        acc.add(get_stripe_at(apply_transforms(x1, y2, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x1, y3, transforms))->_color);
        acc.add(stripe2->_color);

        acc.add(get_stripe_at(apply_transforms(x2, y1, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x2, y2, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x2, y3, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x2, y4, transforms))->_color);

        acc.add(get_stripe_at(apply_transforms(x3, y1, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x3, y2, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x3, y3, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x3, y4, transforms))->_color);

        acc.add(stripe3->_color);
        acc.add(get_stripe_at(apply_transforms(x4, y2, transforms))->_color);
        acc.add(get_stripe_at(apply_transforms(x4, y3, transforms))->_color);
        acc.add(stripe4->_color);

        img.write_pixel(x, y, acc.div<16>());
      }
    }
  }
}

//////////////////////////////////////////////////////////////
float ggo::distorsion_animation_artist::apply_transforms(float x, float y, const std::vector<transform> & transforms)
{
  float result = x;
  
  for (const auto & transform : transforms)
  {
    float dx = x - transform._center.x();
    float dy = y - transform._center.y();
    
    float influence = std::exp(-(dx * dx + dy * dy) / transform._variance);
      
    result += influence * transform._disp;
  }
    
  return result;
}

//////////////////////////////////////////////////////////////
std::vector<ggo::distorsion_animation_artist::stripe>::const_iterator ggo::distorsion_animation_artist::get_stripe_at(float x) const
{
  x = ggo::pos_mod(x, static_cast<float>(width()));

  auto it = _stripes.begin();
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

