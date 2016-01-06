namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T, bool double_sided>
  bool face3d<T, double_sided>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    T	m[3][3], c[3], s[3];

    // If P is the intersection point, we have P=v1+k1*(v2-v1)+k2*(v3*-v1) since
    // P is inside the face place. We also have P=ray_pos+k3*ray_dir. This leads to:
    // k1*(v2-v1)+k2*(v3*-v1)=ray_pos+k3*ray_dir, or
    // k1*(v2-v1)+k2*(v3*-v1)-k3*ray_dir=ray_pos,
    // which is a 3 equations with 3 unknowns (k1, k2 and k3) linear system.

    m[0][0] = _v2._pos.x() - _v1._pos.x();
    m[1][0] = _v2._pos.y() - _v1._pos.y();
    m[2][0] = _v2._pos.z() - _v1._pos.z();

    m[0][1] = _v3._pos.x() - _v1._pos.x();
    m[1][1] = _v3._pos.y() - _v1._pos.y();
    m[2][1] = _v3._pos.z() - _v1._pos.z();

    m[0][2] = -ray.dir().x();
    m[1][2] = -ray.dir().y();
    m[2][2] = -ray.dir().z();

    c[0] = ray.pos().x() - _v1._pos.x();
    c[1] = ray.pos().y() - _v1._pos.y();
    c[2] = ray.pos().z() - _v1._pos.z();

    // Solve the linear system. If it is not possible, it is because
    // the ray is parallel to the face place.
    if (ggo::linsolve3d(m, c, s) == false)
    {
      return false;
    }

    // If k3 is negative, this means that the intersection point is
    // on the wrong side of the ray line.
    if (s[2] <= 0)
    {
      return false;
    }

    // We also have to make sure the intersection point is inside the face.
    if ((s[0] < 0) || (s[1] < 0) || (s[0] + s[1] > 1))
    {
      return false;
    }

    // The normal.
    dist = s[2];
    normal.pos() = ray.pos() + s[2] * ray.dir();
    normal.set_normalized_dir((1 - s[0] - s[1]) * _v1._normal + s[0] * _v2._normal + s[1] * _v3._normal);

    if (ggo::dot(normal.dir(), ray.dir()) > 0)
    {
      if (double_sided == false)
      {
        return false;
      }
      else
      {
        normal.flip();
      }
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  template<typename T, bool double_sided>
  std::string face3d<T, double_sided>::desc() const
  {
    std::ostringstream oss;
    oss << "(" << _v1._pos << ", " << _v1._normal << "), ";
    oss << "(" << _v2._pos << ", " << _v2._normal << "), ";
    oss << "(" << _v3._pos << ", " << _v3._normal << ")";
    return oss.str();
  }
}
