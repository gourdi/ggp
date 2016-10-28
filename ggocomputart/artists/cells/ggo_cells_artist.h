#ifndef __GGO_CELLS_ARTIST__
#define __GGO_CELLS_ARTIST__

#include <ggo_bitmap_artist_abc.h>

namespace ggo
{
  class cells_artist : public bitmap_artist_abc
  {
  public:

    cells_artist(int render_width, int render_height);

  private:

    void	render_bitmap(void * buffer) const override;

    float	compute_potential(const std::vector<ggo::pos2f> & centers, int x, int y) const;
    bool	paint_point(uint8_t * buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b, const std::vector<ggo::pos2f> & centers) const;

  private:

    class cell
    {
    public:

      using real_t = float;

      cell(float left, float right, float top, float bottom, int render_width, int render_height);

      bool                  is_point_inside(float x, float y) const;
      ggo::rect_data<float> get_seed_rect() const;

    private:

      ggo::pos2f	_centers[3];
      float			  _inv_render_width;
      float			  _inv_render_height;
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
