#ifndef __GGO_SONSON_ANIMATION_ARTIST__
#define __GGO_SONSON_ANIMATION_ARTIST__

#include <ggo_rle_image.h>
#include <ggo_animation_artist_abc.h>
#include <ggo_kdtree.h>
#include <ggo_curve.h>
#include <memory>

namespace ggo
{
  class sonson_animation_artist : public animation_artist_abc
  {
  public:

          sonson_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

    void  create_line(int frame_index, bool foreground);

  private:

    class line
    {
    public:

      static  line *  create(float hue, int width, int height, int scale_factor);

      bool            update();
      void            render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf) const;

    private:

      line(int width, int height, int scale_factor);

      std::pair<float, ggo::color_32f>        get_pixel(int x, int y) const;
      ggo::segment_float                      get_segment() const;
      ggo::extended_segment_float             get_glow_segment() const;
      const std::vector<ggo::segment_float> & get_sparks() const { return _sparks; }

      void  render_masks();
      void  update_strips();
      void  update_sparks();
      void  setup_cw();
      void  setup_ccw();

    private:

      ggo::pos2f _pos;
      ggo::vec2f _strip_dir;
      ggo::vec2f _velocity;

      // Arc.
      float _radius;
      ggo::pos2f _center;
      float _angle_start;
      float _angle_end;
      bool _clock_wise;

      int _step;
      int _step_end;

      int _scale_factor;

      std::vector<std::pair<int, ggo::color_32f>> _strips;
      ggo::rle_image<float>                       _opacity_mask;
      ggo::rle_image<ggo::color_32f>              _color_mask;
      std::vector<ggo::segment_float>             _sparks;
    };

    int                                             _frame_index;
    float                                           _hue1;
    float                                           _hue2;
    std::vector<std::vector<std::unique_ptr<line>>> _lines;
  };
}

#endif
