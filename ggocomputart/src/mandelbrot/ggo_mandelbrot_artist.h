#ifndef __GGO_MANDELBROT_ARTIST__
#define __GGO_MANDELBROT_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class mandelbrot_artist : public bitmap_artist_abc
  {
  public:

          mandelbrot_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void	render_bitmap(void * buffer) const override;
  };
}

#endif