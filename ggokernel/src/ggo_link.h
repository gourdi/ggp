#ifndef __GGO_LINK__
#define __GGO_LINK__

#include <algorithm>

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

          link(T v1, T v2) { _v1 = v1; _v2 = v2; };

    bool	operator==(const link & rhs) const;
    bool	operator!=(const link & rhs) const;

  public:

    T _v1;
    T _v2;
  };

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool link<T>::operator==(const link<T> & rhs) const
  {
    if (this->_v1 == rhs._v1 && this->_v2 == rhs._v2) { return true; }
    if (this->_v1 == rhs._v2 && this->_v2 == rhs._v1) { return true; }
    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename T>
  bool link<T>::operator!=(const link<T> & rhs) const
  {
    return !(operator==(rhs));
  }
}

namespace ggo
{
  template <typename IT, typename T, typename CONTAINER = std::vector<T>>
  CONTAINER find_linked(IT first, IT end, T v)
  {
    CONTAINER result;
    for (; first != end; ++first)
    {
      if (first->_v1 == v)
      {
        result.push_back(first->_v2);
      }
      if (first->_v2 == v)
      {
        result.push_back(first->_v1);
      }
    }

    return result;
  }

  template <typename INPUT_CONTAINER, typename T, typename OUTPUT_CONTAINER = std::vector<T>>
  OUTPUT_CONTAINER find_linked(INPUT_CONTAINER container, T v)
  {
    return find_linked<INPUT_CONTAINER::const_iterator, T, OUTPUT_CONTAINER>(container.begin(), container.end(), v);
  }
}

#endif
