//#ifndef __GGO_PLASTIC_ANIMATION_ARTIST__
//#define __GGO_PLASTIC_ANIMATION_ARTIST__
//
//#include <ggo_animation_artist.h>
//
//namespace ggo
//{
//  class plastic_animation_artist : public fixed_frames_count_animation_artist_abc
//  {
//  public:
//
//          plastic_animation_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);
//
//  private:
//
//    void  render_frame(void * buffer, int frame_index, float time_step) override;
//
//    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
//    void  render_frame_t(void * buffer) const;
//
//    float compute_altitude(float x, float y) const;
//
//  private:
//
//    struct params
//    {
//      float	  _den;
//      float	  _mult;
//      float   _power;
//      pos2_f	_center;
//      float	  _angle;
//      float	  _radius;
//      float	  _dangle;
//    };
//    std::vector<params> _params;
//
//    ggo::rgb_32f  _color;
//    float 		    _altitude_factor;
//  };
//}
//
//#endif
