#ifndef __GGO_STRING_HELPERS__
#define __GGO_STRING_HELPERS__

#include <string>
#include <sstream>
#include <stdexcept>
#include <iterator>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename output_t>
  output_t to(const std::string & str)
  {
    output_t result;

    std::istringstream iss(str);

    iss >> result;

    if (iss.fail() || !iss.eof())
    {
      throw std::runtime_error("string conversion failed");
    }

    return result;
  }

  //////////////////////////////////////////////////////////////
  template <typename it, typename func>
  std::string join(it begin, it end, const std::string & delim, func f)
  {
    std::ostringstream oss;

    if (begin != end)
    {
      oss << f(*begin);
      ++begin;
      for (; begin != end; ++begin)
      {
        oss << delim << f(*begin);
      }
    }

    return oss.str();
  }

  //////////////////////////////////////////////////////////////
  template <typename it>
  std::string join(it begin, it end, const std::string & delim)
  {
    using value_t = std::iterator_traits<it>::value_type;

    auto f = [](const value_t & v) { return v; };

    return join(begin, end, delim, f);
  }

  //////////////////////////////////////////////////////////////
  inline std::vector<std::string> split(const std::string & s, char delim)
  {
    std::istringstream iss(s);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(iss, item, delim))
    {
      tokens.emplace_back(item);
    }
    return tokens;
  }

  //////////////////////////////////////////////////////////////
  template<typename T>
  std::string bin_dump(T value)
  {
    std::string	text;

    for (int i = 0; i < 8 * sizeof(T); ++i)
    {
      text += (value >> i) & 1 ? '1' : '0';
    }

    return text;
  }
}

#endif
