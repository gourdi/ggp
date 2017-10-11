namespace ggo
{
  template <typename data_t> class ray3d;
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename data_t>
  class basis3d
  {
  public:

                              basis3d();
                              basis3d(const ggo::pos3<data_t> & pos);

    const ggo::pos3<data_t> & pos() const { return _pos; }
    const ggo::vec3<data_t> & x() const { return _x; }
    const ggo::vec3<data_t> & y() const { return _y; }
    const ggo::vec3<data_t> & z() const { return _z; }

    ggo::pos3<data_t> &       pos() { return _pos; }
      
    void                      reset();
  
    void                      set_pos(data_t x, data_t y, data_t z) { _pos.x() = x; _pos.y() = y; _pos.z() = z; }
    void                      move(const ggo::vec3<data_t> & move) { _pos += move; }
    void                      move(data_t x, data_t y, data_t z) { _pos.x() += x; _pos.y() += y; _pos.z() += z; }
  
    void                      rotate_x(data_t angle);
    void                      rotate_y(data_t angle);
    void                      rotate_z(data_t angle);
    void                      rotate(const ggo::vec3<data_t> & axis, data_t angle);
    void                      rotate(const ggo::ray3d<data_t> & axis, data_t angle);
    void                      reset_rotation();
    
    void                      look_at(const ggo::pos3<data_t> & at); // Assuming the basis is looking in the Z negative direction.

    ggo::pos3<data_t>         point_from_local_to_world(const ggo::pos3<data_t> & point) const;
    ggo::pos3<data_t>         point_from_world_to_local(const ggo::pos3<data_t> & point) const;
      
    ggo::vec3<data_t>         vector_from_local_to_world(const ggo::vec3<data_t> & vector) const;
    ggo::vec3<data_t>         vector_from_world_to_local(const ggo::vec3<data_t> & vector) const;

    ggo::ray3d<data_t>        ray_from_local_to_world(const ggo::ray3d<data_t> & ray) const;
    ggo::ray3d<data_t>        ray_from_world_to_local(const ggo::ray3d<data_t> & ray) const;

    // Project a 3D point on a screen, assuming the basis is the one of a point camera looking in the z-negative direction.
    ggo::pos2<data_t>         project(const ggo::pos3<data_t> & p, data_t aperture, int screen_width, int screen_height) const;

  private:

    ggo::pos3<data_t>  _pos;
    ggo::vec3<data_t>  _x;
    ggo::vec3<data_t>  _y;
    ggo::vec3<data_t>  _z;
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
  template <typename data_t>
  std::ostream & operator<<(std::ostream & os, const basis3d<data_t> & param)
  {
    return os << "(" << param.pos() << ", " << param.x() << ", " << param.y() << ", " << param.z() << ")";
  }
}