namespace ggo
{
  //////////////////////////////////////////////////////////////
  // Solve the equation (x-center_x)^2+(y-center_y)^2+(z-center_z)^2=r^2
  // injecting the ray into the equation.
  // Returns the (possibly negative) distance of intersections to the line's origin.
  template <typename data_t>
  bool sphere3d<data_t>::intersect_line(const ggo::line3d<data_t> & line, data_t & dist_inf, data_t & dist_sup) const
  {
    // Build the quadratic and solve it.
    const ggo::vec3<data_t> & dir = line.dir();
    const ggo::pos3<data_t> & pos = line.pos();
    const ggo::vec3<data_t> diff(pos - _center);

    // Hint 1: deg2 is dot(dir, dir) which is 1
    // Hint 2: actually, deg1 should be 2*dot(dir,diff), but removing 
    // the multiplication simplifies the computations
    data_t deg1 = ggo::dot(dir, diff);
    data_t deg0 = ggo::dot(diff, diff) - _radius * _radius;

    // Solve it.
    data_t d = deg1 * deg1 - deg0;
    if (d < 0)
    {
      return false;
    }
    d = sqrt(d);

    dist_inf = -deg1 - d;
    dist_sup = -deg1 + d;

    return true;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool sphere3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray, data_t & dist, ggo::ray3d<data_t> & normal) const
  {
    data_t dist_inf, dist_sup;
    if (intersect_line(ggo::line3d<data_t>(ray), dist_inf, dist_sup) == false)
    {
      return false;
    }

    // Check the lower solution.
    if (dist_inf >= 0)
    {
      dist = dist_inf;
      normal.pos() = ray.pos() + dist * ray.dir();
      normal.set_dir(normal.pos() - _center);
      return true;
    }

    // Check the greatest solution (the ray's origin is then inside the sphere, which is a valid case).
    // In that case, the normal points towards the center of the sphere.
    if (dist_sup >= 0)
    {
      dist = dist_sup;
      normal.pos() = ray.pos() + dist * ray.dir();
      normal.set_dir(_center - normal.pos());
      return true;
    }

    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool sphere3d<data_t>::intersect_ray_fast(const ggo::ray3d<data_t> & ray) const
  {
    data_t dist_inf, dist_sup;
    if (intersect_line(ggo::line3d<data_t>(ray), dist_inf, dist_sup) == false)
    {
      return false;
    }

    return dist_sup >= 0;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::vector<data_t> sphere3d<data_t>::intersect_ray(const ggo::ray3d<data_t> & ray) const
  {
    std::vector<data_t> intersections;

    data_t dist_inf = 0;
    data_t dist_sup = 0;
    if (intersect_line(ggo::line3d<data_t>(ray), dist_inf, dist_sup) == true && dist_inf > 0)
    {
      intersections.push_back(dist_inf);
      intersections.push_back(dist_sup);
    }

    return intersections;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool sphere3d<data_t>::is_point_inside(data_t x, data_t y, data_t z) const
  {
    ggo::vec3<data_t> diff(x - _center.x(), y - _center.y(), z - _center.z());

    return diff.get_hypot() <= ggo::square(_radius);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool sphere3d<data_t>::is_point_inside(const ggo::pos3<data_t> & p) const
  {
    return is_point_inside(p.x(), p.y(), p.z());
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::optional<box3d_data<data_t>> sphere3d<data_t>::get_bounding_box(const ggo::basis3d<data_t> & basis) const
  {
    const ggo::vec3<data_t> x(data_t(1), data_t(0), data_t(0));
    const ggo::vec3<data_t> y(data_t(0), data_t(1), data_t(0));
    const ggo::vec3<data_t> z(data_t(0), data_t(0), data_t(1));

    ggo::vec3<data_t> world_center = basis.point_from_local_to_world(_center);

    return box3d_data<data_t>::from({
      world_center - _radius * x, world_center + _radius * x,
      world_center - _radius * y, world_center + _radius * y,
      world_center - _radius * z, world_center + _radius * z });
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  sphere3d<data_t> get_union(const sphere3d<data_t> & sphere1, const sphere3d<data_t> & sphere2)
  {
    data_t hypot = ggo::hypot(sphere1.center(), sphere2.center());
    data_t squared_radius1 = ggo::square(sphere1.radius());
    data_t squared_radius2 = ggo::square(sphere2.radius());

    // Sphere1 inside sphere2?
    if (hypot + squared_radius1 < squared_radius2)
    {
      return sphere2;
    }

    // Sphere2 inside sphere1?
    if (hypot + squared_radius2 < squared_radius1)
    {
      return sphere1;
    }

    ggo::vec3<data_t> diff(sphere2.center() - sphere1.center());
    diff /= diff.get_length(); // Normalize.

    ggo::pos3<data_t> p1(sphere1.center() - sphere1.radius() * diff);
    ggo::pos3<data_t> p2(sphere2.center() + sphere2.radius() * diff);
    ggo::vec3<data_t> diff2(p2 - p1);

    return sphere3d<data_t>((p1 + p2) / data_t(2), diff2.get_length() / data_t(2));
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const ggo::sphere3d<data_t> & sphere)
  {
    os << "(center=" << sphere.center() << ", radius=" << sphere.radius() <<")";
    return os;
  }
}
