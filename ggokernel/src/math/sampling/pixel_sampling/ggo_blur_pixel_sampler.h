#ifndef __GGO_BLUR_SAMPLER__
#define __GGO_BLUR_SAMPLER__

#include <ggo_pixel_sampler_abc.h>

namespace ggo
{
  class blur_pixel_sampler : public ggo::pixel_sampler_abc
  {
  public:

          blur_pixel_sampler(float radius, int steps = 8);

    void  sample_pixel(int x, int y, ggo::sample_func fct) const override;
    float get_horz_extent() const override { return _radius; }
    float get_vert_extent() const override { return _radius; }
    bool  is_pixel_aligned() const override { return false; }
    int   get_samples_count() const override { return static_cast<int>(_samples.size()); }
      
  private:

    std::vector<ggo::pos2f> _samples;
    float                   _radius;
  };
}

#endif
