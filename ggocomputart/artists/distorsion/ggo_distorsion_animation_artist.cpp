#include "ggo_distorsion_animation_artist.h"
#include <ggo_kernel.h>
#include <ggo_fill.h>

namespace 
{
  const int frames_count = 300;
}

//////////////////////////////////////////////////////////////
ggo::distorsion_animation_artist::distorsion_animation_artist(int render_width, int render_height)
:
animation_artist_abc(render_width, render_height),
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
    transform._center_start.get<0>() = ggo::rand<float>(-0.25, 1.25) * get_render_width();
    transform._center_start.get<1>() = ggo::rand<float>(-0.25, 1.25) * get_render_height();
    transform._center_end.get<0>() = ggo::rand<float>(-0.25, 1.25) * get_render_width();
    transform._center_end.get<1>() = ggo::rand<float>(-0.25, 1.25) * get_render_height();
    transform._variance = 0.05f * ggo::square(get_render_min_size());
    
    float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    float length = 0.5f * get_render_min_size();
    transform._disp = ggo::from_polar(angle, length);
  }

  _stripes.clear();

  float x = 0;
  while (true)
  {
    ggo::distorsion_animation_artist::colored_stripe colored_stripe;
    
    x += ggo::rand<float>(0.01f, 0.06f) * get_render_min_size();

    colored_stripe._x_sup = x;
    colored_stripe._color = ggo::from_hsv<ggo::color_8u>(_hue, ggo::rand<float>(), ggo::rand<float>());
    
    _stripes.push_back(colored_stripe);

    if (x > get_render_width())
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

  uint8_t * ptr = static_cast<uint8_t *>(buffer);

  for (int y = 0; y < get_render_height(); ++y)
  {
    for (int x = 0; x < get_render_width(); ++x)
    {
      float x1 = x - 0.25f;
      float x2 = x + 0.25f;
      float y1 = y - 0.25f;
      float y2 = y + 0.25f;

      auto stripe1 = get_stripe_at(transform(x1, y1, transforms));
      auto stripe2 = get_stripe_at(transform(x1, y2, transforms));
      auto stripe3 = get_stripe_at(transform(x2, y1, transforms));
      auto stripe4 = get_stripe_at(transform(x2, y2, transforms));
      
      ggo::color_8u pixel_color(ggo::black<ggo::color_8u>());
      
      // 4 samples are of the same color, we don't go further.
      if (stripe1 == stripe2 && stripe2 == stripe3 && stripe3 == stripe4)
      {
        pixel_color = stripe1->_color;
      }
      // Full processing.
      else
      {
        for (int y_i = -7; y_i <= 7; y_i += 2)
        {
          float y_f = y + y_i / 16.f;
          
          for (int x_i = -7; x_i <= 7; x_i += 2)
          {
            float x_f = x + x_i / 16.f;

            auto stripe = get_stripe_at(transform(x_f, y_f, transforms));
              
            pixel_color += stripe->_color;
          }
        }
        
        pixel_color /= 64;
      }
        
      ptr[0] = pixel_color.r();
      ptr[1] = pixel_color.g();
      ptr[2] = pixel_color.b();
      
      ptr += 3;
    } 
  }
  
  return true;
}

//////////////////////////////////////////////////////////////
float ggo::distorsion_animation_artist::transform(float x, float y, const std::vector<fixed_transform> & transforms)
{
  float result = x;
  
  for (const auto & transform : transforms)
  {
    float dx = x - transform._center.get<0>();
    float dy = y - transform._center.get<1>();
    
    float influence = std::exp(-(dx * dx + dy * dy) / transform._variance);
      
    result += influence * transform._disp.get<0>();
  }
    
  return result;
}

//////////////////////////////////////////////////////////////
std::vector<ggo::distorsion_animation_artist::colored_stripe>::const_iterator ggo::distorsion_animation_artist::get_stripe_at(float x) const
{
  x = ggo::pos_mod(x, static_cast<float>(get_render_width()));
  
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

