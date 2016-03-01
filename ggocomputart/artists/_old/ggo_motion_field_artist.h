#ifndef __GGO_MOTION_FIELD_ARTIST__
#define __GGO_MOTION_FIELD_ARTIST__

#include "ggo_artist_abc.h"
#include <ggo_array.h>
#include <ggo_array2d.h>
#include <ggo_2d.h>

class ggo_motion_field_artist : public ggo_artist_abc
{
public:
    
                            ggo_motion_field_artist(int render_width, int render_height, ggo_rendering_mode rendering_mode);
    virtual				   ~ggo_motion_field_artist();    
    
private:
   
	virtual	bool			process_sub(int counter, uint8_t * buffer);
	virtual	void			render_bkgd(uint8_t * buffer);
	virtual	ggo_artist_id	get_id() const { return GGO_ARTIST_MOTION_FIELD; };
	virtual	std::string		get_name() const { return "Motion Field"; };
    
            void            init_buffers();
            void            create_vortex(float x_inf, float x_sup, float y_inf, float y_sup);
    
private:
    
    uint32_t *                      _current_buffer;
	uint32_t *                      _previous_buffer;
    uint32_t *                      _accumulation_buffer;
    ggo_array2d<ggo_vector2d_float> _motion_field;
    float                           _hue;
    float                           _saturation;
};

#endif

