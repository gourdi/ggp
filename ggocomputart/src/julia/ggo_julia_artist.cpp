#include "ggo_julia_artist.h"
#include <kernel/math/interpolation/ggo_curve.h>

//////////////////////////////////////////////////////////////
ggo::julia_artist::julia_artist()
{
	// Build the palette.
	float hue = ggo::rand<float>();
	
	ggo::linear_curve_float sat_curve;
	sat_curve.push_point(0, ggo::rand<float>(0, 0.25));
	sat_curve.push_point(1, ggo::rand<float>());
	
	ggo::linear_curve_float val_curve;
	val_curve.push_point(0, ggo::rand<float>(0.75, 1));
	val_curve.push_point(1, ggo::rand<float>(0, 0.25));
	
  int i = 0;
	for (ggo::rgb_8u & c : _palette)
	{
		float x = float(i) / _palette.size();
		
		float sat = sat_curve.evaluate(x);
		float val = val_curve.evaluate(x);
		
		c = ggo::from_hsv<ggo::rgb_8u>(hue, sat, val);

    ++i;
	}
}
	
//////////////////////////////////////////////////////////////
std::complex<float> ggo::julia_artist::pickup_seed()
{
	if (ggo::rand<bool>())
	{	
		// Pick the seek near the main cardioid of the Mandelbrot set.
		float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
		float radius = (1 - cos(angle)) / 2 + ggo::rand<float>(0, 0.01f);
	
		return std::complex<float>(0.25f + radius * std::cos(angle), radius * std::sin(angle));
	}
	else
	{
		// Pick the seek near the circle on the left of the main cardioid.
		float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
		float radius = 0.25f + ggo::rand<float>(0, 0.01f);

		return std::complex<float>(-1 + radius * std::cos(angle), radius * std::sin(angle));	
	}
}

//////////////////////////////////////////////////////////////
int ggo::julia_artist::iterate(float x, float y, const std::complex<float> & seed) const
{
  std::complex<float> z(x, y);

  int i = 0;
  for (i = 0; i < static_cast<int>(_palette.size()); ++i)
  {
    z = z * z + seed;

    if (z.real() * z.real() + z.imag() * z.imag() > 10)
    {
      break;
    }
  }

  return i;
}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::julia_artist::render_bitmap_t(image_t<pixel_type, memory_lines_order> & img, const std::complex<float> & seed, float range) const
{
  for (int y = 0; y < img.height(); ++y)
  {
    float range_x = img.width() > img.height() ? range * img.width() / img.height() : range;
    float range_y = img.width() > img.height() ? range : range * img.height() / img.width();

    float y1 = ggo::map(y - 3 / 8.f, 0.f, static_cast<float>(img.height()), -range_y, range_y);
    float y2 = ggo::map(y - 1 / 8.f, 0.f, static_cast<float>(img.height()), -range_y, range_y);
    float y3 = ggo::map(y + 1 / 8.f, 0.f, static_cast<float>(img.height()), -range_y, range_y);
    float y4 = ggo::map(y + 3 / 8.f, 0.f, static_cast<float>(img.height()), -range_y, range_y);
    int iterations[16];

    for (int x = 0; x < img.width(); ++x)
    {
      // Iterate and sample.
      float x1 = ggo::map(x - 3 / 8.f, 0.f, static_cast<float>(img.width()), -range_x, range_x);
      float x2 = ggo::map(x - 1 / 8.f, 0.f, static_cast<float>(img.width()), -range_x, range_x);
      float x3 = ggo::map(x + 1 / 8.f, 0.f, static_cast<float>(img.width()), -range_x, range_x);
      float x4 = ggo::map(x + 3 / 8.f, 0.f, static_cast<float>(img.width()), -range_x, range_x);

      iterations[0] = iterate(x1, y1, seed);
      iterations[1] = iterate(x1, y4, seed);
      iterations[2] = iterate(x4, y1, seed);
      iterations[3] = iterate(x4, y4, seed);

      if ((iterations[0] == iterations[1]) && (iterations[1] == iterations[2]) && (iterations[2] == iterations[3]))
      {
        int index = std::min(static_cast<int>(_palette.size() - 1), iterations[0]);

        img.write_pixel(x, y, _palette[index]);
      }
      else
      {
        iterations[4] = iterate(x1, y2, seed);
        iterations[5] = iterate(x1, y3, seed);

        iterations[6] = iterate(x2, y1, seed);
        iterations[7] = iterate(x2, y2, seed);
        iterations[8] = iterate(x2, y3, seed);
        iterations[9] = iterate(x2, y4, seed);

        iterations[10] = iterate(x3, y1, seed);
        iterations[11] = iterate(x3, y2, seed);
        iterations[12] = iterate(x3, y3, seed);
        iterations[13] = iterate(x3, y4, seed);

        iterations[14] = iterate(x4, y2, seed);
        iterations[15] = iterate(x4, y3, seed);

        int r = 0;
        int g = 0;
        int b = 0;
        for (int i = 0; i < 16; ++i)
        {
          int index = std::min(static_cast<int>(_palette.size() - 1), iterations[i]);
          r += _palette[index].r();
          g += _palette[index].g();
          b += _palette[index].b();
        }

        ggo::rgb_8u c_8u(uint8_t((r + 8) / 16), uint8_t((g + 8) / 16), uint8_t((b + 8) / 16));

        img.write_pixel(x, y, c_8u);
      }
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::julia_artist::render_bitmap(void * buffer, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order,
                                      const std::complex<float> & seed, float range) const
{
  if (pixel_type == ggo::pixel_type::bgrx_8u && memory_lines_order == ggo::lines_order::down)
  {
    image_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down> img(buffer, { width, height }, line_byte_step);
    render_bitmap_t(img, seed, range);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::up)
  {
    image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> img(buffer, { width, height }, line_byte_step);
    render_bitmap_t(img, seed, range);
  }
  else if (pixel_type == ggo::pixel_type::rgb_8u && memory_lines_order == ggo::lines_order::down)
  {
    image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down> img(buffer, { width, height }, line_byte_step);
    render_bitmap_t(img, seed, range);
  }
  else
  {
    GGO_FAIL();
  }
}

