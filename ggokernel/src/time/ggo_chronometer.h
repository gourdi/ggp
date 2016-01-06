#ifndef __GGO_CHRONOMETER__
#define __GGO_CHRONOMETER__

#include <time.h>
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
    
    time_t _start_time;
  };
}

#endif

