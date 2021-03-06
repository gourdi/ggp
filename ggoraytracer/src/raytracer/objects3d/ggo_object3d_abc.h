#ifndef __GGO_OBJECT3D_ABC__
#define __GGO_OBJECT3D_ABC__

#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <2d/ggo_color.h>
#include <raytracer/ggo_raytracer_global.h>
#include <raytracer/ggo_raytracer.h>

// object3d_abc
//  |
//  +--> point_light
//  |
//  +--> volumetric_effect_abc
//  |       |
//  |       +--> disc_glow
//  |       +--> sphere_glow
//  |       +--> linear_fog
//  |       +--> z_fog
//  |
//  +--> shape_object3d_abc
//          |
//          +--> simple_color_object3d
//          |
//          +--> reflection_object3d_abc  
//                  |
//                  +--> diffuse_object3d
//                  +--> transparent_object3d
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

    virtual const object3d_abc *              handle_self_intersection(ggo::ray3d_f & ray) const = 0;
    virtual std::optional<box3d_data_float>   get_bounding_box() const = 0;

    // Solid object.
    virtual ggo::rgb_32f                      get_color(const ggo::pos3_f & pos) const = 0;
    virtual std::optional<intersection_data>  intersect_ray(const ggo::ray3d_f & ray) const = 0;
    virtual bool                              intersect_segment(const ggo::pos3_f & pos, const ggo::vec3_f & dir, float length) const = 0;
    virtual ggo::rgb_32f                      process_ray(const ggo::ray3d_f & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2) const = 0;
    virtual transmission_data                 compute_transmission(const ggo::ray3d_f & ray, const ggo::ray3d_f & normal, int & depth) const = 0;

    // Light.
    virtual ggo::rgb_32f                      get_emissive_color() const = 0;
    virtual ggo::pos3_f                        sample_point(const ggo::pos3_f & target_pos, float random_variable1, float random_variable2) const = 0;
    virtual ggo::ray3d_f                  sample_ray(float random_variable1, float random_variable2) const = 0;

    // Volumetric object.
    virtual ggo::rgb_32f                      process_segment(const ggo::pos3_f & p1, const ggo::pos3_f & p2, const ggo::rgb_32f & color) const = 0;
    virtual ggo::rgb_32f                      process_background_ray(const ggo::ray3d_f & ray, const ggo::rgb_32f & color) const = 0;
  };
}

#endif