#ifndef __GGO_PIXEL_PROCESSING__
#define __GGO_PIXEL_PROCESSING__

#if 0

#include <stdint.h>
#include <ggo_2d.h>

void	ggo_brightness_constrast(uint8_t * buffer, int size, float brightness, float contrast, ggo_user_feedback_abc * feedback);
void	ggo_threshold(uint8_t * r, uint8_t * g, uint8_t * b, int size, float threshold, ggo_user_feedback_abc * feedback);

void	ggo_blend(uint8_t * r_out, uint8_t * g_out, uint8_t * b_out, const uint8_t * r_in, const uint8_t * g_in, const uint8_t * b_in, const uint8_t * alpha, int size);
void	ggo_zoom_merge_line(const uint8_t * r, const uint8_t * g, const uint8_t * b, uint8_t * out, int width, ggo_zoom_factor zoom_factor);

void	ggo_uniform_noise(uint8_t * r, uint8_t * g, uint8_t * b, int size, float amp, bool luma_only, ggo_user_feedback_abc * feedback);
void	ggo_blur(uint8_t * buffer, int width, int height, float variance, ggo_user_feedback_abc * feedback);
void	ggo_equalize_histogram(uint8_t * r, uint8_t * g, uint8_t * b, int size, ggo_user_feedback_abc * feedback);
void	ggo_apply_mapping(uint8_t * buffer, int size, const ggo_mapping & mapping, ggo_interpolation interpolation, ggo_user_feedback_abc * feedback);
void	ggo_apply_palette(uint8_t * r, uint8_t * g, uint8_t * b, int size, const ggo_palette & palette, bool optimized_palette, ggo_user_feedback_abc * feedback);
void	ggo_colorize(uint8_t * r, uint8_t * g, uint8_t * b, int size, ggo_color color, float factor, ggo_user_feedback_abc * feedback);
void	ggo_mozaic(uint8_t * buffer, int width, int height, int tile_width, int tile_height, ggo_user_feedback_abc * feedback);
void	ggo_wind(uint8_t * buffer, int width, int height, float direction, int length, ggo_user_feedback_abc * feedback);

void	ggo_grayscale(uint8_t * r, uint8_t * g, uint8_t * b, int size, ggo_user_feedback_abc * feedback);
void	ggo_invert(uint8_t * buffer, int size, ggo_user_feedback_abc * feedback);

#endif

#endif

