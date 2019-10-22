#ifndef __GGO_REX_ARTIST__
#define __GGO_REX_ARTIST__

#include <ggo_bitmap_artist_abc.h>
#include <kernel/ggo_unordered_pair.h>
#include <2d/paint/ggo_color_triangle.h>
#include <2d/paint/ggo_multi_scale_paint.h>

namespace ggo
{
  class rex_artist : public bitmap_artist_abc
  {
  public:

          rex_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);

  private:

    using rex_edge = ggo::unordered_pair<const ggo::pos2_f *>;

    using color_triangle_rgb8u = ggo::solid_color_triangle<ggo::rgb_8u>;

    struct rex_pattern_triangle
    {
      ggo::pos2_f	_v1, _v2, _v3;
      float				_delta;
      float				_radius;
    };

    struct rex_disc_clip_triangle
    {
      ggo::pos2_f	_v1, _v2, _v3;
    };

    struct rex_opened_disc_data
    {
      ggo::pos2_f _center;
      float       _outter_radius;
      float       _inner_radius;
    };

    void	render_bitmap(void * buffer) const override;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_color_triangles(void * buffer, const scene2d<ggo::rgb_8u> & color_triangles) const;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_patterns(void * buffer, const std::vector<rex_pattern_triangle> & pattern_triangles) const;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_clipped_discs(void * buffer, const std::vector<rex_disc_clip_triangle> & discs_clip_triangles, const std::vector<rex_opened_disc_data> & opened_discs) const;

    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
    void render_edges(void * buffer, const std::vector<ggo::segment_f> & edges) const;
  };
}

#endif
