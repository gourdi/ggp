//#ifndef __GGO_WAKENDA_REALTIME_ARTIST__
//#define __GGO_WAKENDA_REALTIME_ARTIST__
//
//#include <ggo_realtime_artist_abc.h>
//#include <2d/ggo_image.h>
//
//namespace ggo
//{
//  class wakenda_realtime_artist : public realtime_artist_abc
//  {
//  public:
//
//    wakenda_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);
//
//  private:
//
//    void preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
//    void render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
//    bool finished(int frame_index) const override;
//
//    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order> void  copy_bkgd(void * buffer, const ggo::rect_int & clipping) const;
//    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order> void  process_bkgd(void * buffer, const ggo::rect_int & clipping) const;
//    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order> void  paint_points(void * buffer, const ggo::rect_int & clipping) const;
//
//  private:
//
//    struct point
//    {
//      point(float x, float y, const ggo::rgb_8u & color) : _pos(x, y), _color(color) {}
//      point(const ggo::pos2_f & p, const ggo::rgb_8u & color) : _pos(p), _color(color) {}
//
//      ggo::pos2_f _pos;
//      ggo::rgb_8u _color;
//      float _opacity = 0.f;
//      int _counter = 0;
//    };
//
//    struct transform
//    {
//      float _scale;
//      float _dx;
//      float _dy;
//      float _powx;
//      float _powy;
//    };
//
//    struct transform_interpolation
//    {
//      transform _start;
//      transform _end;
//    };
//
//    struct paint_point
//    {
//      paint_point(const ggo::pos2_f & p, const ggo::rgb_8u & color, float opacity) : _pos(p), _color(color), _opacity(opacity) {}
//
//      const ggo::pos2_f _pos;
//      const ggo::rgb_8u _color;
//      const float _opacity;
//    };
//
//  private:
//
//    float _hue;
//    ggo::image _bkgd_image;
//    std::vector<point> _points;
//    std::array<transform_interpolation, 4> _transforms_x;
//    std::array<transform_interpolation, 4> _transforms_y;
//    std::vector<paint_point> _paint_points;
//  };
//}
//
//#endif
