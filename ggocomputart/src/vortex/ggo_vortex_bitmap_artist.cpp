#include "ggo_vortex_bitmap_artist.h"
#include "ggo_vortex_artist.h"
#include "ggo_artist.h"

namespace
{
  class vortex_bitmap_artist_internal : public ggo::artist
  {
  public:

    //////////////////////////////////////////////////////////////
    vortex_bitmap_artist_internal(int width, int height) : artist(width, height) {}

    //////////////////////////////////////////////////////////////
    void render_bitmap(void * buffer, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
    {
      const int vortices_count = 20;

      ggo::vortex_artist::params params;
      params._vortices = ggo::array<ggo::vortex_artist::vortex, 1>(vortices_count);
      params._color1 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
      params._color2 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
      params._color3 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
      params._color4 = ggo::from_hsv<ggo::rgb_32f>(ggo::rand<float>(), ggo::rand<float>(0.4f, 0.6f), 1);
      params._split_horz = ggo::rand<float>(0.4f * width(), 0.6f * width());
      params._split_vert = ggo::rand<float>(0.4f * height(), 0.6f * height());

      for (auto & vortex : params._vortices)
      {
        vortex._pos.x() = ggo::rand<float>() * width();
        vortex._pos.y() = ggo::rand<float>() * height();
        vortex._angle = ggo::rand<float>(ggo::pi<float>() / 2, ggo::pi<float>() / 2 + 0.5f);
        vortex._speed = ggo::rand<float>(0.004f, 0.008f) * max_size();
        vortex._power = ggo::rand<float>(6.f, 8.f);
      }

      ggo::vortex_artist::render(buffer, width(), height(), line_byte_step, pixel_type, memory_lines_order, params);
    }
  };
}

//////////////////////////////////////////////////////////////
void ggo::vortex_bitmap_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order) const
{
  vortex_bitmap_artist_internal artist(width, height);

  artist.render_bitmap(buffer, line_byte_step, pixel_type, memory_lines_order);
}



