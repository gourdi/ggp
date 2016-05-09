#ifndef __GGO_SONSON_ANIMATION_ARTIST__
#define __GGO_SONSON_ANIMATION_ARTIST__

#include <ggo_rle_image.h>
#include <ggo_animation_artist_abc.h>
#include <ggo_kdtree.h>
#include <ggo_curve.h>
#include <memory>

class ggo_sonson_animation_artist : public ggo_animation_artist_abc
{
public:

              ggo_sonson_animation_artist(int render_width, int render_height);

  void        init_sub() override;
  bool        render_next_frame_sub(uint8_t * buffer, int frame_index) override;

private:

  void        create_line(int frame_index, bool foreground);

private:

  class ggo_line
  {
  public:

    static  ggo_line *                      create(float hue, int width, int height, int scale_factor);

            bool                            update();
            void                            render(ggo::rgb_image_buffer_uint8 & image) const;

  private:

                                            ggo_line(int width, int height, int scale_factor);


    std::pair<float, ggo::color>            get_pixel(int x, int y) const;
    ggo::segment_float                      get_segment() const;
    ggo::extended_segment_float             get_glow_segment() const;
    const std::vector<ggo::segment_float> & get_sparks() const { return _sparks; }

    void  render_masks();
    void  update_strips();
    void  update_sparks();
    void  setup_cw();
    void  setup_ccw();

  private:

    ggo::point2d_float _pos;
    ggo::vector2d_float _strip_dir;
    ggo::vector2d_float _velocity;

    // Arc.
    float _radius;
    ggo::point2d_float _center;
    float _angle_start;
    float _angle_end;
    bool _clock_wise;
    
    int _step;
    int _step_end;

    int _scale_factor;

    std::vector<std::pair<int, ggo::color>> _strips;
    ggo::rle_image<float>                   _opacity_mask;
    ggo::rle_image<ggo::color>              _color_mask;
    std::vector<ggo::segment_float>         _sparks;
  };

  float                                               _hue1;
  float                                               _hue2;
  std::vector<std::vector<std::unique_ptr<ggo_line>>> _lines;
};

#endif
