#include "ggo_plastic_artist.h"

//////////////////////////////////////////////////////////////
ggo::plastic_artist::plastic_artist(int width, int height, int line_step, ggo::image_format format)
:
artist(width, height, line_step, format)
{

}

//////////////////////////////////////////////////////////////
template <ggo::image_format format>
void ggo::plastic_artist::render_t(void * buffer, int line_step, const std::vector<ggo::plastic_artist::params> & params, const ggo::rgb_32f & color, float altitude_factor) const
{
  using format_traits = ggo::image_format_traits<format>;

  const float width_f = static_cast<float>(width());
  const float height_f = static_cast<float>(height());
  const float range_x = width_f > height_f ? width_f / height_f : 1.f;
  const float range_y = width_f > height_f ? 1.f : height_f / width_f;

  for (int y = 0; y < height(); ++y)
  {
    const float y1 = ggo::map(y - 3 / 8.f, 0.f, height_f, -range_y, range_y);
    const float y2 = ggo::map(y + 3 / 8.f, 0.f, height_f, -range_y, range_y);

    void * ptr = ggo::get_line_ptr<format_traits::lines_order>(buffer, y, height(), line_step);

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
      const ggo::vec3f v1(dx, dy, z22 - z11);
      const ggo::vec3f v2(dx, -dy, z21 - z12);
      ggo::vec3f normal = ggo::cross(v1, v2);
      normal.normalize();

      const ggo::rgb_32f pixel_color = color * (altitude_factor + std::abs(normal.get<2>()) / altitude_factor);

      ggo::write_pixel<format>(buffer, ggo::convert_color_to<ggo::rgb_8u>(pixel_color));

      buffer = ggo::ptr_offset<ggo::image_format_traits<format>::pixel_byte_size>(buffer);
    }
  }
}

//////////////////////////////////////////////////////////////
void ggo::plastic_artist::render(void * buffer, const std::vector<ggo::plastic_artist::params> & params, const ggo::rgb_32f & color, float altitude_factor) const
{
  switch (format())
  {
  case ggo::rgb_8u_yu:
    render_t<ggo::rgb_8u_yu>(buffer, line_step(), params, color, altitude_factor);
    break;
  case ggo::bgrx_8u_yd:
    render_t<ggo::bgrx_8u_yd>(buffer, line_step(), params, color, altitude_factor);
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

