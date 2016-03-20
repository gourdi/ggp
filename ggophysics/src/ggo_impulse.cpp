#include <ggo_impulse.h>
#include <ggo_physics.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////////
  float compute_impulse(const ggo::oriented_box_body & body,
                        const ggo::vector2d_float & pos,
                        const ggo::vector2d_float & normal)
  {
    ggo::vector2d_float diff = body._box.get_center() - pos;
    float ortho_dot = ggo::ortho_dot(diff, normal);

    float num = -(1 + body._restitution) * (ggo::dot(body._linear_velocity, normal) + body._angular_velocity * ortho_dot);
    float den = 1 / body._mass + ggo::square(ortho_dot) / body.moment_of_intertia();

    return num / den;
  }

  //////////////////////////////////////////////////////////////////
  float compute_impulse(const ggo::oriented_box_body & body1,
                        const ggo::oriented_box_body & body2,
                        const ggo::vector2d_float & pos,
                        const ggo::vector2d_float & normal)
  {
    return 0.f;
  }
}
