#include "ggo_command.h"
#include <ggo_string_helpers.h>
#include <ggo_kernel.h>

namespace
{
  /////////////////////////////////////////////////////////////////////
  std::map<std::string, std::string> parse_parameters(const std::string & txt)
  {
    std::map<std::string, std::string> parameters;
    auto s = ggo::split(txt, ';');

    for (const auto & parameter : s)
    {
      auto key_value = ggo::split(parameter, '=');
      if (key_value.size() != 2)
      {
        throw std::runtime_error(std::string("invalid parameter: '") + parameter + "'");
      }

      parameters[key_value[0]] = key_value[1];
    }

    return parameters;
  }
}

namespace ggo
{
  command parse_command(const std::string & txt, bool filename_command)
  {
    if (txt.empty() == true)
    {
      throw std::runtime_error("empty command text");
    }

    command result;

    size_t pos = txt.find(':');
    if (pos == std::string::npos)
    {
      result._name = txt;
      return result;
    }

    // Handle file name command with a drive letter in it (like 'd:/foo.jpg').
    if (filename_command == true && pos == 1)
    {
      pos = txt.find(':', pos + 1);
    }

    result._name = txt.substr(0, pos);
    if (pos == std::string::npos)
    {
      return result;
    }

    std::string parameters = txt.substr(pos + 1);
    result._parameters = parse_parameters(parameters);

    return result;
  }
}
