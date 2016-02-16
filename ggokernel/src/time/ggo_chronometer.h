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
    
    double 		  get_elapsed_time() const;
    void		    get_elapsed_time(int & hours, int & minutes, int & seconds) const;
    std::string	get_display_time() const;
    
  private:
    
    std::chrono::system_clock::time_point _start_time;
  };
}

#endif

