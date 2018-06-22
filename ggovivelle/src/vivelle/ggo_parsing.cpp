#include "ggo_parsing.h"
#include "ggo_command.h"
#include <kernel/ggo_string_helpers.h>

namespace ggo
{
  namespace
  {
    /////////////////////////////////////////////////////////////////////
    ggo::size parse_size(const std::string & param, const ggo::size & image_size)
    {
      auto sizes = ggo::split(param, ',');
      if (sizes.size() != 2)
      {
        throw std::runtime_error("expecting 2 values for the size parameter");
      }

      auto parse_size1d = [](const std::string & size_1d, int image_size_1d)
      {
        // Pixel coordinate.
        if (size_1d.size() > 2 && size_1d.substr(size_1d.size() - 2) == "px")
        {
          return ggo::to<int>(size_1d.substr(0, size_1d.size() - 2));
        }
        // Percentage.
        else if (size_1d.back() == '%')
        {
          float percent = ggo::to<float>(size_1d.substr(0, size_1d.size() - 1));
          return ggo::round_to<int>(percent * image_size_1d / 100.f);
        }
        else
        {
          throw std::runtime_error("invalid value size parameter '" + size_1d + "', must specify either 'px' or '%'");
          return 0;
        }
      };

      int w = parse_size1d(sizes[0], image_size.width());
      int h = parse_size1d(sizes[1], image_size.height());

      if (w == 0 || h == 0)
      {
        throw std::runtime_error("scaling output size is 0");
      }

      return { w, h };
    }
  }

  /////////////////////////////////////////////////////////////////////
  ggo::rect_int parse_rect(const parameters & params, const ggo::size & image_size)
  {
    // Parse size first.
    auto size_param = params.get({ "size" });
    if (!size_param || size_param->empty() == true)
    {
      throw std::runtime_error("missing or empty 'size' parameter");
    }
    ggo::size rect_size = parse_size(*size_param, image_size);

    // Parse position.
    ggo::pos2i left_bottom = parse_margins(params, "pos", image_size, rect_size);

    // The output rect.
    int left    = left_bottom.x();
    int right   = left + rect_size.width() - 1;
    int bottom  = left_bottom.y();
    int top     = bottom + rect_size.height() - 1;
    return rect_int::from_left_right_bottom_top(left, right, bottom, top);
  }

  /////////////////////////////////////////////////////////////////////
  ggo::pos2i parse_margins(const parameters & params, const std::string & key, const ggo::size & image_size, const ggo::size & content_size)
  {
    auto margins_param = params.get({ key });
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
    auto scalings_param = params.get({ key });
    if (!scalings_param || scalings_param->empty() == true)
    {
      throw std::runtime_error("missing or empty '" + key + "' parameter");
    }

    return parse_size(*scalings_param, image_size);
  }
}
