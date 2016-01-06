#ifndef __GGO_SAMPLER__
#define __GGO_SAMPLER__

#include <ggo_shapes2d.h>
#include <functional>

namespace ggo
{
  using sample_func = std::function<void(float, float)>;

  class pixel_sampler_abc
  {
  public:

    virtual void  sample_pixel(int x, int y, ggo::sample_func fct) const = 0;
    virtual float get_horz_extent() const = 0;
    virtual float get_vert_extent() const = 0;
    virtual bool  is_pixel_aligned() const = 0;
    virtual int   get_samples_count() const = 0;
      
            void  sample_shape(int x, int y, const ggo::samplable_shape2d_abc<float> & shape, ggo::sample_func fct) const;
  };
  
  class pixel_aligned_sampler_abc : public pixel_sampler_abc
  {
  public:

    float get_horz_extent() const override { return 0.5f; }
    float get_vert_extent() const override { return 0.5f; }
    bool  is_pixel_aligned() const override { return true; }
    
    using pixel_sampler_abc::sample_shape;
    
    float sample_shape(int x, int y, const ggo::samplable_shape2d_abc<float> & shape) const;
  };

  class pixel_sampler_1 : public pixel_aligned_sampler_abc
  {
  public:

    void  sample_pixel(int x, int y, ggo::sample_func fct) const override;
     int  get_samples_count() const override { return 1; }
  };

  class pixel_sampler_2X2 : public pixel_aligned_sampler_abc
  {
  public:

    void  sample_pixel(int x, int y, ggo::sample_func fct) const override;
    int   get_samples_count() const override { return 2*2; }
  };

  class pixel_sampler_4X4 : public pixel_aligned_sampler_abc
  {
  public:

    void  sample_pixel(int x, int y, ggo::sample_func fct) const override;
    int   get_samples_count() const override { return 4*4; }
  };

  class pixel_sampler_8X8 : public pixel_aligned_sampler_abc
  {
  public:

    void  sample_pixel(int x, int y, ggo::sample_func fct) const override;
    int   get_samples_count() const override { return 8*8; }
  };

  class pixel_sampler_16X16 : public pixel_aligned_sampler_abc
  {
  public:

    void  sample_pixel(int x, int y, ggo::sample_func fct) const override;
    int   get_samples_count() const override { return 16*16; }
  };
}

#endif
