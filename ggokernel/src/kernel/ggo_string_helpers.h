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
// Dump.
namespace ggo
{
  namespace details
  {
    template <typename data_t, int count>
    struct dump_t
    {
      static void dump(const data_t * ptr, std::ostream & os)
      {
        if constexpr (std::is_same_v<data_t, uint8_t>)
        {
          os << static_cast<int>(ptr[0]) << "; ";
        }
        else
        {
          os << ptr[0] << "; ";
        }
        ggo::details::dump_t<data_t, count - 1>::dump(ptr + 1, os);
      }
    };

    template <typename data_t>
    struct dump_t<data_t, 1>
    {
      static void dump(const data_t * ptr, std::ostream & os)
      {
        if constexpr (std::is_same_v<data_t, uint8_t>)
        {
          os << static_cast<int>(ptr[0]);
        }
        else
        {
          os << ptr[0];
        }
      }
    };
  }

  template <int size, typename data_t>
  void dump(const data_t * ptr, std::ostream & os)
  {
    os << "(";
    ggo::details::dump_t<data_t, size>::dump(ptr, os);
    os << ")";
  }

  template <int size, typename data_t>
  void dump(data_t const (&ptr)[size], std::ostream & os)
  {
    os << "(";
    ggo::details::dump_t<data_t, size>::dump(ptr, os);
    os << ")";
  }
}

//////////////////////////////////////////////////////////////
// String to value.
namespace ggo
{
  template <typename data_t>
  data_t to(const std::string & str)
  {
    if constexpr (std::is_same_v<data_t, bool>)
    {
      if (str == "true")
      {
        return true;
      }
      else if (str == "false")
      {
        return false;
      }
      else
      {
        throw std::runtime_error("failed converting string");
      }
    }
    else
    {
      data_t v;
      std::istringstream iss(str);
      iss >> v;
      if (iss.bad() || !iss.eof())
      {
        throw std::runtime_error("failed converting string");
      }
      return v;
    }
  }
}

//////////////////////////////////////////////////////////////
// Literals
inline constexpr uint8_t operator "" _u8(unsigned long long v) noexcept
{
  return static_cast<uint8_t>(v);
}

inline constexpr uint16_t operator "" _u16(unsigned long long v) noexcept
{
  return static_cast<uint16_t>(v);
}

inline constexpr uint32_t operator "" _u32(unsigned long long v) noexcept
{
  return static_cast<uint32_t>(v);
}

#endif
