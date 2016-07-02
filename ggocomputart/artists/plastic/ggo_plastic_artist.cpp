#include "ggo_plastic_artist.h"

//////////////////////////////////////////////////////////////
ggo_plastic_artist::ggo_plastic_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_plastic_artist::render(uint8_t * buffer, const std::vector<ggo_plastic_params> & params, const ggo::color & color, float altitude_factor) const
{
	float range_x = get_render_width() > get_render_height() ? get_render_width() / float(get_render_height()) : 1;
	float range_y = get_render_width() > get_render_height() ? 1 : get_render_height() / float(get_render_width());

	for (int y = 0; y < get_render_height(); ++y)
	{
		float y1 = ggo::map(y - 3 / 8.f, 0.f, static_cast<float>(get_render_height()), -range_y, range_y);
		float y2 = ggo::map(y + 3 / 8.f, 0.f, static_cast<float>(get_render_height()), -range_y, range_y);
	
		for (int x = 0; x < get_render_width(); ++x)
		{
			// Compute altitude.
			float x1 = ggo::map(x - 3 / 8.f, 0.f, static_cast<float>(get_render_width()), -range_x, range_x);
			float x2 = ggo::map(x + 3 / 8.f, 0.f, static_cast<float>(get_render_width()), -range_x, range_x);

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
		
			ggo::color pixel_color = color;
			pixel_color *= altitude_factor + std::abs(normal.get<2>()) / altitude_factor;
		
			buffer[0] = pixel_color.r8();
			buffer[1] = pixel_color.g8();
			buffer[2] = pixel_color.b8();
		
			buffer += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
float ggo_plastic_artist::evaluate(float x, float y, const std::vector<ggo_plastic_params> & params) const
{
	float z = 0;
	for (std::vector<ggo_plastic_params>::const_iterator it = params.begin(); it != params.end(); ++it)
	{
		float dx = it->_pos_x - x;
		float dy = it->_pos_y - y;
		
		float r = dx * dx + dy * dy;
		z += it->_den / (0.1f + std::pow(r * it->_mult, it->_power));
		z += it->_den / (0.1f + std::pow(r * it->_mult, it->_power));
	}

	return z;
}

