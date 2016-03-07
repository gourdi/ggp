#ifndef __GGO_ROTATE__
#define __GGO_ROTATE__

#if 0

#include <stdint.h>

class ggo_user_feedback_abc;

void	ggo_rotate(const uint8_t * input, int width_in, int height_in, uint8_t * output, int width_out, int height_out, float angle, uint8_t backgrnd);

void	ggo_rotate_90_cw(const uint8_t * in, uint8_t * out, int width, int height, ggo_user_feedback_abc * feedback);
void	ggo_rotate_90_ccw(const uint8_t * in, uint8_t * out, int width, int height, ggo_user_feedback_abc * feedback);
void	ggo_rotate_180(const uint8_t * in, uint8_t * out, int width, int height, ggo_user_feedback_abc * feedback);

void	ggo_flip_horz(uint8_t * buffer, int width, int height, ggo_user_feedback_abc * feedback);
void	ggo_flip_vert(uint8_t * buffer, int width, int height, ggo_user_feedback_abc * feedback);

#endif

#endif
