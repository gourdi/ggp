#include "ggo_topodoko_artist.h"
#include <ggo_gaussian_blur2d.h>
#include <ggo_array.h>
#include <ggo_pbf_paint.h>
#include <ggo_pbf_fill.h>
#include <ggo_blit.h>

namespace
{
  struct color_square
  {
    ggo::polygon2d_float	_square;
    ggo::color_32f        _color;
  };

  //////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf>
  void render_t(void * buffer, const ggo::topodoko_artist & artist, float hue1, float hue2, float square_size, const std::vector<color_square> & color_squares)
  {
    const ggo::color_8u bkgd_color1 = ggo::from_hsv<ggo::color_8u>(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);
    const ggo::color_8u bkgd_color2 = ggo::from_hsv<ggo::color_8u>(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);
    const ggo::color_8u bkgd_color3 = ggo::from_hsv<ggo::color_8u>(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);
    const ggo::color_8u bkgd_color4 = ggo::from_hsv<ggo::color_8u>(ggo::rand_bool() ? hue1 : hue2, 0.5f, 0.5f);

    ggo::fill_4_colors<ggo::rgb_8u_yu>(buffer, artist.get_width(), artist.get_height(), artist.get_line_step(),
      bkgd_color1, bkgd_color2, bkgd_color3, bkgd_color4, ggo::rect_int::from_width_height(artist.get_width(), artist.get_height()));

    std::cout << "Rendering shadow" << std::endl;

    // Render the shadow.
    std::vector<uint8_t> shadow_buffer(artist.get_width() * artist.get_height(), 0xff);

    float shadow_offset_scalar = 0.25f * square_size;
    ggo::pos2f shadow_offset(shadow_offset_scalar, shadow_offset_scalar);

    for (const auto & color_square : color_squares)
    {
      ggo::polygon2d_float square(4);
      for (int i = 0; i < 4; ++i)
      {
        ggo::pos2f point = artist.map_fit(color_square._square.get_point(i) + shadow_offset, 0, 1);

        square.add_point(point);
      }

      ggo::paint_shape<ggo::y_8u_yu, ggo::sampling_4x4>(
        shadow_buffer.data(), artist.get_width(), artist.get_height(), artist.get_width(), square, uint8_t(0x40f));
    }

    ggo::gaussian_blur2d_mirror<ggo::y_8u_yu>(
      shadow_buffer.data(), artist.get_width(), artist.get_height(), artist.get_width(), 0.005f *  artist.get_min_size());

    ggo::blit<ggo::y_8u_yu, pbf>(shadow_buffer.data(), artist.get_width(), artist.get_height(), artist.get_width(),
      buffer, artist.get_width(), artist.get_height(), artist.get_line_step(), 0, 0);

    std::cout << "Rendering squares" << std::endl;

    // Render the coloured squares.
    for (const auto & color_square : color_squares)
    {
      ggo::polygon2d_float square(4);
      for (int i = 0; i < 4; ++i)
      {
        ggo::pos2f point = color_square._square.get_point(i);
        point = artist.map_fit(point, 0, 1);
        square.add_point(point);
      }

      ggo::paint_shape<pbf, ggo::sampling_16x16>(
        buffer, artist.get_width(), artist.get_height(), artist.get_line_step(), square, ggo::convert_color_to<ggo::color_8u>(color_square._color));
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::topodoko_artist::topodoko_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{
	
}

//////////////////////////////////////////////////////////////
void ggo::topodoko_artist::render_bitmap(void * buffer) const
{
	float hue1 = ggo::rand<float>();
	float hue2 = std::fmod(hue1 + ggo::rand<float>(0.25f, 0.75f), 1.f);

  std::vector<color_square>	color_squares;

	const int blocks = ggo::rand<int>(4, 6);
	const int sub_blocks = ggo::rand<int>(4, 6);
	const float square_dist = ggo::rand<float>(0.8f, 0.9f) / (blocks * sub_blocks);
	const float square_size = ggo::rand<float>(0.8f, 0.9f) * square_dist;

	for (int block_y = 0; block_y < blocks; ++block_y)
	{
		for (int block_x = 0; block_x < blocks; ++block_x)
		{
			ggo::pos2f block_center((1 + 2 * block_x) / (2.f * blocks), (1 + 2 * block_y) / (2.f * blocks));
			
			ggo::color_32f color = ggo::from_hsv<ggo::color_32f>(ggo::rand_bool() ? hue1 : hue2, 0.75, 1);
			
			for (int y = 0; y < sub_blocks; ++y)
			{
				for (int x = 0; x < sub_blocks; ++x)
				{
					color_square color_square;
					
					ggo::pos2f square_center(block_center);
					square_center.get<0>() += (x - 0.5f * sub_blocks + 0.5f) * square_dist + (square_dist - square_size) * ggo::rand<float>(-1, 1);
					square_center.get<1>() += (y - 0.5f * sub_blocks + 0.5f) * square_dist + (square_dist - square_size) * ggo::rand<float>(-1, 1);
					
					float dangle = ggo::rand<float>(-0.1f, 0.1f);
					color_square._square.add_point(square_center + ggo::from_polar(    ggo::pi<float>() / 4 + dangle, 0.5f * square_size * ggo::sqrt2<float>()));
					color_square._square.add_point(square_center + ggo::from_polar(3 * ggo::pi<float>() / 4 + dangle, 0.5f * square_size * ggo::sqrt2<float>()));
					color_square._square.add_point(square_center + ggo::from_polar(5 * ggo::pi<float>() / 4 + dangle, 0.5f * square_size * ggo::sqrt2<float>()));
					color_square._square.add_point(square_center + ggo::from_polar(7 * ggo::pi<float>() / 4 + dangle, 0.5f * square_size * ggo::sqrt2<float>()));

					if (ggo::rand<int>(0, 8) == 0)
					{
						color_square._color = ggo::from_hsv<ggo::color_32f>(ggo::rand_bool() ? hue1 : hue2, 0.75, 1);
					}
					else
					{
						color_square._color = color;
					}
					
					color_square._color.r() = ggo::clamp(color_square._color.r() + ggo::rand<float>(-0.15f, 0.15f), 0.f, 1.f);
					color_square._color.g() = ggo::clamp(color_square._color.g() + ggo::rand<float>(-0.15f, 0.15f), 0.f, 1.f);
					color_square._color.b() = ggo::clamp(color_square._color.b() + ggo::rand<float>(-0.15f, 0.15f), 0.f, 1.f);
					
					if (ggo::rand<int>(0, 20) != 0)
					{
						color_squares.push_back(color_square);
					}
				}
			}
		}
	}

  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    render_t<ggo::rgb_8u_yu>(buffer, *this, hue1, hue2, square_size, color_squares);
    break;
  case ggo::bgra_8u_yd:
    render_t<ggo::bgra_8u_yd>(buffer, *this, hue1, hue2, square_size, color_squares);
    break;
    default:
      GGO_FAIL();
      break;
  }
}

