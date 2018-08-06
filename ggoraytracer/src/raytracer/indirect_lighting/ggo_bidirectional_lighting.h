#ifndef __GGO_BIDIRECTIONAL_LIGHTING__
#define __GGO_BIDIRECTIONAL_LIGHTING__

#include <raytracer/indirect_lighting/ggo_indirect_lighting_abc.h>

namespace ggo
{
  class multi_sampling_camera_abc;
  class raycaster_abc;
  
  class bidirectional_lighting : public indirect_lighting_abc
  {
  public:
  
                  bidirectional_lighting(const ggo::raycaster_abc & raycaster, const ggo::scene & scene);
    
    ggo::rgb_32f  process(const ggo::ray3d_float & ray,
                          const ggo::ray3d_float & world_normal,
                          const ggo::object3d_abc & hit_object,
                          const ggo::rgb_32f & hit_color,
                          float random_variable1,
                          float random_variable2) const override;
    
  private:
  
    const ggo::raycaster_abc &  _raycaster;
    const ggo::scene &          _scene;
  };
}

#endif
