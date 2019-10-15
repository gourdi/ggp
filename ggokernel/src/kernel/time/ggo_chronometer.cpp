#include "ggo_chronometer.h"
#include <sstream>
#include <iostream>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  chronometer::chronometer()
    :
    _start_time(std::chrono::system_clock::now())
  {
  }

  //////////////////////////////////////////////////////////////
  double chronometer::get_elapsed_seconds() const
  {
    return get_elapsed_milli_seconds() / 1000.f;
  }

  //////////////////////////////////////////////////////////////
  double chronometer::get_elapsed_milli_seconds() const
  {
    std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - _start_time);

    return static_cast<double>(duration.count());
  }

  //////////////////////////////////////////////////////////////
  void chronometer::get_elapsed_time(int & hours, int & minutes, int & seconds, int & milli_seconds) const
  {
    long long elapsed_milli_seconds = static_cast<long long>(get_elapsed_milli_seconds() + 0.5);
    
    hours = static_cast<int>(elapsed_milli_seconds / 3600000);
    elapsed_milli_seconds -= 3600000 * hours;

    minutes = static_cast<int>(elapsed_milli_seconds / 60000);
    elapsed_milli_seconds -= 60000 * minutes;

    seconds = static_cast<int>(elapsed_milli_seconds / 1000);
    elapsed_milli_seconds -= 1000 * seconds;

    milli_seconds = static_cast<int>(elapsed_milli_seconds);
  }

  //////////////////////////////////////////////////////////////
  std::string chronometer::get_display_time(bool display_ms) const
  {
    std::ostringstream oss;
    
    int hours, minutes, seconds, milli_seconds;
    get_elapsed_time(hours, minutes, seconds, milli_seconds);

    if (hours > 0)
    {
      oss << hours << 'h';
      oss.width(2);
      oss.fill('0');
      oss << minutes << 'm';
      oss.width(2);
      oss.fill('0');
      oss << seconds << 's';
      if (display_ms == true)
      {
        oss.width(3);
        oss.fill('0');
        oss << milli_seconds << "ms";
      }
    }
    else
    if (minutes > 0)
    {
      oss << minutes << 'm';
      oss.width(2);
      oss.fill('0');
      oss << seconds << 's';
      if (display_ms == true)
      {
        oss.width(3);
        oss.fill('0');
        oss << milli_seconds << "ms";
      }
    }
    else
    {
      if (seconds > 0)
      {
        oss << seconds << 's';
        oss.width(3);
        oss.fill('0');
      }
      oss << milli_seconds << "ms";
    }
    
    return oss.str();
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  scoped_chronometer::scoped_chronometer(const std::string & label, bool display_ms)
    :
    _label(label), _display_ms(display_ms)
  {

  }
  
  //////////////////////////////////////////////////////////////
  scoped_chronometer::~scoped_chronometer()
  {
    std::cout << _label << ": " << _chronometer.get_display_time(_display_ms) << std::endl;
  }
}