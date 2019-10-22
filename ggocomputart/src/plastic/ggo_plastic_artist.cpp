#include "ggo_plastic_artist.h"
#include <kernel/ggo_vec3.h>

//////////////////////////////////////////////////////////////
ggo::plastic_artist::plastic_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
:
artist(width, height, line_byte_step, pixel_type, memory_lines_order)
{

}

//////////////////////////////////////////////////////////////
template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
void ggo::plastic_artist::render_t(void * buffer, const std::vector<ggo::plastic_artist::params> & params, const ggo::rgb_32f & color, float altitude_factor) const
{
  image_t<pixel_type, memory_lines_order> img(buffer, size(), line_byte_step());

  const float width_f = static_cast<float>(width());
  const float height_f = static_cast<float>(height());
  const float range_x = width_f > height_f ? width_f / height_f : 1.f;
  const float range_y = width_f > height_f ? 1.f : height_f / width_f;

  for (int y = 0; y < height(); ++y)
  {
    const float y1 = ggo::map(y - 3 / 8.f, 0.f, height_f, -range_y, range_y);
    const float y2 = ggo::map(y + 3 / 8.f, 0.f, height_f, -range_y, range_y);

    for (int x = 0; x < width(); ++x)
    {
      // Compute altitude.
      const float x1 = ggo::map(x - 3 / 8.f, 0.f, width_f, -range_x, range_x);
      const float x2 = ggo::map(x + 3 / 8.f, 0.f, width_f, -range_x, range_x);

      const float z11 = evaluate(x1, y1, params);
      const float z12 = evaluate(x1, y2, params);
      const float z21 = evaluate(x2, y1, params);
      const float z22 = evaluate(x2, y2, params);

      // Render the slope.
      const float dx = x2 - x1;
      const float dy = y2 - y1;
      const ggo::vec3_f v1(dx, dy, z22 - z11);
      const ggo::vec3_f v2(dx, -dy, z21 - z12);
      const ggo::vec3_f normal = ggo::normalize(ggo::cross(v1, v2));

      const ggo::rgb_32f pixel_color = color * (altitude_factor + std::abs(normal.z()) / altitude_factor);

      img.write_pixel(x, y, ggo::convert_color_to<ggo::rgb_8u>(pixel_color));
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::plastic_artist::render(void * buffer, const std::vector<ggo::plastic_artist::params> & params, const ggo::rgb_32f & color, float altitude_factor) const
{
  if (pixel_type() == ggo::pixel_type::bgrx_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_t<ggo::pixel_type::bgrx_8u, ggo::lines_order::down>(buffer, params, color, altitude_factor);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::up)
  {
    render_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up>(buffer, params, color, altitude_factor);
  }
  else if (pixel_type() == ggo::pixel_type::rgb_8u && memory_lines_order() == ggo::lines_order::down)
  {
    render_t<ggo::pixel_type::rgb_8u, ggo::lines_order::down>(buffer, params, color, altitude_factor);
  }
  else
  {
    GGO_FAIL();
  }
}

//////////////////////////////////////////////////////////////
float ggo::plastic_artist::evaluate(float x, float y, const std::vector<ggo::plastic_artist::params> & params) const
{
	float z = 0;
	for (const auto & param : params)
	{
		float dx = param._pos_x - x;
		float dy = param._pos_y - y;
		
		float r = dx * dx + dy * dy;
		z += param._den / (0.1f + std::pow(r * param._mult, param._power));
		z += param._den / (0.1f + std::pow(r * param._mult, param._power));
	}

	return z;
}

