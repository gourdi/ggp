#include "ggo_chronometer.h"
#include <sstream>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  chronometer::chronometer()
    :
    _start_time(std::chrono::system_clock::now())
  {
  }

  //////////////////////////////////////////////////////////////
  double chronometer::get_elapsed_time() const
  {
    std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - _start_time);

    return static_cast<double>(duration.count()) / 1000.f;
  }

  //////////////////////////////////////////////////////////////
  void chronometer::get_elapsed_time(int & hours, int & minutes, int & seconds) const
  {
    seconds = static_cast<int>(get_elapsed_time() + 0.5);
    
    hours = seconds / 3600;
    seconds -= 3600 * hours;
    minutes = seconds / 60;
    seconds -= 60 * minutes;
  }

  //////////////////////////////////////////////////////////////
  std::string chronometer::get_display_time() const
  {
    std::ostringstream oss;
    
    int hours, minutes, seconds;
    get_elapsed_time(hours, minutes, seconds);

    if (hours > 0)
    {
      oss << hours << 'h';
      oss.width(2);
      oss.fill('0');
      oss << minutes << 'm';
      oss.width(2);
      oss.fill('0');
      oss << seconds << 's';
    }
    else
    if (minutes > 0)
    {
      oss << minutes << 'm';
      oss.width(2);
      oss.fill('0');
      oss << seconds << 's';
    }
    else
    {
      oss << seconds << 's';
    }
    
    return oss.str();
  }
}