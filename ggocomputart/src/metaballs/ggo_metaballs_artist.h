// Reference: http://www.geisswerks.com/ryan/BLOBS/blobs.html

#ifndef __GGO_METABALLS_ARTIST__
#define __GGO_METABALLS_ARTIST__

#include <ggo_artist.h>
#include <raytracer/ggo_raytracer_global.h>

namespace ggo
{
  class renderer_abc;

  namespace metaballs_artist
  {
    struct params
    {
      params();

      std::vector<ggo::pos3_f>  _centers;
      float 							      _threshold;
      float					            _phong_factor;
      float					            _phong_shininess;
      ggo::pos3_f				        _light1;
      ggo::pos3_f				        _light2;
      ggo::rgb_32f              _background_color;
    };

    void render_bitmap(void * buffer, int width, int height, int line_step, ggo::image_format format,
      ggo::renderer_abc & renderer, const ggo::metaballs_artist::params & params);
  };
}

#endif

