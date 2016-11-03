namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  parallelogram3d<T>::parallelogram3d(const ggo::pos3<T> & pos, const ggo::vec3<T> & v1, const ggo::vec3<T> & v2)
  :
  _pos(pos), _v1(v1), _v2(v2), _normal(ggo::cross(v1, v2))
  {
    _normal.normalize();
  }
    
  //////////////////////////////////////////////////////////////
  template <typename T>
  bool parallelogram3d<T>::intersect_ray(const ggo::ray3d<T> & ray, T & dist, ggo::ray3d<T> & normal) const
  {
    const T m[3][3] = {{ray.dir().x(), -_v1.x(), -_v2.x()},
                       {ray.dir().y(), -_v1.y(), -_v2.y()},
                       {ray.dir().z(), -_v1.z(), -_v2.z()}};
    const T c[3] = {_pos.x() - ray.pos().x(),
                    _pos.y() - ray.pos().y(),
                    _pos.z() - ray.pos().z()};         
    T s[3];
    
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
  template <typename T>
  std::ostream & parallelogram3d<T>::operator<<(std::ostream & os) const
  {
    os << "parallelogram3d (" << _pos << ", " << _v1 << ", " << _v2 << ")";
    return os;
  }
}
