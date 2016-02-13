#include "ggo_phong_shader.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::color phong_shader::diffuse_shading(const ggo::color & object_color, const ggo::color & light_color, const ggo::ray3d_float & ray, const ggo::ray3d_float & world_normal, const ggo::ray3d_float & ray_to_light) const
  {
    GGO_ASSERT_BTW(world_normal.pos().x() - ray_to_light.pos().x(), -0.01, 0.01);
    GGO_ASSERT_BTW(world_normal.pos().y() - ray_to_light.pos().y(), -0.01, 0.01);
    GGO_ASSERT_BTW(world_normal.pos().z() - ray_to_light.pos().z(), -0.01, 0.01);
    GGO_ASSERT_GE(ggo::dot(world_normal.dir(), ray_to_light.dir()), -0.01);

    float intensity = ggo::dot(world_normal.dir(), ray_to_light.dir());

    ggo::color output_color(object_color * light_color * intensity);

    if (get_phong_factor() > 0)
    {
      ggo::vector3d_float reflexion_dir(2 * ggo::dot(world_normal.dir(), ray_to_light.dir()) * world_normal.dir() - ray_to_light.dir());
      GGO_ASSERT(reflexion_dir.is_normalized(0.0001));

      ggo::vector3d_float viewer_dir(ray.pos() - world_normal.pos());
      viewer_dir.normalize();

      float phong = ggo::dot(reflexion_dir, viewer_dir);
      if (phong > 0)
      {
        float specular = _phong_factor * powf(phong, _phong_shininess);
        output_color += light_color * specular;
      }
    }

    return output_color;
  }
}
