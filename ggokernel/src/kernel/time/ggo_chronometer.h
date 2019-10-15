#ifndef __GGO_CHRONOMETER__
#define __GGO_CHRONOMETER__

#include <chrono>
#include <string>

namespace ggo
{
  class chronometer
  {
  public:

    chronometer();

    double 		  get_elapsed_seconds() const;
    double 		  get_elapsed_milli_seconds() const;
    void		    get_elapsed_time(int & hours, int & minutes, int & seconds, int & milli_seconds) const;
    std::string	get_display_time(bool display_ms = false) const;

  private:

    std::chrono::system_clock::time_point _start_time;
  };
}

namespace ggo
{
  class scoped_chronometer final
  {
  public:

    scoped_chronometer(const std::string & label, bool display_ms);
   ~scoped_chronometer();

  private:

    std::string _label;
    bool _display_ms;
    chronometer _chronometer;
  };
}

#endif

