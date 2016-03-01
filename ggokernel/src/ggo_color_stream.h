#ifndef __GGO_COLOR_STREAM__

#include <string>
#include <iostream>

namespace ggo
{
  enum class console_color
  {
    GREEN,
    YELLOW,
    RED
  };

  class color_stream
  {
  public:

    color_stream(console_color c);
    ~color_stream();

    template <typename T>
    color_stream & operator<<(const T & v) { std::cout << v; return *this; }

    // Make std::endl work with this class.
    typedef std::basic_ostream<char, std::char_traits<char>> cout_type;
    typedef cout_type& (*standard_end_line)(cout_type&);
    color_stream& operator<<(standard_end_line manip)
    {
      manip(std::cout);
      return *this;
    }

  private:
#ifdef _WIN32
      int _old_color;
#endif
  };
}



#endif
