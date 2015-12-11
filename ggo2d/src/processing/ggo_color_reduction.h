#ifndef __GGO_COLOR_REDUCTION__
#define __GGO_COLOR_REDUCTION__

#include <ggo_2d.h>

void	ggo_color_reduction_median_cut(const uint8_t * r, const uint8_t * g, const uint8_t * b, int size, ggo_palette & palette, int palette_size, ggo_user_feedback_abc * feedback);

#endif

