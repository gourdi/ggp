#ifndef __GGO_DIFFUSE_OBJECT3D__
#define __GGO_DIFFUSE_OBJECT3D__

#include <ggo_reflection_object3d_abc.h>

namespace ggo
{
  template <uint32_t flags, typename shape_t, typename material_t>
  class diffuse_object3d : public reflection_object3d_abc<flags, shape_t>
  {
  public:

    diffuse_object3d(const shape_t & shape, const material_t & material) : reflection_object3d_abc<flags, shape_t>(shape), _material(material) {}

  private:

    ggo::color_32f  get_emissive_color() const override { return ggo::black_32f(); }
    ggo::color_32f  process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const override;

    ggo::color_32f  compute_diffuse_color(const std::vector<ggo::light_sample> & light_samples, const intersection_data & intersection) const;

  private:

    material_t  _material;
  };
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t, typename material_t>
  ggo::color_32f diffuse_object3d<flags, shape_t, material_t>::compute_diffuse_color(const std::vector<ggo::light_sample> & light_samples, const intersection_data & intersection) const
  {
    const ggo::color_32f diffuse_color(_material.get_color(intersection._local_normal.pos()));

    ggo::color_32f output_color = ggo::black_32f();

    for (const auto & light_sample : light_samples)
    {
      GGO_ASSERT_BTW(intersection._world_normal.pos().get<0>() - light_sample._ray_to_light.pos().get<0>(), -0.01, 0.01);
      GGO_ASSERT_BTW(intersection._world_normal.pos().get<1>() - light_sample._ray_to_light.pos().get<1>(), -0.01, 0.01);
      GGO_ASSERT_BTW(intersection._world_normal.pos().get<2>() - light_sample._ray_to_light.pos().get<2>(), -0.01, 0.01);
      GGO_ASSERT_GE(ggo::dot(world_normal.dir(), light_sample._ray_to_light.dir()), -0.01);

      output_color += ggo::dot(intersection._world_normal.dir(), light_sample._ray_to_light.dir()) * diffuse_color * light_sample._color;
    }

    return output_color;
  }

  //////////////////////////////////////////////////////////////
  template <uint32_t flags, typename shape_t, typename material_t>
  ggo::color_32f diffuse_object3d<flags, shape_t, material_t>::process_ray(const ggo::ray3d_float & ray, const intersection_data & intersection, const ggo::raytracer & raytracer, int depth, float random_variable1, float random_variable2) const
  {
    auto light_samples = raytracer.filter_lights(intersection._world_normal, this, random_variable1, random_variable2);

    ggo::color_32f output_color = compute_diffuse_color(light_samples, intersection);

    if constexpr(!(flags & discard_reflection))
    {
      output_color += compute_reflection_color(ray, intersection._world_normal, raytracer, depth, random_variable1, random_variable2);
    }

    if constexpr(!(flags & discard_phong))
    {
      output_color += compute_phong_color(ray, light_samples, intersection);
    }

    return output_color;
  }
}
#endif
