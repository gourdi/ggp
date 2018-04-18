#include "ggo_parsing.h"
#include "ggo_command.h"
#include <kernel/ggo_string_helpers.h>

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  ggo::pos2i parse_margins(const parameters & params, const std::string & key, const ggo::size & image_size, const ggo::size & content_size)
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

    auto parse_margin = [&key](const std::string & margin, int image, int content)
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

    int x = parse_margin(margins[0], image_size.width(), content_size.width());
    int y = parse_margin(margins[1], image_size.height(), content_size.height());

    return { x, y };
  }

  /////////////////////////////////////////////////////////////////////
  ggo::size parse_scaling(const parameters & params, const std::string & key, const ggo::size & image_size)
  {
    auto scalings_param = params[key];
    if (!scalings_param || scalings_param->empty() == true)
    {
      throw std::runtime_error("missing or empty '" + key + "' parameter");
    }

    auto scalings = ggo::split(*scalings_param, ',');
    if (scalings.size() != 2)
    {
      throw std::runtime_error("expecting 2 values for the parameter '" + key + "'");
    }

    auto parse_scaling = [&key](const std::string & scaling, int image)
    {
      // Pixel coordinate.
      if (scaling.size() > 2 && scaling.substr(scaling.size() - 2) == "px")
      {
        return ggo::to<int>(scaling.substr(0, scaling.size() - 2));
      }
      // Percentage.
      else if (scaling.back() == '%')
      {
        float percent = ggo::to<float>(scaling.substr(0, scaling.size() - 1));
        return ggo::round_to<int>(percent * image / 100.f);
      }
      else
      {
        throw std::runtime_error("invalid value '" + scaling + "' for key '" + key + "', must specify either 'px' or '%'");
        return 0;
      }
    };

    int x = parse_scaling(scalings[0], image_size.width());
    int y = parse_scaling(scalings[1], image_size.height());

    if (x == 0 || y == 0)
    {
      throw std::runtime_error("scaling output size is 0");
    }

    return { x, y };
  }
}
