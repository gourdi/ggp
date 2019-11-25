//#ifndef __GGO_ENTABENI_ANIMATION_ARTIST__
//#define __GGO_ENTABENI_ANIMATION_ARTIST__
//
//#include <ggo_animation_artist.h>
//#include <kernel/memory/ggo_array.h>
//#include <kernel/ggo_random_interpolator_abc.h>
//#include <kernel/math/interpolation/ggo_curve.h>
//
//namespace ggo
//{
//  class entabeni_animation_artist : public fixed_frames_count_animation_artist_abc
//  {
//  public:
//
//          entabeni_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);
//
//  private:
//
//    void  render_frame(void * buffer, int frame_index, float time_step) override;
//
//  private:
//
//    class angle_interpolator : public ggo::random_interpolator_abc<float, float>
//    {
//      void get_random_data(float & data, float & dt) override
//      {
//        data = ggo::rand<float>(0.f, 2.f * ggo::pi<float>());
//        dt = ggo::rand<float>(50, 100);
//      }
//    };
//
//    float                                   _angle = 0.f;
//    angle_interpolator                      _angle_interpolator;
//    float                                   _z = 0.f;
//    ggo::array<float, 2>                    _grid_start;
//    ggo::array<float, 2>                    _grid_end;
//    ggo::cubic_curve<float, ggo::rgb_32f>   _color_map;
//  };
//}
//
//#endif
