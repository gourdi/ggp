#ifndef __GGO_ANIMATION_ARTIST_ABC__
#define __GGO_ANIMATION_ARTIST_ABC__

#include <ggo_artist.h>
#include <ggo_artist_ids.h>
#include <ggo_array.h>
#include <ggo_pixel_buffer.h>

namespace ggo
{
  class animation_artist_abc : public artist
  {
  public:

    enum rendering_type
    {
      offscreen_rendering,
      realtime_rendering
    };

    static animation_artist_abc * create(animation_artist_id artist_id, int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    bool            render_next_frame(void * buffer);
    void            init();

    int             get_frame_index() const { return _counter; };
    rendering_type  get_rendering_type() const { return _rendering_type; }

  protected:

                 animation_artist_abc(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    virtual	void init_sub() = 0;

    virtual	bool render_next_frame_sub(void * buffer, int frame_index) = 0;

  private:

    int _counter;
    const rendering_type _rendering_type;
  };
}

// The artist reuses the same buffer each time.
namespace ggo
{
  class accumulation_animation_artist_abc : public animation_artist_abc
  {
  public:

                  accumulation_animation_artist_abc(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

            bool	render_next_frame_sub(void * buffer, int frame_index) override final;

    virtual	void	init_output_buffer(void * buffer) const = 0;
    virtual	bool	render_next_frame_acc(void * buffer, int frame_index) = 0;
  };
}

// The artist uses a background buffer which is set up once at initialization time.
namespace ggo
{
  class static_background_animation_artist_abc : public animation_artist_abc
  {
  public:

                  static_background_animation_artist_abc(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

            bool	render_next_frame_sub(void * buffer, int frame_indexx) override final;

    virtual void	init_bkgd_buffer(void * bkgd_buffer) const = 0;
    virtual	bool	render_next_frame_bkgd(void * buffer, int frame_index) = 0;

  private:

    ggo::array_uint8 _bkgd_buffer;
  };
}

#endif
