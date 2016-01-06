#include "ggo_chronometer.h"
#include <sstream>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  chronometer::chronometer()
  {
    time(&_start_time);
  }

  //////////////////////////////////////////////////////////////
  double chronometer::get_elapsed_time() const
  {
    time_t end_time;
    
    time(&end_time);
    
    return difftime(end_time, _start_time);
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