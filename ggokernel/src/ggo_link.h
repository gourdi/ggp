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
  template <typename it, typename data_type, typename container = std::vector<data_type>>
  container find_linked(it first, it end, data_type v)
  {
    container result;
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

  template <typename input_container, typename data_type, typename output_container = std::vector<data_type>>
  output_container find_linked(const input_container & c, data_type v)
  {
    return find_linked<input_container::const_iterator, data_type, output_container>(c.begin(), c.end(), v);
  }
}

#endif
