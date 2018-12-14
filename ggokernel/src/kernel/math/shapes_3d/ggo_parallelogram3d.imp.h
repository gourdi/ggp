namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  parallelogram3d<data_t>::parallelogram3d(const ggo::pos3<data_t> & pos, const ggo::vec3<data_t> & v1, const ggo::vec3<data_t> & v2)
    :
    _pos(pos), _v1(v1), _v2(v2), _normal(normalize(cross(v1, v2)))
  {
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool parallelogram3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    const data_t m[3][3] = {
      { ray.dir().x(), -_v1.x(), -_v2.x() },
      { ray.dir().y(), -_v1.y(), -_v2.y() },
      { ray.dir().z(), -_v1.z(), -_v2.z() } };
    const data_t c[3] = {
      _pos.x() - ray.pos().x(),
      _pos.y() - ray.pos().y(),
      _pos.z() - ray.pos().z() };
    data_t s[3];

    if (ggo::linsolve3d(m, c, s) == false)
    {
      return false;
    }

    dist = s[0];
    if (dist < 0)
    {
      return false;
    }

    if ((s[1] > 1) || (s[1] < -1) || (s[2] > 1) || (s[2] < -1))
    {
      return false;
    }

    normal.pos() = _pos + s[1] * _v1 + s[2] * _v2;
    normal.set_normalized_dir(_normal);

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::optional<box3d_data<data_t>> parallelogram3d<data_t>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
  {
    const ggo::pos3<data_t> p1 = _pos + _v1;
    const ggo::pos3<data_t> p2 = _pos - _v1;
    const ggo::pos3<data_t> p3 = _pos + _v2;
    const ggo::pos3<data_t> p4 = _pos - _v2;

    return box3d_data<data_t>::from({
      basis.point_from_local_to_world(p1),
      basis.point_from_local_to_world(p2),
      basis.point_from_local_to_world(p3),
      basis.point_from_local_to_world(p4) });
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream &  operator<<(std::ostream & os, const parallelogram3d<data_t> & parallelogram)
  {
    os << "(pos=" << parallelogram.pos() << ", v1=" << parallelogram.v1() << ", v2=" << parallelogram.v2() << ")";
    return os;
  }
}
