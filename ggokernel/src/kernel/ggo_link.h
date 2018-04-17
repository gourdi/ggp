#ifndef __GGO_LINK__
#define __GGO_LINK__

#include <algorithm>

//////////////////////////////////////////////////////////////
// Class representing a symmetric pair, where both value have
// the same role.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  class link
  {
  public:

          link(data_t v1, data_t v2) { _v1 = v1; _v2 = v2; };

    bool	operator==(const link & rhs) const;
    bool	operator!=(const link & rhs) const;

  public:

    data_t _v1;
    data_t _v2;
  };

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool link<data_t>::operator==(const link<data_t> & rhs) const
  {
    if (this->_v1 == rhs._v1 && this->_v2 == rhs._v2) { return true; }
    if (this->_v1 == rhs._v2 && this->_v2 == rhs._v1) { return true; }
    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool link<data_t>::operator!=(const link<data_t> & rhs) const
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
    return find_linked<typename input_container::const_iterator, data_type, output_container>(c.begin(), c.end(), v);
  }
}

#endif
