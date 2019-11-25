//#ifndef _GGO_SMOKE_ANIMATION_ARTIST__
//#define _GGO_SMOKE_ANIMATION_ARTIST__
//
//#include <kernel/memory/ggo_array.h>
//#include <ggo_animation_artist.h>
//
//namespace ggo
//{
//  class smoke_animation_artist : public fixed_frames_count_animation_artist_abc
//  {
//  public:
//
//          smoke_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);
//
//     void  render_frame(void * buffer, int frame_index, float time_step) override;
//
//  private:
//
//    void process_sources(int frame_index);
//    void velocity_self_advection();
//    void make_incompressible();
//    void opacity_advection();
//    void diffuse();
//
//    struct fluid_source
//    {
//      ggo::disc_f _disc;
//      float			  _speed;
//      float			  _angle;
//      float			  _angle_amplitude;
//      float			  _angle_offset;
//      float			  _wave_length;
//      int					_timer1;
//      int					_timer2;
//      float			  _opacity;
//    };
//
//    float                       _cell_size;
//    float                       _density;
//    ggo::array<float, 2>	      _velocity_x1;
//    ggo::array<float, 2>	      _velocity_x2;
//    ggo::array<float, 2>	      _velocity_y1;
//    ggo::array<float, 2>	      _velocity_y2;
//    ggo::array<float, 2> *      _velocity_x_cur;
//    ggo::array<float, 2> *      _velocity_x_tmp;
//    ggo::array<float, 2> *      _velocity_y_cur;
//    ggo::array<float, 2> *      _velocity_y_tmp;
//    ggo::array<float, 2>	      _opacity1;
//    ggo::array<float, 2>	      _opacity2;
//    ggo::array<float, 2> *      _opacity_cur;
//    ggo::array<float, 2> *      _opacity_tmp;
//    ggo::array<fluid_source, 1> _sources;
//    ggo::array<uint8_t, 1>			_bkgd_buffer;
//    ggo::rgb_8u      	          _smoke_color;
//  };
//}
//
//#endif
