#ifndef __GGO_PLASTIC_ARTIST__
#define __GGO_PLASTIC_ARTIST__

#include <ggo_artist.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class plastic_artist : public artist
  {
  public:

    struct params
    {
      float _pos_x;
      float _pos_y;
      float _den;
      float _mult;
      float _power;
    };

  public:

          plastic_artist(int width, int height, int line_step, ggo::image_format format);

    void	render(void * buffer, const std::vector<ggo::plastic_artist::params> & params, const ggo::rgb_32f & color, float altitude_factor) const;

  private:

    float	evaluate(float x, float y, const std::vector<ggo::plastic_artist::params> & params) const;

    template <ggo::image_format format>
    void render_t(void * buffer, int line_step, const std::vector<ggo::plastic_artist::params> & params, const ggo::rgb_32f & color, float altitude_factor) const;
  };
}

#include "ggo_plastic_artist.imp.h"

#endif