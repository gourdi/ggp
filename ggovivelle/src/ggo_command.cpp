#include "ggo_command.h"
#include <ggo_string_helpers.h>
#include <ggo_kernel.h>

namespace
{
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

    auto command_parameters = split(txt, ':');

    // Check for a drive letter and if so, restore the filename.
    if (filename_command == true && command_parameters[0].size() == 1 && command_parameters.size() > 1)
    {
      command_parameters[1] = command_parameters[0] + ':' + command_parameters[1];
      command_parameters.erase(command_parameters.begin());
    }

    if (command_parameters.size() > 2)
    {
      throw std::runtime_error("more than 1 ':' in the command text");
    }

    command result;
    result._name = command_parameters[0];

    switch (command_parameters.size())
    {
    case 1: // No parameters, only the command.
      break;
    case 2: // Parameters need to be parsed too.
      result._parameters = parse_parameters(command_parameters[1]);
      break;
    default:
      GGO_FAIL();
      throw std::runtime_error("unexpected behavior");
      break;
    }

    return result;
  }
}
