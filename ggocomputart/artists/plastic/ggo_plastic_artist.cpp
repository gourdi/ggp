#include "ggo_plastic_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <ggo::pixel_buffer_format pbf>
  void render_t(void * buffer, const ggo::plastic_artist & artist,
    const std::vector<ggo::plastic_artist::params> & params,
    const ggo::color_32f & color, float altitude_factor)
  {
    const float width_f = static_cast<float>(artist.get_width());
    const float height_f = static_cast<float>(artist.get_height());
    const float range_x = width_f > height_f ? width_f / height_f : 1.f;
    const float range_y = width_f > height_f ? 1.f : height_f / width_f;

    for (int y = 0; y < artist.get_height(); ++y)
    {
      const float y1 = ggo::map(y - 3 / 8.f, 0.f, height_f, -range_y, range_y);
      const float y2 = ggo::map(y + 3 / 8.f, 0.f, height_f, -range_y, range_y);

      void * ptr = ggo::get_line_ptr<ggo::pixel_buffer_format_info<pbf>::y_dir>(buffer, y, artist.get_height(), artist.get_line_step());

      for (int x = 0; x < artist.get_width(); ++x)
      {
        // Compute altitude.
        const float x1 = ggo::map(x - 3 / 8.f, 0.f, width_f, -range_x, range_x);
        const float x2 = ggo::map(x + 3 / 8.f, 0.f, width_f, -range_x, range_x);

        const float z11 = artist.evaluate(x1, y1, params);
        const float z12 = artist.evaluate(x1, y2, params);
        const float z21 = artist.evaluate(x2, y1, params);
        const float z22 = artist.evaluate(x2, y2, params);

        // Render the slope.
        const float dx = x2 - x1;
        const float dy = y2 - y1;
        const ggo::vec3f v1(dx, dy, z22 - z11);
        const ggo::vec3f v2(dx, -dy, z21 - z12);
        ggo::vec3f normal = ggo::cross(v1, v2);
        normal.normalize();

        const ggo::color_32f pixel_color = color * (altitude_factor + std::abs(normal.get<2>()) / altitude_factor);

        ggo::write_pixel<pbf>(buffer, ggo::convert_color_to<ggo::color_8u>(pixel_color));

        buffer = ggo::ptr_offset(buffer, ggo::pixel_buffer_format_info<pbf>::pixel_byte_size);
      }
    }
  }
}

//////////////////////////////////////////////////////////////
ggo::plastic_artist::plastic_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
artist(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::plastic_artist::render(void * buffer, const std::vector<ggo::plastic_artist::params> & params, const ggo::color_32f & color, float altitude_factor) const
{
  switch (get_pixel_buffer_format())
  {
  case ggo::rgb_8u_yu:
    render_t<ggo::rgb_8u_yu>(buffer, *this, params, color, altitude_factor);
    break;
  case ggo::bgra_8u_yd:
    render_t<ggo::bgra_8u_yd>(buffer, *this, params, color, altitude_factor);
    break;
  default:
    GGO_FAIL();
    break;
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

