#ifndef __GGO_LINK__
#define __GGO_LINK__

//////////////////////////////////////////////////////////////
// Class representing a symmetric pair, where both value have
// the same role.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename T>
  class link
  {
  public:

          link() { _v1 = nullptr; _v2 = nullptr; };
          link(const T * v1, const T * v2) { _v1 = v1; _v2 = v2; };

    bool	is_set() const { return _v1 != nullptr && _v2 != nullptr; };
    bool	operator==(const link & l) const;
    bool	operator!=(const link & l) const;

  public:

    const T * _v1;
    const T * _v2;
  };

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool link<T>::operator==(const link<T> & l) const
  {
    if ((_v1 == l._v1) && (_v2 == l._v2)) { return true; }
    if ((_v1 == l._v2) && (_v2 == l._v1)) { return true; }
    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool link<T>::operator!=(const link & l) const
  {
    return !(operator==(l));
  }
}

#endif
