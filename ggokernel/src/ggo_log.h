#ifndef __GGO_LOG__
#define __GGO_LOG__

#include <ggo_color_stream.h>

#define GGO_LOG_ERROR(xxx) { std::cout << ggo::red_color << __FILE__ << ":" << __LINE__ << ": " << xxx << ggo::default_color << std::endl; }

#endif