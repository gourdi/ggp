#include "ggo_motion_field_artist.h"
#include <ggo_color_conv.h>
#include <ggo_paint.h>
#include <ggo_fill_buffer.h>
#include <ggo_pixel_interpolation.h>
#include <ggo_kernel.h>
using namespace std;

#define GGO_COUNTER_MAX 40
#define GGO_DIV_POW     10

//////////////////////////////////////////////////////////////
ggo_motion_field_artist::ggo_motion_field_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode)
:
ggo_artist_abc(render_width, render_height, rendering_mode),
_motion_field(get_render_width(), get_render_height(), ggo_vector2d_float(0, 0))
{
    _previous_buffer = new uint32_t[3*render_width*render_height];
    _current_buffer = new uint32_t[3*render_width*render_height];
    _accumulation_buffer = new uint32_t[3*render_width*render_height];
    
    _hue = ggo::rand_float();
    _saturation = ggo::rand_float();
    
    _motion_field.fill(ggo_vector2d_float(0, 0));
    create_vortex(0.0, 0.5, 0.0, 0.5);
    create_vortex(0.5, 1.0, 0.0, 0.5);
    create_vortex(0.0, 0.5, 0.5, 1.0);
    create_vortex(0.5, 1.0, 0.5, 1.0);
    
    init_buffers();
}

//////////////////////////////////////////////////////////////
ggo_motion_field_artist::~ggo_motion_field_artist()
{
    delete[] _previous_buffer;
    delete[] _current_buffer;
    delete[] _accumulation_buffer;
}

//////////////////////////////////////////////////////////////
bool ggo_motion_field_artist::process_sub(int counter, uint8_t * buffer)
{
	if (counter >= GGO_COUNTER_MAX)
	{
		return false;
	}

    std::swap(_previous_buffer, _current_buffer);
    
    uint32_t * it = _current_buffer;
    uint32_t * it_acc = _accumulation_buffer;
    
    for (int y = 0; y < get_render_height(); ++y) 
    {
        for (int x = 0; x < get_render_width(); ++x) 
        {
            float src_x = x + 0.5 + _motion_field(x, y).x();
            float src_y = y + 0.5 + _motion_field(x, y).y();
            
            uint32_t r = ggo_interpolate_backgrnd(_previous_buffer+0, get_render_width(), get_render_height(), src_x, src_y, 3, 0);
            uint32_t g = ggo_interpolate_backgrnd(_previous_buffer+1, get_render_width(), get_render_height(), src_x, src_y, 3, 0);
            uint32_t b = ggo_interpolate_backgrnd(_previous_buffer+2, get_render_width(), get_render_height(), src_x, src_y, 3, 0);
            
            *it++ = r;
            *it++ = g;
            *it++ = b;
            
            *it_acc++ += r;
            *it_acc++ += g;
            *it_acc++ += b;
        }
    }

    ggo_color color = ggo_color::from_hsv(_hue, _saturation, 0.3);
    
    for (int i = 0; i < get_render_width()*get_render_height(); ++i) 
    {
        buffer[3*i+0] = std::min<int>(255, color.r() + ggo_round_div(_accumulation_buffer[3*i+0], GGO_DIV_POW));
        buffer[3*i+1] = std::min<int>(255, color.g() + ggo_round_div(_accumulation_buffer[3*i+1], GGO_DIV_POW));
        buffer[3*i+2] = std::min<int>(255, color.b() + ggo_round_div(_accumulation_buffer[3*i+2], GGO_DIV_POW));
    }
    
    return true;
}

//////////////////////////////////////////////////////////////
void ggo_motion_field_artist::render_bkgd(uint8_t * buffer)
{
	ggo_fill_solid_rgb(buffer, get_render_width()*get_render_height(), ggo_color::BLACK);
}

//////////////////////////////////////////////////////////////
void ggo_motion_field_artist::init_buffers()
{
    // Zero buffers.
    memset(_previous_buffer, 0, 3*get_render_width()*get_render_height()*sizeof(uint32_t));
    memset(_current_buffer, 0, 3*get_render_width()*get_render_height()*sizeof(uint32_t));
    memset(_accumulation_buffer, 0, 3*get_render_width()*get_render_height()*sizeof(uint32_t));
    
    // Paint random points.
    uint8_t *buffer = new uint8_t[3*get_render_width()*get_render_height()];
    
    ggo_fill_solid_rgb(buffer, get_render_width()*get_render_height(), ggo_color::BLACK);
    
    ggo_circle_float circle;
    circle.set_radius(0.002*get_render_min_size());

    for (int i = 0; i < 10000; ++i)
    {
        ggo_color color = ggo_color::from_hsv(ggo_clamp<float>(_hue + ggo::rand_float(-0.1, 0.1), 0, 1), ggo_clamp<float>(_saturation + ggo::rand_float(-0.1, 0.1), 0, 1), 1);
        
        circle.set_center(ggo::rand_float(0, get_render_width()), ggo::rand_float(0, get_render_height()));
        ggo_paint_shape_rgb(buffer, get_render_width(), get_render_height(), circle, color, 1); 
    }
    
    for (int i = 0; i < 3*get_render_width()*get_render_height(); ++i) {
        _current_buffer[i] = buffer[i];
    }
    
    delete[] buffer;
}

//////////////////////////////////////////////////////////////
void ggo_motion_field_artist::create_vortex(float x_inf, float x_sup, float y_inf, float y_sup) 
{
    float amplitude = 0.0005*get_render_min_size();
    float phase = ggo::rand_float(-GGO_PI/2, GGO_PI/2); // Always attractive.
    float center_x = ggo::rand_float(x_inf, x_sup);
    float center_y = ggo::rand_float(y_inf, y_sup);
    
    for (int y = 0; y < get_render_height(); ++y) 
    {
        for (int x = 0; x < get_render_width(); ++x) 
        {
            float dx = ggo_map<float>(x, 0, get_render_width(),  0, 1) - center_x;
            float dy = ggo_map<float>(y, 0, get_render_height(), 0, 1) - center_y;
            
            float angle = atan2(dy, dx) + phase;
            float attenuation = 1 / (1 + (dx*dx+dy*dy));
            
            _motion_field(x, y).x() += attenuation * amplitude * cosf(angle);
            _motion_field(x, y).y() += attenuation * amplitude * sinf(angle);
        }
    }
}

