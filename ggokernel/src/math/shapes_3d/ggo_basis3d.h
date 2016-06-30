namespace ggo
{
  template <typename T> class ray3d;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class basis3d
  {
  public:

                              basis3d();

    const ggo::point3d<T> &   pos() const { return _pos; }
    const ggo::vector3d<T> &  x() const { return _x; }
    const ggo::vector3d<T> &  y() const { return _y; }
    const ggo::vector3d<T> &  z() const { return _z; }

    ggo::point3d<T> &         pos() { return _pos; }
      
    void                      reset();
  
    void                      set_pos(T x, T y, T z) { _pos.x() = x; _pos.y() = y; _pos.z() = z; }
    void                      move(const ggo::vector3d<T> & move) { _pos += move; }
    void                      move(T x, T y, T z) { _pos.x() += x; _pos.y() += y; _pos.z() += z; }
  
    void                      rotate_x(T angle);
    void                      rotate_y(T angle);
    void                      rotate_z(T angle);
    void                      rotate(const ggo::vector3d<T> & axis, T angle);
    void                      rotate(const ggo::ray3d<T> & axis, T angle);
    void                      reset_rotation();
    
    void                      look_at(const ggo::point3d<T> & at); // Assuming the basis is looking in the Z negative direction.

    ggo::point3d<T>           point_from_local_to_world(const ggo::point3d<T> & point) const;
    ggo::point3d<T>           point_from_world_to_local(const ggo::point3d<T> & point) const;
      
    ggo::vector3d<T>          vector_from_local_to_world(const ggo::vector3d<T> & vector) const;
    ggo::vector3d<T>          vector_from_world_to_local(const ggo::vector3d<T> & vector) const;

    ggo::ray3d<T>             ray_from_local_to_world(const ggo::ray3d<T> & ray) const;
    ggo::ray3d<T>             ray_from_world_to_local(const ggo::ray3d<T> & ray) const;

    // Project a 3D point on a screen, assuming the basis is the one of a point camera looking in the z-negative direction.
    ggo::pos2<T>              project(const ggo::point3d<T> & p, T aperture, int screen_width, int screen_height) const;

  private:

    ggo::point3d<T>   _pos;
    ggo::vector3d<T>  _x;
    ggo::vector3d<T>  _y;
    ggo::vector3d<T>  _z;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using basis3d_double = basis3d<double>;
  using basis3d_float  = basis3d<float>;
}

//////////////////////////////////////////////////////////////////
// I/O
namespace ggo
{
  template <typename T>
  std::ostream & operator<<(std::ostream & os, const basis3d<T> & param)
  {
    return os << "(" << param.pos() << ", " << param.x() << ", " << param.y() << ", " << param.z() << ")";
  }
}