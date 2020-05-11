#ifndef __GGO_SIZE__
#define __GGO_SIZE__

#include <ostream>

namespace ggo
{
  class size
  {
  public:

    size(int w, int h) : _width(w), _height(h) {}

    int width() const { return _width; }
    int height() const { return _height; }

    int & width() { return _width; }
    int & height() { return _height; }

    bool operator==(const size & s) const { return s._width == _width && s._height == _height; }
    bool operator!=(const size & s) const { return s._width != _width || s._height != _height; }

  private:

    int _width;
    int _height;
  };

  inline std::ostream & operator<<(std::ostream & os, const size & s)
  {
    os << '(' << s.width() << ", " << s.height() << ')';
    return os;
  }
}

#endif
