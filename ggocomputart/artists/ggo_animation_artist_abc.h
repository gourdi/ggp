#ifndef __GGO_ANIMATION_ARTIST_ABC__
#define __GGO_ANIMATION_ARTIST_ABC__

#include <ggo_artist_abc.h>
#include <ggo_artist_ids.h>
#include <ggo_array.h>

class ggo_animation_artist_abc: public ggo_artist_abc
{
public:

	static	ggo_animation_artist_abc *	create(ggo_animation_artist_id artist_id, int render_width, int render_height);

          bool						            render_next_frame(uint8_t * buffer);
          void						            init();
                  
          int							            get_frame_index() const { return _counter; };
	
protected:
	
                                      ggo_animation_artist_abc(int render_width, int render_height);

	virtual	void						            init_sub() = 0;
            
	virtual	bool						            render_next_frame_sub(uint8_t * buffer, int frame_index) = 0;
	
private:
	
	int	_counter;
};

// The artist reuses the same buffer each time.
class ggo_accumulation_animation_artist_abc : public ggo_animation_artist_abc
{
public:

                ggo_accumulation_animation_artist_abc(int render_width, int render_height);

private:

          bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
  
  virtual	void	init_output_buffer(uint8_t * buffer) = 0;
  virtual	bool	render_next_frame_acc(uint8_t * buffer, int frame_index) = 0;
};

// The artist uses a background buffer which is set up once at initialization time.
class ggo_static_background_animation_artist_abc : public ggo_animation_artist_abc
{
public:

                ggo_static_background_animation_artist_abc(int render_width, int render_height);

private:

          bool	render_next_frame_sub(uint8_t * buffer, int frame_index) override;
    
  virtual void	init_bkgd_buffer(uint8_t * bkgd_buffer) = 0;
  virtual	bool	render_next_frame_bkgd(uint8_t * buffer, int frame_index) = 0;
    
private:

  ggo::array_uint8 _bkgd_buffer;
};

// The artist uses a background buffer which is initialized and then be changed by the artist.
class ggo_dynamic_background_animation_artist_abc : public ggo_animation_artist_abc
{
public:

                ggo_dynamic_background_animation_artist_abc(int render_width, int render_height);

private:

	virtual	bool	render_next_frame_sub(uint8_t * buffer, int frame_index);
    
  virtual void	init_bkgd_buffer(uint8_t * bkgd_buffer) = 0;
  virtual	bool	render_next_frame_bkgd(uint8_t * output_buffer, uint8_t * bkgd_buffer, int frame_index) = 0;
    
private:

  ggo::array_uint8 _bkgd_buffer;
};

#endif
