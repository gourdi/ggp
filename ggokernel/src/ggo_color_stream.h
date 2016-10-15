#ifndef __GGO_COLOR_STREAM__
#define __GGO_COLOR_STREAM__

#include <string>
#include <iostream>

namespace ggo
{
#ifdef GGO_WIN
  void set_console_green();
  void set_console_yellow();
  void set_console_red();
  void set_console_default();
#endif

  template<class CharT, class Traits>
  std::basic_ostream<CharT, Traits>& green_color(std::basic_ostream<CharT, Traits>& os)
  {
#ifdef GGO_WIN
    set_console_green();
#else
    os << "\033[1;32m";
#endif
    return os;
  }

  template<class CharT, class Traits>
  std::basic_ostream<CharT, Traits>& yellow_color(std::basic_ostream<CharT, Traits>& os)
  {
#ifdef GGO_WIN
    set_console_yellow();
#else
    os << "\033[1;33m";
#endif
    return os;
  }

  template<class CharT, class Traits>
  std::basic_ostream<CharT, Traits>& red_color(std::basic_ostream<CharT, Traits>& os)
  {
#ifdef GGO_WIN
    set_console_red();
#else
    os << "\033[31m";
#endif
    return os;
  }

  template<class CharT, class Traits>
  std::basic_ostream<CharT, Traits>& default_color(std::basic_ostream<CharT, Traits>& os)
  {
#ifdef GGO_WIN
    set_console_default();
#else
    os << "\033[0m";
#endif
    return os;
  }
}

#endif
