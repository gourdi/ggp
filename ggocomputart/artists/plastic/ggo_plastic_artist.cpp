#include "ggo_plastic_artist.h"

//////////////////////////////////////////////////////////////
ggo::plastic_artist::plastic_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
artist(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::plastic_artist::render(void * buffer, const std::vector<ggo::plastic_artist::params> & params, const ggo::color_32f & color, float altitude_factor) const
{
	float range_x = get_width() > get_height() ? get_width() / float(get_height()) : 1;
	float range_y = get_width() > get_height() ? 1 : get_height() / float(get_width());

	for (int y = 0; y < get_height(); ++y)
	{
		float y1 = ggo::map(y - 3 / 8.f, 0.f, static_cast<float>(get_height()), -range_y, range_y);
		float y2 = ggo::map(y + 3 / 8.f, 0.f, static_cast<float>(get_height()), -range_y, range_y);
	
		for (int x = 0; x < get_width(); ++x)
		{
			// Compute altitude.
			float x1 = ggo::map(x - 3 / 8.f, 0.f, static_cast<float>(get_width()), -range_x, range_x);
			float x2 = ggo::map(x + 3 / 8.f, 0.f, static_cast<float>(get_width()), -range_x, range_x);

			float z11 = evaluate(x1, y1, params);
			float z12 = evaluate(x1, y2, params);
			float z21 = evaluate(x2, y1, params);
			float z22 = evaluate(x2, y2, params);

			// Render the slope.
			float dx = x2 - x1;
			float dy = y2 - y1;
			ggo::vec3f v1(dx, dy, z22 - z11);
			ggo::vec3f v2(dx, -dy, z21 - z12);
			ggo::vec3f normal = ggo::cross(v1, v2);
			normal.normalize();
		
			ggo::color_32f pixel_color_32f = color;
      pixel_color_32f *= altitude_factor + std::abs(normal.get<2>()) / altitude_factor;

      const ggo::color_8u pixel_color = ggo::convert_color_to<ggo::color_8u>(pixel_color_32f);

      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), pixel_color);
		}
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

