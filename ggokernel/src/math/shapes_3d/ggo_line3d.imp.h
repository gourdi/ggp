namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool find_closest_lines_points(const ggo::set3<data_t> & orig1, const ggo::set3<data_t> & dir1,
                                 const ggo::set3<data_t> & orig2, const ggo::set3<data_t> & dir2,
                                 data_t & dist1, data_t & dist2)
  {
    GGO_ASSERT(dir1.is_normalized() == true);
    GGO_ASSERT(dir2.is_normalized() == true);

    const ggo::vector3d_float diff(orig2 - orig1);

    const float dot = ggo::dot(dir1, dir2);
    const float m[2][2] = { { 1, -dot }, { dot, -1 } };
    const float c[2] = { ggo::dot(dir1, diff), ggo::dot(dir2, diff) };
    float s[2] = { 0.f, 0.f };

    if (ggo::linsolve2d(m, c, s) == false)
    {
      return false;
    }

    dist1 = s[0];
    dist2 = s[1];

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool find_closest_lines_points(const ggo::set3<data_t> & orig1, const ggo::set3<data_t> & dir1,
                                 const ggo::set3<data_t> & orig2, const ggo::set3<data_t> & dir2,
                                 ggo::set3<data_t> & p1, ggo::set3<data_t> & p2)
  {
    data_t dist1, dist2;

    if (find_closest_lines_points(orig1, dir1, orig2, dir2, dist1, dist2) == false)
    {
      return false;
    }

    p1 = orig1 + dist1 * dir1;
    p2 = orig2 + dist2 * dir2;

    return true;
  }
}

