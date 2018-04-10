#ifndef __GGO_COMMAND__
#define __GGO_COMMAND__

#include <string>
#include <vector>

namespace ggo
{
  struct parameter
  {
    parameter(const std::string & key, const std::string & value) : _key(key), _value(value) {}

    std::string _key;
    std::string _value;
  };

  struct command
  {
    std::string _name;
    std::vector<parameter> _parameters;
  };

  command parse_command(const std::string & txt, bool filename_command = false);
}

#endif
