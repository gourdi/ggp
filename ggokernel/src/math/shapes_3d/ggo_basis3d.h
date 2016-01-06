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

    const ggo::set3<T> &  pos() const { return _pos; }
    const ggo::set3<T> &  x() const { return _x; }
    const ggo::set3<T> &  y() const { return _y; }
    const ggo::set3<T> &  z() const { return _z; }

    ggo::set3<T> &        pos() { return _pos; }
      
    void                  reset();
  
    void                  set_pos(T x, T y, T z) { _pos.x() = x; _pos.y() = y; _pos.z() = z; }
    void                  move(const ggo::set3<T> & move) { _pos += move; }
    void                  move(T x, T y, T z) { _pos.x() += x; _pos.y() += y; _pos.z() += z; }
  
    void                  rotate_x(T angle);
    void                  rotate_y(T angle);
    void                  rotate_z(T angle);
    void                  rotate(const ggo::set3<T> & axis, T angle);
    void                  rotate(const ggo::ray3d<T> & axis, T angle);
    void                  reset_rotation();
    
    void                  look_at(const ggo::set3<T> & at); // Assuming the basis is looking in the Z negative direction.

    ggo::set3<T>          point_from_local_to_world(const ggo::set3<T> & point) const;
    ggo::set3<T>          point_from_world_to_local(const ggo::set3<T> & point) const;
      
    ggo::set3<T>          vector_from_local_to_world(const ggo::set3<T> & vector) const;
    ggo::set3<T>          vector_from_world_to_local(const ggo::set3<T> & vector) const;

    ggo::ray3d<T>         ray_from_local_to_world(const ggo::ray3d<T> & ray) const;
    ggo::ray3d<T>         ray_from_world_to_local(const ggo::ray3d<T> & ray) const;

  private:

    ggo::set3<T>  _pos;
    ggo::set3<T>  _x;
    ggo::set3<T>  _y;
    ggo::set3<T>  _z;
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