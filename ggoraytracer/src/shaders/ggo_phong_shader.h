#ifndef __GGO_PHONG_SHADER__
#define __GGO_PHONG_SHADER__

#include <ggo_shader_abc.h>

namespace ggo
{
  class phong_shader : public shader_abc
  {
  public:

                phong_shader() = default;
                phong_shader(float phong_factor, float phong_shininess) : _phong_factor(phong_factor), _phong_shininess(phong_shininess) {}

    ggo::color  diffuse_shading(const ggo::color & object_color,
                      const ggo::color & light_color,
                      const ggo::ray3d_float & ray,
                      const ggo::ray3d_float & world_normal,
                      const ggo::ray3d_float & ray_to_light) const override;

    void        set_phong_factor(float factor) { _phong_factor = factor; }
    float       get_phong_factor() const { return _phong_factor; }

    void        set_phong_shininess(float shininess) { _phong_shininess = shininess; }
    float       get_phong_shininess() const { return _phong_shininess; }

  private:

    float _phong_factor = 0;
    float _phong_shininess = 0;
  };
}

#endif
