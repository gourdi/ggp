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
    ggo::vector2d_float diff1 = body1._box.get_center() - pos;
    ggo::vector2d_float diff2 = body2._box.get_center() - pos;
    float ortho_dot1 = ggo::ortho_dot(diff1, normal);
    float ortho_dot2 = ggo::ortho_dot(diff2, normal);

    float v1 = ggo::dot(body1._linear_velocity, normal) + body1._angular_velocity * ortho_dot1;
    float v2 = ggo::dot(body2._linear_velocity, normal) + body2._angular_velocity * ortho_dot2;

    float num = -(1 + std::max(body1._restitution, body2._restitution)) * (v1 - v2);
    float den = 1 / body1._mass + 1 / body2._mass + ggo::square(ortho_dot1) / body1.moment_of_intertia() + +ggo::square(ortho_dot2) / body2.moment_of_intertia();

    return num / den;
  }
}
