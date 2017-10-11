namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, bool double_sided>
  face3d<data_t, double_sided>::face3d(const vertex<data_t> & v1, const vertex<data_t> & v2, const vertex<data_t> & v3)
    :
    _v1(v1), _v2(v2), _v3(v3)
  {
    GGO_ASSERT_FLOAT_EQ(v1._normal.get_length(), 1.f);
    GGO_ASSERT_FLOAT_EQ(v2._normal.get_length(), 1.f);
    GGO_ASSERT_FLOAT_EQ(v3._normal.get_length(), 1.f);

    _m00 = _v2._pos.template get<0>() - _v1._pos.template get<0>();
    _m10 = _v2._pos.template get<1>() - _v1._pos.template get<1>();
    _m20 = _v2._pos.template get<2>() - _v1._pos.template get<2>();

    _m01 = _v3._pos.template get<0>() - _v1._pos.template get<0>();
    _m11 = _v3._pos.template get<1>() - _v1._pos.template get<1>();
    _m21 = _v3._pos.template get<2>() - _v1._pos.template get<2>();

    _m10m21subm11m20 = _m10 * _m21 - _m11 * _m20;
    _m20m01subm00m21 = _m20 * _m01 - _m00 * _m21;
    _m00m11subm10m01 = _m00 * _m11 - _m10 * _m01;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool double_sided>
  bool face3d<data_t, double_sided>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    GGO_ASSERT_FLOAT_EQ(ray.dir().get_length(), 1.f);

    // If P is the intersection point, we have P=v1+s0*(v2-v1)+s1*(v3-v1) since
    // P is inside the face place. We also have P=ray_pos+s2*ray_dir. This leads to:
    // s0*(v2-v1)+s1*(v3-v1)=ray_pos+s2*ray_dir, or
    // s0*(v2-v1)+s1*(v3-v1)-s2*ray_dir=ray_pos,
    // which is a 3 equations with 3 unknowns (s0, s1 and s2) linear system.

    data_t m02 = -ray.dir().template get<0>(); 
    data_t m12 = -ray.dir().template get<1>();
    data_t m22 = -ray.dir().template get<2>();

    data_t det = _m00m11subm10m01 * m22 + _m20m01subm00m21 * m12 + _m10m21subm11m20 * m02;
    if (det == 0)
    {
      return false;
    }

    data_t c0 = ray.pos().template get<0>() - _v1._pos.template get<0>();
    data_t c1 = ray.pos().template get<1>() - _v1._pos.template get<1>();
    data_t c2 = ray.pos().template get<2>() - _v1._pos.template get<2>();

    data_t t1 = c1 *  m22 - c2 *  m12;
    data_t t2 = c1 * _m21 - c2 * _m11;
    data_t t3 = c2 * _m10 - c1 * _m20;

    data_t inv_det = 1 / det;
    data_t s2 = (c0 * _m10m21subm11m20 - _m00 * t2 - _m01 * t3) * inv_det;

    // If s2 is negative, this means that the intersection point is
    // on the wrong side of the ray line.
    if (s2 <= 0)
    {
      return false;
    }

    // We also have to make sure the intersection point is inside the face.
    data_t s0 = (c0 * (_m11 *  m22 - m12 * _m21) - _m01 * t1 + m02 * t2) * inv_det;
    if (s0 < 0)
    {
      return false;
    }

    data_t s1 = (c0 * (m12 * _m20 - _m10 *  m22) + _m00 * t1 + m02 * t3) * inv_det;
    if (s1 < 0 || s0 + s1 > 1)
    {
      return false;
    }

    // The normal.
    dist = s2;
    normal.pos() = ray.pos() + s2 * ray.dir();
    normal.set_dir((1 - s0 - s1) * _v1._normal + s0 * _v2._normal + s1 * _v3._normal);

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
  template<typename data_t, bool double_sided>
  std::ostream & operator<<(std::ostream & os, const face3d<data_t, double_sided> & face)
  {
    os << "(";
    os << "v1=(pos=" << face.v1()._pos << ", normal=" << face.v1()._normal << "), ";
    os << "v2=(pos=" << face.v2()._pos << ", normal=" << face.v2()._normal << "), ";
    os << "v3=(pos=" << face.v3()._pos << ", normal=" << face.v3()._normal << "))";
    return os;
  }
}
