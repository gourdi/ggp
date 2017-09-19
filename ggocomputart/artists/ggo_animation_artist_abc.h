#ifndef __GGO_ANIMATION_ARTIST_ABC__
#define __GGO_ANIMATION_ARTIST_ABC__

#include <ggo_artist.h>
#include <ggo_artist_ids.h>
#include <ggo_array.h>
#include <ggo_pixel_buffer.h>
#include <mutex>

namespace ggo
{
  class animation_artist_abc : public artist
  {
  public:

    enum rendering_type
    {
      offscreen_rendering,
      realtime_rendering_android,
      realtime_rendering_pc
    };

    static animation_artist_abc * create(animation_artist_id artist_id, int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    virtual void init() = 0;
    virtual bool update() = 0;
    virtual void render_frame(void * buffer, const ggo::rect_int & clipping) = 0;

    int                       get_line_step() const { return _line_step; }
    ggo::pixel_buffer_format  get_pixel_buffer_format() const { return _pbf; }
    rendering_type            get_rendering_type() const { return _rendering_type; }

  protected:

                 animation_artist_abc(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    const int _line_step;
    const ggo::pixel_buffer_format _pbf;
    const rendering_type _rendering_type;
  };
}

#endif
