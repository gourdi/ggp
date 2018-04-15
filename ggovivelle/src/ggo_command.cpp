#include "ggo_command.h"
#include <ggo_string_helpers.h>
#include <ggo_kernel.h>
#include <ggo_rect_int.h>

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
  ggo::pos2i parse_margins(const parameters & params, int image_width, int image_height, int content_width, int content_height, const std::string & key)
  {
    auto margins_param = params[key];
    if (!margins_param || margins_param->empty() == true)
    {
      throw std::runtime_error("missing or empty '" + key + "' parameter");
    }

    auto margins = ggo::split(*margins_param, ',');
    if (margins.size() != 2)
    {
      throw std::runtime_error("expecting 2 values for the parameter '" + key + "'");
    }

    auto parse_margin = [& key](const std::string & margin, int image, int content)
    {
      // Pixel coordinate.
      if (margin.size() > 2 && margin.substr(margin.size() - 2) == "px")
      {
        // Right align.
        if (margin.front() == '-')
        {
          return image - content - ggo::to<int>(margin.substr(1, margin.size() - 3));
        }
        // Left align.
        else
        {
          return ggo::to<int>(margin.substr(0, margin.size() - 2));
        }
      }
      // Percentage.
      else if (margin.back() == '%')
      {
        float percent = ggo::to<float>(margin.substr(0, margin.size() - 1));
        return ggo::round_to<int>(percent * (image - content) / 100.f);
      }
      else
      {
        throw std::runtime_error("invalid value '" + margin + "' for key '" + key + "', must specify either 'px' or '%'");
        return 0;
      }
    };

    int x = parse_margin(margins[0], image_width, content_width);
    int y = parse_margin(margins[1], image_height, content_height);

    return { x, y };
  }
}
