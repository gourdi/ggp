#include "ggo_distorsion_animation_artist.h"
#include <ggo_kernel.h>
#include <ggo_fill.h>

namespace 
{
  const int frames_count = 300;

  //////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf>
  void render_t(void * buffer, const ggo::distorsion_animation_artist & artist, const std::vector<ggo::distorsion_animation_artist::fixed_transform> & transforms)
  {
    for (int y = 0; y < artist.get_height(); ++y)
    {
      const float y1 = y - 0.375f;
      const float y2 = y - 0.125f;
      const float y3 = y + 0.125f;
      const float y4 = y + 0.375f;

      void * ptr = ggo::get_line_ptr<ggo::pixel_buffer_format_info<pbf>::y_dir>(buffer, y, artist.get_height(), artist.get_line_step());

      for (int x = 0; x < artist.get_width(); ++x)
      {
        const float x1 = x - 0.375f;
        const float x2 = x - 0.125f;
        const float x3 = x + 0.125f;
        const float x4 = x + 0.375f;

        const auto stripe1 = artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y1, transforms));
        const auto stripe2 = artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y4, transforms));
        const auto stripe3 = artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y1, transforms));
        const auto stripe4 = artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y4, transforms));

        // 4 samples are of the same color, we don't go further.
        if (stripe1 == stripe2 && stripe2 == stripe3 && stripe3 == stripe4)
        {
          ggo::write_pixel<pbf>(buffer, stripe1->_color);
        }
        // Full processing.
        else
        {
          ggo::accumulator<ggo::color_8u> acc;

          acc.add(stripe1->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y2, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x1, y3, transforms))->_color);
          acc.add(stripe2->_color);

          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y1, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y2, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y3, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x2, y4, transforms))->_color);

          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y1, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y2, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y3, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x3, y4, transforms))->_color);

          acc.add(stripe3->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y2, transforms))->_color);
          acc.add(artist.get_stripe_at(ggo::distorsion_animation_artist::transform(x4, y3, transforms))->_color);
          acc.add(stripe4->_color);

          ggo::write_pixel<pbf>(buffer, acc.div<16>());
        }

        buffer = ggo::ptr_offset(buffer, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size);
      }
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::distorsion_animation_artist::distorsion_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt),
_transforms(32)
{
}

//////////////////////////////////////////////////////////////
void ggo::distorsion_animation_artist::render_last_frame(void * buffer)
{
  render_next_frame_sub(buffer, frames_count);
}

//////////////////////////////////////////////////////////////
void ggo::distorsion_animation_artist::init_sub()
{
  _hue = ggo::rand<float>();

  for (auto & transform : _transforms)
  {
    transform._center_start.x() = ggo::rand<float>(-0.25, 1.25) * get_width();
    transform._center_start.y() = ggo::rand<float>(-0.25, 1.25) * get_height();
    transform._center_end.x() = ggo::rand<float>(-0.25, 1.25) * get_width();
    transform._center_end.y() = ggo::rand<float>(-0.25, 1.25) * get_height();
    transform._variance = 0.05f * ggo::square(get_min_size());
    
    float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    float length = 0.5f * get_min_size();
    transform._disp = ggo::from_polar(angle, length);
  }

  _stripes.clear();

  float x = 0;
  while (true)
  {
    ggo::distorsion_animation_artist::colored_stripe colored_stripe;
    
    x += ggo::rand<float>(0.01f, 0.06f) * get_min_size();

    colored_stripe._x_sup = x;
    colored_stripe._color = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
    
    _stripes.push_back(colored_stripe);

    if (x > get_width())
    {
      break;
    }
  }
}

//////////////////////////////////////////////////////////////
bool ggo::distorsion_animation_artist::render_next_frame_sub(void * buffer, int frame_index)
{
  if (frame_index > frames_count)
  {
    return false;
  }

  std::vector<fixed_transform> transforms;
  for (auto & transform : _transforms)
  { 
    transforms.emplace_back(
      ggo::ease_inout(frame_index, frames_count, transform._center_start, transform._center_end),
      ggo::ease_inout(frame_index, frames_count, ggo::vec2f(0.f, 0.f), transform._disp),
      transform._variance);
  }

  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    render_t<ggo::rgb_8u_yu>(buffer, *this, transforms);
    break;
  case ggo::bgra_8u_yd:
    render_t<ggo::bgra_8u_yd>(buffer, *this, transforms);
    break;
  }
  
  return true;
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
  x = ggo::pos_mod(x, static_cast<float>(get_width()));
  
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

