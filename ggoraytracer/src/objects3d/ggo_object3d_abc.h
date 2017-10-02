#ifndef __GGO_OBJECT3D_ABC__
#define __GGO_OBJECT3D_ABC__

#include <ggo_shapes3d.h>
#include <ggo_color.h>
#include <ggo_raytracer_global.h>
#include <ggo_raytracer.h>

// object3d_abc
//  |
//  +--> glow
//  |
//  +--> point_light
//  |
//  +--> shape_object3d_abc
//          |
//          +--> simple_color_object3d
//          |
//          +--> reflection_object3d_abc
//                  |
//                  +--> diffuse_object3d
//                  |
//                  +--> transparent_object3d
//                  |
//                  +--> shape_light

namespace ggo
{
  constexpr uint32_t no_flags = 0;
  constexpr uint32_t discard_basis = 1 << 0;
  constexpr uint32_t discard_roughness = 1 << 1;
  constexpr uint32_t discard_phong = 1 << 2;
  constexpr uint32_t discard_reflection = 1 << 3;
  constexpr uint32_t discard_all = 0xffffffff;

  //////////////////////////////////////////////////////////////
  // Abstract base class.
  class object3d_abc
  {
  public:

    object3d_abc() = default;
    object3d_abc(const object3d_abc &) = delete;
    void operator=(const object3d_abc &) = delete;

    virtual ggo::color_32f                    get_color(const ggo::pos3f & pos) const = 0;
    virtual ggo::color_32f                    get_emissive_color() const = 0;
    virtual const object3d_abc *              handle_self_intersection(ggo::ray3d_float & ray) const = 0;
    virtual ggo::pos3f                        sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const = 0;
    virtual ggo::ray3d_float                  sample_ray(float random_variable1, float random_variable2) const = 0;
    virtual std::optional<intersection_data>  intersect_ray(const ggo::ray3d_float & ray) const = 0;
    virtual ggo::color_32f                    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const = 0;
  };
}

#endif