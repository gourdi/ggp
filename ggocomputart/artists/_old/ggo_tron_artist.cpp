#include "ggo_tron_artist.h"
#include <ggo_paint.h>
#include <ggo_fill_buffer.h>
#include <ggo_color_conv.h>
#include <iostream>
#include <algorithm>
using namespace std;

//////////////////////////////////////////////////////////////
ggo_tron_artist::ggo_tron_artist(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{

}

//////////////////////////////////////////////////////////////
void ggo_tron_artist::circular_clamp(int & pos_x, int & pos_y) const
{
	if ( pos_x < 0 ) { pos_x = _grid.get_size()-1; }
	if ( pos_y < 0 ) { pos_y = _grid.get_size()-1; }
	if ( pos_x >= _grid.get_size() ) { pos_x = 0; }
	if ( pos_y >= _grid.get_size() ) { pos_y = 0; }
}

//////////////////////////////////////////////////////////////
void ggo_tron_artist::update_sub(int counter, std::ostringstream & oss)
{
	int	new_pos_x, new_pos_y, new_dx, new_dy;

	for (int i = 0; i < _trons.get_size(); ++i)
	{
		// Random direction change.
		if (ggo::rand_int(0, _grid.get_size()/4) == 0)
		{
			turn_tron(_trons[i]);
		}

		// Compute next position.
		new_pos_x = _trons[i]._pos_x + _trons[i]._dx;
		new_pos_y = _trons[i]._pos_y + _trons[i]._dy;
		circular_clamp(new_pos_x, new_pos_y);

		// Next cell is not empty.
		if ( _grid( new_pos_x, new_pos_y ) >= 0 )
		{
			// Try to turn.
			if ( turn_tron( _trons[i] ) == true )
			{
				// Compute next position.
				new_pos_x = _trons[i]._pos_x + _trons[i]._dx;
				new_pos_y = _trons[i]._pos_y + _trons[i]._dy;
				circular_clamp( new_pos_x, new_pos_y );
			}
			else
			{
				// The turn failed, respawn the tron.
				do
				{
					new_pos_x = ggo::rand_int( 0, _grid.get_size()-1 );
					new_pos_y = ggo::rand_int( 0, _grid.get_size()-1 );
				}
				while ( _grid( new_pos_x, new_pos_y ) >= 0 );
			}
		}

		// Update the current tron.
		_trons[i]._pos_x = new_pos_x;
		_trons[i]._pos_y = new_pos_y;

		// Update hue.
		_trons[i]._hue = ggo_loop_clampfp<float>( _trons[i]._hue + _trons[i]._delta_hue, 0, 1 );

		// Update the grid.
		_grid( new_pos_x, new_pos_y ) = i;

		++_counter;
		if (is_finished() == true) { break; }
	}
}

//////////////////////////////////////////////////////////////
void ggo_tron_artist::render_sub(uint8_t * buffer)
{
	ggo_color	color;
	
	int	min_size = 0;
	int offset_x = 0;
	int offset_y = 0;
	if (get_render_width() > get_render_height())
	{
		min_size = get_render_height();
		offset_x = (get_render_width() - get_render_height())/2;
	}
	else
	{
		min_size = get_render_width();
		offset_y = (get_render_height() - get_render_width())/2;
	}
	GGO_ASSERT(offset_x >= 0);
	GGO_ASSERT(offset_y >= 0);

	for (int i = 0; i < _trons.get_size(); ++i)
	{
		// Paint the cell.
		int left	= offset_x +  _trons[i]._pos_x    * min_size / _grid.get_size();
		int right	= offset_x + (_trons[i]._pos_x+1) * min_size / _grid.get_size() - 1;
		int bottom	= offset_y +  _trons[i]._pos_y    * min_size / _grid.get_size();
		int top		= offset_y + (_trons[i]._pos_y+1) * min_size / _grid.get_size() - 1;

		ggo_paint_rect_rgb(buffer, get_render_width(), get_render_height(), 
				left, right, top, bottom,
				ggo_color::from_hsv(_trons[i]._hue, _trons[i]._sat, 1));
	}
}

//////////////////////////////////////////////////////////////
void ggo_tron_artist::render_bkgd(uint8_t * buffer)
{
	ggo_fill_solid_rgb(buffer, get_render_width()*get_render_height(), ggo_color::BLACK);
}

//////////////////////////////////////////////////////////////
bool ggo_tron_artist::is_finished() const
{
	return _counter >= _grid.get_size()*_grid.get_size();
}

//////////////////////////////////////////////////////////////
bool ggo_tron_artist::is_finished(int counter) const
{
	return is_finished();
}

//////////////////////////////////////////////////////////////
void ggo_tron_artist::init_sub()
{
	_counter = 0;

	float hue = ggo::rand_float();

	_grid.set_size(ggo::rand_int(20, 80));
	_grid.fill(-1);
	_trons.set_size(ggo::rand_int(5, 9));

	for (int i = 0; i < _trons.get_size(); ++i)
	{
		_trons[i]._hue			= ggo_loop_clampfp<float>(hue + ggo::rand_float(-0.1, 0.1), 0, 1);
		_trons[i]._sat			= ggo::rand_float();
		_trons[i]._delta_hue	= ggo::rand_float(0, 2.0/(_grid.get_size()*_grid.get_size()));
		_trons[i]._pos_x		= ggo::rand_int(0, _grid.get_size() - 1);
		_trons[i]._pos_y		= ggo::rand_int(0, _grid.get_size() - 1);

		switch (ggo::rand_int(0, 3))
		{
		case 0:
			_trons[i]._dx = -1;
			_trons[i]._dy = 0;
			break;
		case 1:
			_trons[i]._dx = 1;
			_trons[i]._dy = 0;
			break;
		case 2:
			_trons[i]._dx = 0;
			_trons[i]._dy = -1;
			break;
		case 3:
			_trons[i]._dx = 0;
			_trons[i]._dy = 1;
			break;
		default:
			GGO_FAIL();
			break;
		}
	}
}

//////////////////////////////////////////////////////////////
bool ggo_tron_artist::turn_tron(ggo_tron & tron) const
{
	vector<ggo_point2d_int>	candidates;
	int						pos_x, pos_y;

	pos_x = tron._pos_x + 1;
	pos_y = tron._pos_y;
	circular_clamp( pos_x,  pos_y );
	if ( _grid( pos_x, pos_y ) < 0 ) { candidates.push_back( ggo_point2d_int( 1, 0 ) ); }

	pos_x = tron._pos_x - 1;
	pos_y = tron._pos_y;
	circular_clamp( pos_x,  pos_y );
	if ( _grid( pos_x, pos_y ) < 0 ) { candidates.push_back( ggo_point2d_int( -1, 0 ) ); }

	pos_x = tron._pos_x;
	pos_y = tron._pos_y + 1;
	circular_clamp( pos_x,  pos_y );
	if ( _grid( pos_x, pos_y ) < 0 ) { candidates.push_back( ggo_point2d_int( 0, 1 ) ); }

	pos_x = tron._pos_x;
	pos_y = tron._pos_y - 1;
	circular_clamp( pos_x,  pos_y );
	if ( _grid( pos_x, pos_y ) < 0 ) { candidates.push_back( ggo_point2d_int( 0, -1 ) ); }

	// All the cells aound the current position are not free, the turn is not allowed.
	if ( candidates.empty() == true ) { return ( false ); }

	// Pick up and random turn.
	random_shuffle( candidates.begin(), candidates.end() );

	tron._dx = candidates[0].x();
	tron._dy = candidates[0].y();

	return true;
}


