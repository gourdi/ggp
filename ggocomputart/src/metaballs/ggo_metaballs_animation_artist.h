//#ifndef __GGO_METABALLS_ANIMATION_ARTIST__
//#define __GGO_METABALLS_ANIMATION_ARTIST__
//
//#include <ggo_animation_artist.h>
//#include "ggo_metaballs_artist.h"
//
//namespace ggo
//{
//  class metaballs_animation_artist : public fixed_frames_count_animation_artist_abc
//  {
//  public:
//
//    metaballs_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);
//
//  private:
//
//    struct moving_center
//    {
//      ggo::pos3_f _center;
//      float				_radius = 0.f;
//      float				_start_angle1 = 0.f;
//      float				_start_angle2 = 0.f;
//      float				_end_angle1 = 0.f;
//      float				_end_angle2 = 0.f;
//    };
//
//  private:
//
//    void  render_frame(void * buffer, int frame_index, float time_step) override;
//
//  private:
//
//    ggo::metaballs_artist::params   _params;
//    std::vector<moving_center>      _centers;
//    ggo::basis3d_f              _camera_basis;
//  };
//}
//
//#endif
