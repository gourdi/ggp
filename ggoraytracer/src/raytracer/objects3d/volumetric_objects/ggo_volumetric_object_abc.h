#ifndef __GGO_FOG_ABC__
#define __GGO_FOG_ABC__

#include <kernel/math/shapes_3d/ggo_shapes3d.h>
#include <2d/ggo_color.h>
#include <raytracer/objects3d/ggo_object3d_abc.h>

namespace ggo
{
  class volumetric_object_abc : public object3d_abc
  {
  public:

    const object3d_abc *              handle_self_intersection(ggo::ray3d_float & ray) const override { GGO_FAIL(); return nullptr; }

    // Solid object.
    ggo::color_32f                    get_color(const ggo::pos3f & pos) const override { GGO_FAIL(); return ggo::black_32f(); }
    std::optional<intersection_data>  intersect_ray(const ggo::ray3d_float & ray) const override { GGO_FAIL(); return {}; }
    bool                              intersect_segment(const ggo::pos3f & pos, const ggo::vec3f & dir, float length) const override { GGO_FAIL(); return false; }
    ggo::color_32f                    process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, const ggo::indirect_lighting_abc * indirect_lighting, float random_variable1, float random_variable2) const override { GGO_FAIL(); return ggo::black_32f(); }
    transmission_data                 compute_transmission(const ggo::ray3d_float & ray, const ggo::ray3d_float & normal, int & depth) const override { GGO_FAIL(); return ggo::transmission_type::internal_error; }

    // Light.
    ggo::color_32f                    get_emissive_color() const override { GGO_FAIL(); return ggo::black_32f(); }
    ggo::pos3f                        sample_point(const ggo::pos3f & target_pos, float random_variable1, float random_variable2) const override { GGO_FAIL(); return ggo::pos3f(); }
    ggo::ray3d_float                  sample_ray(float random_variable1, float random_variable2) const override { GGO_FAIL(); return ggo::ray3d_float(); }
  };
}

#endif
