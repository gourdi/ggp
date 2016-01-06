#include "ggo_voronoi_artist.h"
#include <cstring>

#define GGO_POINTS_COUNT 500

//////////////////////////////////////////////////////////////
ggo_voronoi_artist::ggo_voronoi_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_voronoi_artist::render_bitmap(uint8_t * buffer)
{
	ggo::color color1;
  ggo::color color2;
  ggo::color color3;
  ggo::color color4;
  
  color1.randomize();
  color2.randomize();
  color3.randomize();
  color4.randomize();
  
  _points.clear();
  for (int i = 0; i < GGO_POINTS_COUNT; ++i)
  {
    ggo_voronoi_point voronoi_point;
    
    voronoi_point._position = ggo::point2d_float(ggo::rand_float(), ggo::rand_float());
    
    float r1 = voronoi_point._position.x()*color1.r() + (1-voronoi_point._position.x())*color2.r();
    float g1 = voronoi_point._position.x()*color1.g() + (1-voronoi_point._position.x())*color2.g();
    float b1 = voronoi_point._position.x()*color1.b() + (1-voronoi_point._position.x())*color2.b();
    
    float r2 = voronoi_point._position.x()*color3.r() + (1-voronoi_point._position.x())*color4.r();
    float g2 = voronoi_point._position.x()*color3.g() + (1-voronoi_point._position.x())*color4.g();
    float b2 = voronoi_point._position.x()*color3.b() + (1-voronoi_point._position.x())*color4.b();
    
    float r = voronoi_point._position.y()*r1 + (1-voronoi_point._position.y())*r2;
    float g = voronoi_point._position.y()*g1 + (1-voronoi_point._position.y())*g2; 
    float b = voronoi_point._position.y()*b1 + (1-voronoi_point._position.y())*b2; 
    
    voronoi_point._r = ggo::clamp(ggo::to<int>(255 * r) + ggo::rand_int(-10, 10), 0, 255);
    voronoi_point._g = ggo::clamp(ggo::to<int>(255 * g) + ggo::rand_int(-10, 10), 0, 255);
    voronoi_point._b = ggo::clamp(ggo::to<int>(255 * b) + ggo::rand_int(-10, 10), 0, 255);

    voronoi_point._position.x() *= get_render_width();
    voronoi_point._position.y() *= get_render_height();
      
    _points.push_back(voronoi_point);
  }

	memset(buffer, 0, 3 * get_render_width() * get_render_height());

	int counter = 0;
	while (true)
	{
		const int BLOCK_SIZE = 4;

		int horz_blocks = get_render_width() / BLOCK_SIZE;
		int block_x = counter % horz_blocks;
		int block_y = counter / horz_blocks;

		int x_from = BLOCK_SIZE * block_x;
		int y_from = BLOCK_SIZE * block_y;

		if (y_from > get_render_height())
		{
			break;
		}

		int x_to = std::min(BLOCK_SIZE * (block_x + 1) - 1, get_render_width()- 1);
		int y_to = std::min(BLOCK_SIZE * (block_y + 1) - 1, get_render_height()- 1);

		int i1 = check_point(x_from - 3 / 8.f, y_from - 3 / 8.f);
		int i2 = check_point(x_from - 3 / 8.f, y_to + 3 / 8.f);
		int i3 = check_point(x_to + 3 / 8.f, y_from - 3 / 8.f);
		int i4 = check_point(x_to + 3 / 8.f, y_to + 3 / 8.f);

		if ((i1 == i2) && (i2 == i3) && (i3 == i4))
		{
			fill_block(buffer, x_from, x_to, y_from, y_to,
					_points[i1]._r, _points[i1]._g, _points[i1]._b);
		}
		else
		{
			process_block(buffer, x_from, x_to, y_from, y_to);
		}
		
		++counter;
	}
}

//////////////////////////////////////////////////////////////
void ggo_voronoi_artist::fill_block(uint8_t * buffer, int x_from, int x_to, int y_from, int y_to, int r, int g, int b) const
{
	for (int y = y_from; y <= y_to; ++y)
	{
		uint8_t * it = buffer + 3 * (y * get_render_width() + x_from);
		
		for (int x = x_from; x <= x_to; ++x)
		{
			it[0] = r;
			it[1] = g;
			it[2] = b;
			it += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
void  ggo_voronoi_artist::process_block(uint8_t * buffer, int x_from, int x_to, int y_from, int y_to) const
{
	for (int y = y_from; y <= y_to; ++y)
	{
		uint8_t * it = buffer + 3 * (y * get_render_width() + x_from);
		
		for (int x = x_from; x <= x_to; ++x)
		{
			int r = 0;
			int g = 0;
			int b = 0;
		
			sample(x - 3 / 8.f, y - 3 / 8.f, r, g, b);
			sample(x - 3 / 8.f, y - 1 / 8.f, r, g, b);
			sample(x - 3 / 8.f, y + 1 / 8.f, r, g, b);
			sample(x - 3 / 8.f, y + 3 / 8.f, r, g, b);
			
			sample(x - 1 / 8.f, y - 3 / 8.f, r, g, b);
			sample(x - 1 / 8.f, y - 1 / 8.f, r, g, b);
			sample(x - 1 / 8.f, y + 1 / 8.f, r, g, b);
			sample(x - 1 / 8.f, y + 3 / 8.f, r, g, b);
			
			sample(x + 1 / 8.f, y - 3 / 8.f, r, g, b);
			sample(x + 1 / 8.f, y - 1 / 8.f, r, g, b);
			sample(x + 1 / 8.f, y + 1 / 8.f, r, g, b);
			sample(x + 1 / 8.f, y + 3 / 8.f, r, g, b);
			
			sample(x + 3 / 8.f, y - 3 / 8.f, r, g, b);
			sample(x + 3 / 8.f, y - 1 / 8.f, r, g, b);
			sample(x + 3 / 8.f, y + 1 / 8.f, r, g, b);
			sample(x + 3 / 8.f, y + 3 / 8.f, r, g, b);
			
			it[0] = (r + 8) / 16;
			it[1] = (g + 8) / 16;
			it[2] = (b + 8) / 16;
			it += 3;
		}
	}
}

//////////////////////////////////////////////////////////////
void  ggo_voronoi_artist::sample(float x, float y, int & r, int & g, int & b) const
{
	int i = check_point(x, y);

	r += _points[i]._r;
	g += _points[i]._g;
	b += _points[i]._b;
}

//////////////////////////////////////////////////////////////
int ggo_voronoi_artist::check_point(float x, float y) const
{
	int result = 0;
	float dist = ggo::hypot(x, y, _points[0]._position.x(), _points[0]._position.y());

	for (int i = 1; i < static_cast<int>(_points.size()); ++i)
	{
		float dist_cur = ggo::hypot(x, y, _points[i]._position.x(), _points[i]._position.y());
		if (dist_cur < dist)
		{
			result = i;
			dist = dist_cur;
		}
	}

	return result;
}

