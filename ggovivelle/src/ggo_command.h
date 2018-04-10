#ifndef __GGO_COMMAND__
#define __GGO_COMMAND__

#include <string>
#include <map>

namespace ggo
{
  struct command
  {
    std::string _name;
    std::map<std::string, std::string> _parameters;
  };

  command parse_command(const std::string & txt, bool filename_command = false);
}

#endif
