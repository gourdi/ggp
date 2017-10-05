namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T, bool double_sided>
  bool triangle3d<T, double_sided>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    T	m[3][3], c[3], s[3];

    // If P is the intersection point, we have P=v1+k1*(v2-v1)+k2*(v3*-v1) since
    // P is inside the face place. We also have P=ray_pos+k3*ray_dir. This leads to:
    // k1*(v2-v1)+k2*(v3*-v1)=ray_pos+k3*ray_dir, or
    // k1*(v2-v1)+k2*(v3*-v1)-k3*ray_dir=ray_pos,
    // which is a 3 equations with 3 unknowns (k1, k2 and k3) linear system.

    m[0][0] = _v2.x() - _v1.x();
    m[1][0] = _v2.y() - _v1.y();
    m[2][0] = _v2.z() - _v1.z();

    m[0][1] = _v3.x() - _v1.x();
    m[1][1] = _v3.y() - _v1.y();
    m[2][1] = _v3.z() - _v1.z();

    m[0][2] = -ray.dir().x();
    m[1][2] = -ray.dir().y();
    m[2][2] = -ray.dir().z();

    c[0] = ray.pos().x() - _v1.x();
    c[1] = ray.pos().y() - _v1.y();
    c[2] = ray.pos().z() - _v1.z();

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
    if ((s[0] < 0) || (s[1] < 0) || (s[0]+s[1] > 1))
    {
      return false;
    }

    // The normal.
    dist = s[2];
    normal.pos() = ray.pos() + s[2] * ray.dir();
    normal.set_normalized_dir(get_normal());

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
  template <typename T, bool double_sided>
  void triangle3d<T, double_sided>::get_bounding_sphere(ggo::sphere3d<T> & bounding_sphere) const
  {
    ggo::vec3<T> v12(_v2 - _v1);
    ggo::vec3<T> v13(_v3 - _v1);
    T dot1212 = ggo::dot(v12, v12);
    T dot1313 = ggo::dot(v13, v13);
    T dot1213 = ggo::dot(v12, v13);

    T d = 2 * (dot1212 * dot1313 - dot1213 * dot1213);

    if (std::fabs(d) <= T(0.0001))
    {
      // The 3 vertices of the triangle are in fact aligned.
      T min_x = std::min(_v1.x(), std::min(_v2.x(), _v3.x()));
      T min_y = std::min(_v1.y(), std::min(_v2.y(), _v3.y()));
      T min_z = std::min(_v1.z(), std::min(_v2.z(), _v3.z()));

      T max_x = std::max(_v1.x(), std::max(_v2.x(), _v3.x()));
      T max_y = std::max(_v1.y(), std::max(_v2.y(), _v3.y()));
      T max_z = std::max(_v1.z(), std::max(_v2.z(), _v3.z()));

      ggo::pos3<T> v_min(min_x, min_y, min_z);
      ggo::pos3<T> v_max(max_x, max_y, max_z);

      bounding_sphere.center() = T(0.5) * (v_min + v_max);
      bounding_sphere.radius() = T(0.5) * ggo_distance(v_min, v_max);
    }
    else
    {
      // s controls height over 13, t over 12, (1-s-t) over 23.
      T s = (dot1212 * dot1313 - dot1313 * dot1213) / d;
      T t = (dot1313 * dot1212 - dot1212 * dot1213) / d;

      if (s <= 0)
      {
        bounding_sphere.center() = T(0.5) * (_v1 + _v3);
        bounding_sphere.radius() = T(0.5) * std::sqrt(dot1313);
      }
      else if (t <= 0)
      {
        bounding_sphere.center() = T(0.5) * (_v1 + _v2);
        bounding_sphere.radius() = T(0.5) * std::sqrt(dot1212);
      }
      else if (s + t >= 1)
      {
        bounding_sphere.center() = T(0.5) * (_v2 + _v3);
        bounding_sphere.radius() = T(0.5) * ggo_distance(_v2, _v3);
      }
      else
      {
        // The circle's center lies inside the rectangle
        // and is equidistant to all points.
        bounding_sphere.center() = _v1 + s * v12 + t * v13;
        bounding_sphere.radius() = ggo_distance(bounding_sphere.center(), _v1);
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename T, bool double_sided>
  ggo::vec3<T> triangle3d<T, double_sided>::get_normal() const
  {
    ggo::vec3<T>	edge1(_v2 - _v1);
    ggo::vec3<T>	edge2(_v3 - _v1);
    ggo::vec3<T>	normal(ggo::cross(edge1, edge2));

    normal.normalize();

    return normal;
  }

  //////////////////////////////////////////////////////////////
  template <typename T, bool double_sided>
  ggo::pos3<T> triangle3d<T, double_sided>::get_center() const
  {
    T x = _v1.x() + _v2.x() + _v3.x();
    T y = _v1.y() + _v2.y() + _v3.y();
    T z = _v1.z() + _v2.z() + _v3.z();

    return ggo::pos3<T>(x/3, y/3, z/3);
  }

  //////////////////////////////////////////////////////////////
  template <typename T, bool double_sided>
  T triangle3d<T, double_sided>::area() const
  {
    ggo::vec3<T> cross = ggo::cross(ggo::vec3<T>(_v3 - _v1), ggo::vec3<T>(_v2 - _v1));

    return cross.get_length() / 2;
  }
  
  //////////////////////////////////////////////////////////////
  template <typename data_t, bool double_sided>
  std::ostream & operator<<(std::ostream & os, const triangle3d<data_t, double_sided> & t)
  {
    os << "(v1=" << t.v1() << ", v2=" << t.v2() << ", v3=" << t.v3() << ")";
	  return os;
  }
}
