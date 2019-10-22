#include "ggo_crystal_artist.h"
#include <kernel/memory/ggo_array.h>
#include <kernel/math/ggo_coordinates_conversions.h>
#include <2d/ggo_color.h>
#include <2d/paint/ggo_blend.h>
#include <2d/fill/ggo_fill.h>

namespace
{
  //////////////////////////////////////////////////////////////
  void render_transform(ggo::image_t<ggo::pixel_type::rgb_32f, ggo::lines_order::up> & img, const ggo::crystal_artist::params & params)
  {
    ggo::pos2_f pt;

    pt.x() = ggo::rand<float>(-1, 1);
    pt.y() = ggo::rand<float>(-1, 1);

    for (int points_count = 0; points_count < 4; ++points_count)
    {
      // Apply transform.
      int i = ggo::rand<int>(0, ggo::crystal_artist::transforms_cout);

      pt.x() = params._coefs[i][0] * pt.x() + params._coefs[i][1] * pt.y() + params._coefs[i][2];
      pt.y() = params._coefs[i][3] * pt.x() + params._coefs[i][4] * pt.y() + params._coefs[i][5];

      // Render point.
      ggo::pos2_f render_pt = ggo::map_fit(pt, -5.f, 5.f, img.width(), img.height());
      int x = ggo::round_to<int>(render_pt.x());
      int y = ggo::round_to<int>(render_pt.y());

      if ((x >= 0) && (x < img.width()) &&
        (y >= 0) && (y < img.height()))
      {
        ggo::rgb_32f c = img.read_pixel(x, y);

        float diff = 4 * (c.r() + c.g() + c.b());
        if (x > 0)
        {
          ggo::rgb_32f c = img.read_pixel(x - 1, y);
          diff -= c.r() + c.g() + c.b();
        }
        if (x < img.width() - 1)
        {
          ggo::rgb_32f c = img.read_pixel(x + 1, y);
          diff -= c.r() + c.g() + c.b();
        }
        if (y > 0)
        {
          ggo::rgb_32f c = img.read_pixel(x, y - 1);
          diff -= c.r() + c.g() + c.b();
        }
        if (y < img.height() - 1)
        {
          ggo::rgb_32f c = img.read_pixel(x, y + 1);
          diff -= c.r() + c.g() + c.b();
        }

        if (diff < 0.25)
        {
          c += { params._dr[i], params._dg[i], params._db[i] };
          img.write_pixel(x, y, c);
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::render_bitmap(ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> & img, const params & params)
{
  image_t<ggo::pixel_type::rgb_32f, ggo::lines_order::up> float_img(img.size());
  fill_black(float_img);

	for (int j = 0; j < 16 * float_img.width() * float_img.height(); ++j)
	{
		render_transform(float_img, params);
	}

	for (int y = 0; y < img.height(); ++y)
	{
    for (int x = 0; x < img.width(); ++x)
    {
      const ggo::rgb_32f c_32f = float_img.read_pixel(x, y);
      const ggo::rgb_8u c_8u(add_blend(img.read_pixel(x, y), ggo::convert_color_to<ggo::rgb_8u>(c_32f)));
      img.write_pixel(x, y, c_8u);
    }
	}
}

//////////////////////////////////////////////////////////////
void ggo::crystal_artist::randomize_params(params & params)
{		
	for (int i = 0; i < transforms_cout; ++i)
	{
		params._coefs[i][0] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][1] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][2] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][3] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][4] = ggo::rand<float>(-1.f, 1.5f);
		params._coefs[i][5] = ggo::rand<float>(-1.f, 1.5f);
		
		params._dr[i] = ggo::rand<float>(0.0005f, 0.001f);
		params._dg[i] = ggo::rand<float>(0.0005f, 0.001f);
		params._db[i] = ggo::rand<float>(0.0005f, 0.001f);
	}
}

