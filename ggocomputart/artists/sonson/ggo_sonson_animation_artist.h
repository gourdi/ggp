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

          sonson_animation_artist(int render_width, int render_height);

  private:

    void  init_sub() override;
    bool  render_next_frame_sub(void * buffer, int frame_index) override;

    void  create_line(int frame_index, bool foreground);

  private:

    class line
    {
    public:

      static  line *  create(float hue, int width, int height, int scale_factor);

      bool            update();
      void            render(void * buffer, int width, int height) const;

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

    float                                           _hue1;
    float                                           _hue2;
    std::vector<std::vector<std::unique_ptr<line>>> _lines;
  };
}

#endif
