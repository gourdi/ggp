#ifndef __GGO_PARSING__
#define __GGO_PARSING__

#include <kernel/ggo_vec.h>
#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>
#include <2d/ggo_color.h>
#include <optional>

namespace ggo
{
  class parameters;

  ggo::rect_int parse_rect(const parameters & params, const ggo::size & image_size);
  ggo::pos2_i   parse_margins(const parameters & params, const std::string & key, const ggo::size & image_size, const ggo::size & content_size);
  ggo::size     parse_scaling(const parameters & params, const std::string & key, const ggo::size & image_size);

  template <typename color_t>
  std::optional<color_t>  parse_color(const std::string & color_str)
  {
    if (color_str == "black")
    {
      return ggo::black<color_t>();
    }
    else if (color_str == "gray")
    {
      return ggo::gray<color_t>();
    }
    else if (color_str == "white")
    {
      return ggo::white<color_t>();
    }
    else if (color_str == "red")
    {
      return ggo::red<color_t>();
    }
    else if (color_str == "green")
    {
      return ggo::green<color_t>();
    }
    else if (color_str == "blue")
    {
      return ggo::blue<color_t>();
    }
    else
    {
      return {};
    }
  }
}

#endif
