//////////////////////////////////////////////////////////////
namespace ggo
{
  template <typename T>
  class ray3d
  {
  public:

                          ray3d() {};
                          ray3d(T pos_x, T pos_y, T pos_z, T dir_x, T dir_y, T dir_z, bool normalize_dir = true);
                          ray3d(const ggo::pos3<T> & pos, const ggo::vec3<T> & dir, bool normalize_dir = true);

    const ggo::pos3<T> &	pos() const { return _pos; }
    ggo::pos3<T> &		    pos() { return _pos; }
    void				          set_pos(T x, T y, T z) { _pos = ggo::pos3<T>(x, y, z); }

    const ggo::vec3<T> &	dir() const { return _dir; };
    void				          set_dir(const ggo::vec3<T> & dir) { _dir = dir; _dir.normalize(); }
    void				          set_dir(T x, T y, T z) { _dir = ggo::vec3<T>(x, y, z); _dir.normalize(); }
    void				          set_normalized_dir(const ggo::vec3<T> & dir) { _dir = dir; GGO_ASSERT(_dir.is_normalized(T(0.001))); }
    void				          set_normalized_dir(T x, T y, T z) { _dir = ggo::vec3<T>(x, y, z); GGO_ASSERT(_dir.is_normalized(T(0.001))); }

    bool                  is_normalized(T epsilon = 0.0001) const { return _dir.is_normalized(epsilon); }
    
    void                  flip();
    
  public:
    
    static	const ray3d<T> &	O_X() { static const ray3d<T> ray(ggo::pos3<T>(0.f, 0.f, 0.f), ggo::vec3<T>(1.f, 0.f, 0.f)); return ray; };
    static	const ray3d<T> &	O_Y() { static const ray3d<T> ray(ggo::pos3<T>(0.f, 0.f, 0.f), ggo::vec3<T>(0.f, 1.f, 0.f)); return ray; };
    static	const ray3d<T> &	O_Z() { static const ray3d<T> ray(ggo::pos3<T>(0.f, 0.f, 0.f), ggo::vec3<T>(0.f, 0.f, 1.f)); return ray; };

  private:

    ggo::pos3<T>	_pos;
    ggo::vec3<T>	_dir;
  };
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  using ray3d_float   = ray3d<float>;
  using ray3d_double  = ray3d<double>;
}
//////////////////////////////////////////////////////////////////
// I/O
namespace ggo
{
  template <typename T>
  std::ostream & operator<<(std::ostream &os, const ray3d<T> & ray)
  {
    os << "(" << ray.pos() << ", " << ray.dir() << ")";
    return os;
  }
}

//////////////////////////////////////////////////////////////
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>  
  ray3d<T>::ray3d(T pos_x, T pos_y, T pos_z, T dir_x, T dir_y, T dir_z, bool normalize_dir)
  :
  _pos(pos_x, pos_y, pos_z),
  _dir(dir_x, dir_y, dir_z)
  {
    if (normalize_dir == true)
    {
      _dir.normalize();
    }
    else
    {
      GGO_ASSERT(_dir.is_normalized(T(0.001)) == true);
    }  
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  ray3d<T>::ray3d(const ggo::pos3<T> & pos, const ggo::vec3<T> & dir, bool normalize_dir)
  :
  _pos(pos),
  _dir(dir)
  {	
    if (normalize_dir == true)
    {
      _dir.normalize();
    }
    else
    {
      GGO_ASSERT(_dir.is_normalized(T(0.001)) == true);
    }
  }
  
  //////////////////////////////////////////////////////////////
  template <typename T>
  void ray3d<T>::flip()
  {
    _dir.flip();
  }
}

