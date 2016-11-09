// Reference: http://www.geisswerks.com/ryan/BLOBS/blobs.html

#ifndef __GGO_METABALLS_ARTIST__
#define __GGO_METABALLS_ARTIST__

#include <ggo_artist.h>
#include <ggo_raytracer_global.h>
#include <ggo_renderer_abc.h>

namespace ggo
{
  class metaballs_artist : public artist
  {
  public:

    struct params
    {
      params();

      std::vector<ggo::pos3f> _centers;
      float 							    _threshold;
      float					          _phong_factor;
      float					          _phong_shininess;
      ggo::pos3f				      _light1;
      ggo::pos3f				      _light2;
      ggo::color_32f          _background_color;
    };

  public:

    metaballs_artist(int render_width, int render_height);

    void 	render_bitmap(void * buffer, ggo::renderer_abc & renderer, const ggo::metaballs_artist::params & params);
  };
}

#endif

