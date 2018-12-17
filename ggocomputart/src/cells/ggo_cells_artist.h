#ifndef __GGO_CELLS_ARTIST__
#define __GGO_CELLS_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class cells_artist : public bitmap_artist_abc
  {
  public:

          cells_artist(int width, int height, int line_step, ggo::image_format format);

  private:

    void	render_bitmap(void * buffer) const override;

  private:

    class cell
    {
    public:

      using real_t = float;

      cell(float left, float right, float top, float bottom, int render_width, int render_height);

      bool                  is_point_inside(float x, float y) const;
      ggo::rect_data<float> get_seed_rect() const;

    private:

      ggo::pos2_f	_centers[3];
      float			  _inv_width;
      float			  _inv_height;
    };

    struct polynom
    {
      float	_deg2;
      float	_deg1;
      float	_deg0;
    };
  };
}

#endif
