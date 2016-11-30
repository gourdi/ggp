#include "ggo_plastic_artist.h"

//////////////////////////////////////////////////////////////
ggo::plastic_artist::plastic_artist(int width, int height)
:
artist(width, height)
{

}

//////////////////////////////////////////////////////////////
void ggo::plastic_artist::render(void * buffer, int line_step, ggo::pixel_buffer_format pbf, const std::vector<ggo::plastic_artist::params> & params, const ggo::color_32f & color, float altitude_factor) const
{
  switch (pbf)
  {
  case ggo::rgb_8u_yu:
    render<ggo::rgb_8u_yu>(buffer, line_step, params, color, altitude_factor);
    break;
  case ggo::bgra_8u_yd:
    render<ggo::bgra_8u_yd>(buffer, line_step, params, color, altitude_factor);
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

