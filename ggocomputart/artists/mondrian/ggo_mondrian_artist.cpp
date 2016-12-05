#include "ggo_mondrian_artist.h"
#include <ggo_buffer_fill.h>
#include <ggo_buffer_paint.h>

namespace
{
  struct area
  {
    ggo::polygon2d_float  _rect;
    float                 _hue;
    float                 _sat;
    float                 _val;
  };
}

//////////////////////////////////////////////////////////////
ggo::mondrian_artist::mondrian_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf)
:
bitmap_artist_abc(width, height, line_step, pbf)
{

}

//////////////////////////////////////////////////////////////
void ggo::mondrian_artist::render_bitmap(void * buffer, const bool & quit) const
{
	float	base_hue = ggo::rand<float>();
  float	base_sat = ggo::rand<float>();
	float	range	= static_cast<float>(get_max_size());
	
	std::vector<area>	areas;
	for (int i = 0; i < 15; ++i)
	{
		area	area;
		
		float rnd = ggo::rand<float>();
		if (rnd < 0.2)
		{
      area._hue = 1 - base_hue;
      area._sat = ggo::rand<float>();
      area._val = 1;
		}
		else
		if (rnd < 0.4)
		{
      area._hue = 1 - base_hue;
      area._sat = 0;
      area._val = ggo::rand<float>();
    }
		else
		{
      area._hue = base_hue;
      area._sat = ggo::rand<float>();
      area._val = 1;
    }
		
		float x = ggo::rand<float>(0, static_cast<float>(get_width()));
		float y = ggo::rand<float>(0, static_cast<float>(get_height()));
		float range_x = ggo::rand<float>(0, range);
		float range_y = ggo::rand<float>(0, range);

		ggo::polygon2d_float::create_axis_aligned_box(x - range_x, x + range_x, y - range_y, y + range_y, area._rect);
		areas.push_back(area);
		
		range *= 0.9f;
	}

	float size1 = ggo::rand<float>(0.7f, 1 / 0.7f) * get_min_size() * 0.001f;
	float size2 = ggo::rand<float>(0.7f, 1 / 0.7f) * get_min_size() * 0.01f;
	
	ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(),
    ggo::from_hsv<ggo::color_8u>(base_hue, base_sat, 1),
    ggo::pixel_rect::from_width_height(get_width(), get_height()));
	
	for (int i = 0; i < 200000; ++i)
	{
		float				angle		  = ggo::rand<float>(-0.2f, 0.2f);
		ggo::pos2f  center		= get_random_point();
		ggo::vec2f	direction	= ggo::vec2f(std::cos(angle), std::sin(angle));
	
    ggo::polygon2d_float box;
		ggo::polygon2d_float::create_oriented_box(center, direction, size1, size2, box);

		// Get a color depending of the area.
    float hue = base_hue;
    float sat = base_sat;
    float val = 1;
		for (const auto & area : areas)
		{
			if (area._rect.is_point_inside(center) == true)
			{
        hue = area._hue;
        sat = area._sat;
        val = area._val;
      }
		}

    val *= ggo::rand<float>(0.9f, 1 / 0.9f);

		// Render the current box.
		ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(
      buffer, get_width(), get_height(), get_line_step(), box, ggo::from_hsv<ggo::color_8u>(hue, sat, val));
	}
}

