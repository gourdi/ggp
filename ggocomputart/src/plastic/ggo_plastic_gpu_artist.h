#ifndef __GGO_PLASTIC_GPU_ARTIST__
#define __GGO_PLASTIC_GPU_ARTIST__

#include <ggo_gpu_artist.h>
#include <kernel/ggo_vec2.h>
#include <2d/ggo_color.h>

namespace ggo
{
  class plastic_gpu_artist : public progress_gpu_artist
  {
  public:

    plastic_gpu_artist();

  private:

    std::string                         get_fragment_shader() const override;
    std::map<std::string, ggo::uniform> update(float progress) override;

  private:

    struct params
    {
      float	  _den;
      float	  _mult;
      float   _power;
      pos2_f	_center;
      float	  _angle_start;
      float	  _angle_end;
      float	  _radius;
    };
    std::vector<params> _params;

    ggo::rgb_32f  _color;
    float 		    _altitude_factor;
  };
}

#endif
