#include "ggo_duffing_bitmap_artist.h"
#include "ggo_duffing.h"
#include <2d/processing/ggo_blit.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/paint/ggo_brush.h>
#include <2d/paint/ggo_blend.h>

//////////////////////////////////////////////////////////////
ggo::duffing_bitmap_artist::duffing_bitmap_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
bitmap_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
{
}

//////////////////////////////////////////////////////////////
void ggo::duffing_bitmap_artist::render_bitmap(void * buffer) const
{
  const int iterations_count = 5000000;

	// Compute the points.
	std::cout << "Computing points" << std::endl;
	
	std::vector<ggo::pos2_f>	points;

  ggo::duffing duffing;

	const float angle_offset = ggo::rand<float>(0, 2 * ggo::pi<float>());
  
	for (int i = 0; i < iterations_count; ++i)
	{
    ggo::pos2_f point = duffing.update(0.002f);

		float angle = atan2(point.y(), point.x()) + angle_offset;
		float dist = ggo::length(point);

    point = dist * ggo::vec2_f::from_angle(angle);
    point = map_fit(point, -1.7f, 1.7f);
		
		points.push_back(point);
	}

	float radius = min_size() / 1500.f;
	ggo::image_t<ggo::pixel_type::rgb_32f, ggo::lines_order::up> img_float(size());

	// Render the background.
	std::cout << "Rendering background" << std::endl;
	
  const ggo::rgb_32f color1 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	const ggo::rgb_32f color2 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	const ggo::rgb_32f color3 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	const ggo::rgb_32f color4 = ggo::from_hsv<ggo::rgb_32f>(0, 0, ggo::rand<float>(0.5, 1));
	
	ggo::fill_4_colors(img_float, color1, color2, color3, color4);

	// Render the shadow points.
	std::cout << "Rendering the shadow" << std::endl;
	
  ggo::image_t<ggo::pixel_type::y_32f, ggo::lines_order::up> img_shadow(size());
  ggo::fill_solid(img_shadow, 1.f);
  for (const auto & point : points)
	{
		// Offset the shadow.
		ggo::pos2_f render_pt = point;
		render_pt.x() += 0.05f * min_size();
		render_pt.y() += 0.05f * min_size();

		ggo::paint<ggo::sampling_2x2>(img_shadow, ggo::disc_f(render_pt, radius), 0.f, 0.2f);
	}

	// Blur and blend the shadow.
	std::cout << "Blurring the shadow" << std::endl;
	
  ggo::gaussian_blur(img_shadow, 0.4f * min_size());

	std::cout << "Blending the shadow" << std::endl;

  for (int y = 0; y < img_float.height(); ++y)
  {
    for (int x = 0; x < img_float.height(); ++x)
    {
      ggo::rgb_32f c = img_float.read_pixel(x, y);
      c *= img_shadow.read_pixel(x, y);
      img_float.write_pixel(x, y, c);
    }
  }

	// Render the points.
	std::cout << "Rendering points" << std::endl;
	
	ggo::cubic_curve_float hue_curve;
	hue_curve.push_point(0, ggo::rand<float>());
	hue_curve.push_point(1, ggo::rand<float>());
	for (int i = 1; i <= 3; ++i)
	{
		hue_curve.push_point(i / 4.f, ggo::rand<float>());
	}
	
	for (int i = 0; i < points.size(); ++i)
	{
		float t = float(i) / points.size();
		float hue = hue_curve.evaluate(t);
		ggo::rgb_32f color = ggo::from_hsv<ggo::rgb_32f>(hue, 1, 1);

    ggo::paint<ggo::sampling_4x4>(img_float, ggo::disc_f(points[i], radius), color, 0.02f);
	}

	// From float to uint8_t.
  ggo::blit(img_float, image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, size(), line_byte_step()));
}
