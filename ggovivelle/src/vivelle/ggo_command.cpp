#include "ggo_command.h"
#include <kernel/ggo_string_helpers.h>
#include <kernel/ggo_kernel.h>
#include <kernel/ggo_rect_int.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  parameters::parameters(const std::string & txt)
  {
    auto s = ggo::split(txt, ';');

    for (const auto & parameter : s)
    {
      auto key_value = ggo::split(parameter, '=');
      if (key_value.size() != 2)
      {
        throw std::runtime_error(std::string("invalid parameter: '") + parameter + "'");
      }

      _parameters[key_value[0]] = key_value[1];
    }
  }

  /////////////////////////////////////////////////////////////////////
  command::command(const std::string & txt, bit_mask<policy> policies) : _parameters("")
  {
    if (txt.empty() == true)
    {
      throw std::runtime_error("empty command text");
    }

    size_t pos = txt.find(':');
    if (pos == std::string::npos)
    {
      _name = txt;
      return;
    }

    // Handle file name command with a drive letter in it (like 'd:/foo.jpg').
    if (policies.has(policy::filename) == true && pos == 1)
    {
      pos = txt.find(':', pos + 1);
    }

    _name = txt.substr(0, pos);

    if (pos != std::string::npos)
    {
      _parameters = ggo::parameters(txt.substr(pos + 1));
    }
  }
}
