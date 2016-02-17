namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T, bool double_sided>
  face3d<T, double_sided>::face3d(const vertex<T> & v1, const vertex<T> & v2, const vertex<T> & v3)
    :
    _v1(v1), _v2(v2), _v3(v3)
  {
    _m00 = _v2._pos.x() - _v1._pos.x();
    _m10 = _v2._pos.y() - _v1._pos.y();
    _m20 = _v2._pos.z() - _v1._pos.z();

    _m01 = _v3._pos.x() - _v1._pos.x();
    _m11 = _v3._pos.y() - _v1._pos.y();
    _m21 = _v3._pos.z() - _v1._pos.z();

    _m10m21subm11m20 = _m10 * _m21 - _m11 * _m20;
    _m20m01subm00m21 = _m20 * _m01 - _m00 * _m21;
    _m00m11subm10m01 = _m00 * _m11 - _m10 * _m01;
  }

  //////////////////////////////////////////////////////////////
  template <typename T, bool double_sided>
  bool face3d<T, double_sided>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    // If P is the intersection point, we have P=v1+s0*(v2-v1)+s1*(v3-v1) since
    // P is inside the face place. We also have P=ray_pos+s2*ray_dir. This leads to:
    // s0*(v2-v1)+s1*(v3-v1)=ray_pos+s2*ray_dir, or
    // s0*(v2-v1)+s1*(v3-v1)-s2*ray_dir=ray_pos,
    // which is a 3 equations with 3 unknowns (s0, s1 and s2) linear system.

    T m02 = -ray.dir().x();
    T m12 = -ray.dir().y();
    T m22 = -ray.dir().z();

    T det = _m00m11subm10m01 * m22 + _m20m01subm00m21 * m12 + _m10m21subm11m20 * m02;
    if (det == 0)
    {
      return false;
    }

    T c0 = ray.pos().x() - _v1._pos.x();
    T c1 = ray.pos().y() - _v1._pos.y();
    T c2 = ray.pos().z() - _v1._pos.z();

    T t1 = c1 *  m22 - c2 *  m12;
    T t2 = c1 * _m21 - c2 * _m11;
    T t3 = c2 * _m10 - c1 * _m20;

    T inv_det = 1 / det;
    T s2 = (c0 * _m10m21subm11m20 - _m00 * t2 - _m01 * t3) * inv_det;

    // If s2 is negative, this means that the intersection point is
    // on the wrong side of the ray line.
    if (s2 <= 0)
    {
      return false;
    }

    // We also have to make sure the intersection point is inside the face.
    T s0 = (c0 * (_m11 *  m22 - m12 * _m21) - _m01 * t1 + m02 * t2) * inv_det;
    if (s0 < 0)
    {
      return false;
    }

    T s1 = (c0 * (m12 * _m20 - _m10 *  m22) + _m00 * t1 + m02 * t3) * inv_det;
    if (s1 < 0 || s0 + s1 > 1)
    {
      return false;
    }

    // The normal.
    dist = s2;
    normal.pos() = ray.pos() + s2 * ray.dir();
    normal.set_normalized_dir((1 - s0 - s1) * _v1._normal + s0 * _v2._normal + s1 * _v3._normal);

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
