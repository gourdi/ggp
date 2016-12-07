#ifndef __GGO_HELPERS__
#define __GGO_HELPERS__

#include <string>
#include <sstream>

namespace ggo
{
  template<typename T>
  bool str2val(const std::string & str, T & value)
  {
    std::istringstream iss(str);

    iss >> value;

    return !iss.fail() && iss.eof();
  }

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
