#ifndef __GGO_STRING_HELPERS__
#define __GGO_STRING_HELPERS__

#include <string>
#include <sstream>
#include <stdexcept>
#include <iterator>
#include <vector>
#include <optional>

//////////////////////////////////////////////////////////////
// join - split
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename it_t>
  void join(std::ostream & os, it_t begin, it_t end, const std::string & delim)
  {
    if (begin != end)
    {
      os << *begin;
      ++begin;
      for (; begin != end; ++begin)
      {
        os << delim << *begin;
      }
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename container_t>
  void join(std::ostream & os, const container_t & container, const std::string & delim)
  {
    join(os, container.begin(), container.end(), delim);
  }

  //////////////////////////////////////////////////////////////
  template <typename it_t>
  std::string join(it_t begin, it_t end, const std::string & delim)
  {
    std::ostringstream oss;

    join(oss, begin, end, delim);

    return oss.str();
  }

  //////////////////////////////////////////////////////////////
  template <typename container_t = std::vector<std::string>>
  inline container_t split(const std::string & s, char delim)
  {
    std::istringstream iss(s);
    std::string item;
    container_t tokens;
    while (std::getline(iss, item, delim))
    {
      tokens.emplace_back(item);
    }
    return tokens;
  }
}

//////////////////////////////////////////////////////////////
// bin_dump
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template<typename data_t>
  std::string bin_dump(data_t value)
  {
    std::string	text;

    for (int i = 0; i < 8 * sizeof(data_t); ++i)
    {
      text += (value >> i) & 1 ? '1' : '0';
    }

    return text;
  }
}

//////////////////////////////////////////////////////////////
// string to value
namespace ggo
{
  template <typename data_t>
  std::optional<data_t> to(const std::string & str)
  {
    return {};
  }

  template <> inline
  std::optional<bool> to(const std::string & str)
  {
    if (str == "true")
    {
      return true;
    }
    if (str == "false")
    {
      return false;
    }
    return {};
  }

  template <> inline
  std::optional<int> to(const std::string & str)
  {
    std::optional<int> val;
    try
    {
      val = stoi(str);
    }
    catch (...)
    {

    }
    return val;
  }
}

//////////////////////////////////////////////////////////////
// Literals
inline constexpr uint8_t operator "" _u8(unsigned long long v) noexcept
{
  return static_cast<uint8_t>(v);
}

#endif
