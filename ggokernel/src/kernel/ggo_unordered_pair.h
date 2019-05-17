#ifndef __GGO_UNORDERED_PAIR__
#define __GGO_UNORDERED_PAIR__

#include <algorithm>

//////////////////////////////////////////////////////////////
// Class representing a symmetric pair, where both value have
// the same role.

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  class unordered_pair
  {
  public:

    unordered_pair(data_t first, data_t second) : _first(first), _second(second) {}

    bool	operator==(const unordered_pair & rhs) const;
    bool	operator!=(const unordered_pair & rhs) const;

  public:

    data_t _first;
    data_t _second;
  };

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool unordered_pair<data_t>::operator==(const unordered_pair<data_t> & rhs) const
  {
    if (this->_first == rhs._first && this->_second == rhs._second) { return true; }
    if (this->_first == rhs._second && this->_second == rhs._first) { return true; }
    return false;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  bool unordered_pair<data_t>::operator!=(const unordered_pair<data_t> & rhs) const
  {
    return !(operator==(rhs));
  }
}

namespace ggo
{
  template <typename it, typename data_t, typename container = std::vector<data_t>>
  container find_paired(it first, it end, data_t v)
  {
    container result;
    for (; first != end; ++first)
    {
      if (first->_first == v)
      {
        result.push_back(first->_second);
      }
      if (first->_second == v)
      {
        result.push_back(first->_first);
      }
    }

    return result;
  }

  template <typename input_container, typename data_t, typename output_container = std::vector<data_t>>
  output_container find_paired(const input_container & c, data_t v)
  {
    return find_paired<typename input_container::const_iterator, data_t, output_container>(c.begin(), c.end(), v);
  }
}

#endif
