//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class axis_aligned_box3d : public raytracable_shape3d_abc<T>
  {
  public:

                  axis_aligned_box3d() {};
                  axis_aligned_box3d(T x_min, T x_max, T y_min, T y_max, T z_min, T z_max);
  
    T			        x_min() const { return _x_min; }
    T			        x_max() const { return _x_max; }
    T			        y_min() const { return _y_min; }
    T			        y_max() const { return _y_max; }
    T			        z_min() const { return _z_min; }
    T			        z_max() const { return _z_max; }
  
    T &	          x_min() { return _x_min; }
    T &	          x_max() { return _x_max; }
    T &	          y_min() { return _y_min; }
    T &	          y_max() { return _y_max; }
    T &	          z_min() { return _z_min; }
    T &	          z_max() { return _z_max; }
  
    bool          is_point_inside(const ggo::set3<T> & point) const;
  
    bool          intersect_ray(const ggo::ray3d<T> & ray) const; // Fast intersection test.
    bool          intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const override;
    bool          is_convex() const override { return true; }
  
    void          get_bounding_sphere(ggo::sphere3d<T> & bounding_sphere) const;

    ggo::set3<T>	get_center() const;
    T			        get_size_x() const { return _x_max - _x_min; }
    T			        get_size_y() const { return _y_max - _y_min; }
    T			        get_size_z() const { return _z_max - _z_min; }
  
    void          merge_with(const axis_aligned_box3d & box);
    void          merge_with(const ggo::set3<T> & point);
    
    std::string   desc() const override;

  private:

    T	_x_min;
    T	_x_max;
    T	_y_min;
    T	_y_max;
    T	_z_min;
    T	_z_max;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using aabox3d_float   = axis_aligned_box3d<float>;
  using aabox3d_double  = axis_aligned_box3d<double>;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  std::ostream& operator<<(std::ostream & os, const axis_aligned_box3d<T> & box)
  {
    os << "x_min=" << box.x_min();
    os << ", x_max=" << box.x_max();
    os << ", y_min=" << box.y_min();
    os << ", y_max=" << box.y_max();
    os << ", z_min=" << box.z_min();
    os << ", z_max=" << box.z_max();
    return os;
  }
}

