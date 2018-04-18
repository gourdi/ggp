#ifndef __GGO_PARSING__
#define __GGO_PARSING__

#include <kernel/ggo_vec.h>
#include <kernel/ggo_size.h>
#include <kernel/ggo_rect_int.h>

namespace ggo
{
  class parameters;

  ggo::rect_int parse_rect(const parameters & params, const ggo::size & image_size);
  ggo::pos2i    parse_margins(const parameters & params, const std::string & key, const ggo::size & image_size, const ggo::size & content_size);
  ggo::size     parse_scaling(const parameters & params, const std::string & key, const ggo::size & image_size);
}

#endif
