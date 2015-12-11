namespace ggo
{
  //////////////////////////////////////////////////////////////
  // Solve the equation (x-center_x)^2+(y-center_y)^2+(z-center_z)^2=r^2
  // injecting the ray into the equation.
  template <typename T>
  bool sphere3d<T>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    // Build the quadratic and solve it.
    const ggo::set3<T> & dir = ray.dir();
    const ggo::set3<T> & pos = ray.pos();
    const ggo::set3<T> diff(pos - _center);
    
    // Hint 1: deg2 is dot(dir, dir) which is 1
    // Hint 2: actually, deg1 should be 2*dot(dir,diff), but removing 
    // the multiplication simplifies the computations
    T deg1 = ggo::dot(dir, diff); 
    T deg0 = ggo::dot(diff, diff) - _radius * _radius;
    
    // Solve it.
    T d = deg1 * deg1 - deg0;
    if (d < 0)
    {
      return false;
    }
    d = sqrt(d);

    // Check the lower solution.
    dist = -deg1 - d;
    if (dist >= 0)
    {
      normal.pos() = pos + dist * dir;
      normal.set_dir(normal.pos() - _center);
      return true;
    }

    // Check the greatest solution (the ray's origin is then inside the sphere, which is a valid case).
    // In that case, the normal points towards the center of the sphere.
    dist = -deg1 + d;
    if (dist >= 0)
    {
      normal.pos() = pos + dist * dir;
      normal.set_dir(_center - normal.pos());
      return true;
    }
    
    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool sphere3d<T>::intersect_ray(const ggo::ray3d<T> & ray) const
  {
    const ggo::set3<T> & dir = ray.dir();
    const ggo::set3<T> & pos = ray.pos();
    
    T dx = pos.x() - _center.x();
    T dy = pos.y() - _center.y();
    T dz = pos.z() - _center.z();
    
    T deg2 = dir.x()*dir.x() + dir.y()*dir.y() + dir.z()*dir.z();
    T deg1 = 2*(dir.x()*dx + dir.y()*dy + dir.z()*dz);
    T deg0 = dx*dx + dy*dy + dz*dz - _radius*_radius;
    GGO_ASSERT(deg2 >= 0);

    T d = deg1 * deg1 - T(4) * deg2 * deg0;
    
    return (d >= 0) && (-std::sqrt(d) > deg1);
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  sphere3d<T> sphere3d<T>::merge(const sphere3d<T> & sphere1, const sphere3d<T> & sphere2)
  {
    T hypot = ggo::hypot(sphere1.center(), sphere2.center());
    T squared_radius1 = ggo::square(sphere1.radius());
    T squared_radius2 = ggo::square(sphere2.radius());
      
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
      
    ggo::set3<T> diff(sphere2.center() - sphere1.center());
    diff /= diff.get_length(); // Normalize.
      
    ggo::set3<T> p1(sphere1.center() - sphere1.radius() * diff);
    ggo::set3<T> p2(sphere2.center() + sphere2.radius() * diff);
    ggo::set3<T> diff2(p2 - p1);
      
    return sphere3d((p1 + p2) / T(2), diff2.get_length() / T(2));
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  std::string sphere3d<T>::desc() const
  {
    std::ostringstream oss;
    oss << "sphere3d " << "(" << _center << ", " << _radius <<")";
    return oss.str();
  }
}

