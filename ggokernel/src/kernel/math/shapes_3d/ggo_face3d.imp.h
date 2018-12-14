namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, bool double_sided>
  face3d<data_t, double_sided>::face3d(const vertex<data_t> & v1, const vertex<data_t> & v2, const vertex<data_t> & v3)
    :
    _v1(v1), _v2(v2), _v3(v3)
  {
    GGO_ASSERT_FLOAT_EQ(ggo::length(v1._normal), 1.f);
    GGO_ASSERT_FLOAT_EQ(ggo::length(v2._normal), 1.f);
    GGO_ASSERT_FLOAT_EQ(ggo::length(v3._normal), 1.f);

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
  template <typename data_t, bool double_sided>
  bool face3d<data_t, double_sided>::solve_intersection(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t & s0, data_t & s1, data_t & s2) const
  {
    GGO_ASSERT(ggo::is_normalized(dir));

    // If P is the intersection point, we have P=v1+s0*(v2-v1)+s1*(v3-v1) since
    // P is inside the face place. We also have P=ray_pos+s2*ray_dir. This leads to:
    // s0*(v2-v1)+s1*(v3-v1)=ray_pos+s2*ray_dir, or
    // s0*(v2-v1)+s1*(v3-v1)-s2*ray_dir=ray_pos,
    // which is a 3 equations with 3 unknowns (s0, s1 and s2) linear system.

    data_t m02 = -dir.x();
    data_t m12 = -dir.y();
    data_t m22 = -dir.z();

    data_t det = _m00m11subm10m01 * m22 + _m20m01subm00m21 * m12 + _m10m21subm11m20 * m02;
    if (det == 0)
    {
      return false;
    }

    data_t c0 = pos.x() - _v1._pos.x();
    data_t c1 = pos.y() - _v1._pos.y();
    data_t c2 = pos.z() - _v1._pos.z();

    data_t t1 = c1 * m22 - c2 * m12;
    data_t t2 = c1 * _m21 - c2 * _m11;
    data_t t3 = c2 * _m10 - c1 * _m20;

    data_t inv_det = 1 / det;
    
    s2 = (c0 * _m10m21subm11m20 - _m00 * t2 - _m01 * t3) * inv_det;

    // If s2 is negative, this means that the intersection point is
    // on the wrong side of the ray line.
    if (s2 <= 0)
    {
      return false;
    }

    // We also have to make sure the intersection point is inside the face.
    s0 = (c0 * (_m11 *  m22 - m12 * _m21) - _m01 * t1 + m02 * t2) * inv_det;
    if (s0 < 0)
    {
      return false;
    }

    s1 = (c0 * (m12 * _m20 - _m10 * m22) + _m00 * t1 + m02 * t3) * inv_det;
    if (s1 < 0 || s0 + s1 > 1)
    {
      return false;
    }

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool double_sided>
  bool face3d<data_t, double_sided>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    data_t s0, s1;
    if (solve_intersection(ray.pos(), ray.dir(), s0, s1, dist) == false)
    {
      return false;
    }

    // The normal.
    normal.pos() = ray.pos() + dist * ray.dir();
    normal.set_dir((1 - s0 - s1) * _v1._normal + s0 * _v2._normal + s1 * _v3._normal);

    if (ggo::dot(normal.dir(), ray.dir()) > 0)
    {
      if constexpr(double_sided == false)
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
  // The segment does not have a direction, unlike a ray. So double-sideness is ignored.
  template <typename data_t, bool double_sided>
  bool face3d<data_t, double_sided>::intersect_segment(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & dir, data_t length) const
  {
    data_t s0, s1, dist;
    if (solve_intersection(pos, dir, s0, s1, dist) == false)
    {
      return false;
    }

    return dist >= 0 && dist < length;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, bool double_sided>
  std::optional<box3d_data<data_t>> face3d<data_t, double_sided>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
  {
    return box3d_data<data_t>::from({
      basis.point_from_local_to_world(_v1._pos),
      basis.point_from_local_to_world(_v2._pos),
      basis.point_from_local_to_world(_v3._pos)
    });
  }
}

namespace ggo
{
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
