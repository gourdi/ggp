#include "ggo_command.h"
#include <ggo_string_helpers.h>
#include <ggo_kernel.h>

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
  command::command(const std::string & txt, bool filename_command) : _parameters("")
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
    if (filename_command == true && pos == 1)
    {
      pos = txt.find(':', pos + 1);
    }

    _name = txt.substr(0, pos);

    if (pos != std::string::npos)
    {
      _parameters = ggo::parameters(txt.substr(pos + 1));
    }
  }

  /////////////////////////////////////////////////////////////////////
  int parse_margin(const parameters & params, int image_size, int content_size, const std::string & key)
  {
    auto pos_param = params[key];

    if (!pos_param || pos_param->empty() == true)
    {
      throw std::runtime_error("missing or empty '" + key + "' parameter");
    }

    // Pixel coordinate.
    if (pos_param->size() > 2 && pos_param->substr(pos_param->size() - 2) == "px")
    {
      // Right align.
      if (pos_param->front() == '-')
      {
        return image_size - content_size - ggo::to<int>(pos_param->substr(1, pos_param->size() - 3));
      }
      // Left align.
      else
      {
        return ggo::to<int>(pos_param->substr(0, pos_param->size() - 2));
      }
    }
    // Percentage.
    else if (pos_param->back() == '%')
    {
      float percent = ggo::to<float>(pos_param->substr(0, pos_param->size() - 1));
      return ggo::round_to<int>(percent * (image_size - content_size) / 100.f);
    }
    else
    {
      throw std::runtime_error("invalid value '" + *pos_param + "' for key '" + key + "', must specify either 'px' or '%'");
      return 0;
    }
  }
}
