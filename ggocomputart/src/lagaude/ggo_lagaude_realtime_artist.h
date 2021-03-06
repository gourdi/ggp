//#ifndef __GGO_LAGAUDE_REALTIME_ARTIST__
//#define __GGO_LAGAUDE_REALTIME_ARTIST__
//
//#include <ggo_realtime_artist_abc.h>
//#include <kernel/memory/ggo_array.h>
//#include <kernel/ggo_random_interpolator_abc.h>
//#include <animation/ggo_animator.h>
//#include <animation/ggo_animate_abc.h>
//#include <animation/paths/ggo_velocity_path.h>
//
//namespace ggo
//{
//  class lagaude_realtime_artist : public fixed_frames_count_realtime_artist_abc
//  {
//  public:
//
//          lagaude_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order);
//
//    void  preprocess_frame(int frame_index, uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step) override;
//    void  render_tile(void * buffer, int frame_index, const ggo::rect_int & clipping) override;
//    int   frames_count() const override { return 450; }
//
//  private:
//
//    template <ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order>
//    void  render_tile_t(void * buffer, int frame_index, const ggo::rect_int & clipping) const;
//
//    //////////////////////////////////////////////////////////////
//    class particle : public ggo::path_animate_abc
//    {
//    public:
//
//            particle(const ggo::pos2_f & pos, ggo::velocity_path * path);
//
//      bool  update(int frame_index, const ggo::pos2_f & pos) override;
//      void  render(ggo::image & img, const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const override;
//
//    public:
//
//      float		      _angle;
//      float		      _dangle;
//      float		      _radius;
//      float		      _dradius;
//      float		      _opacity;
//      ggo::rgb_8u   _color;
//    };
//
//    //////////////////////////////////////////////////////////////
//    class angle_generator : public ggo::random_interpolator_abc<float, float>
//    {
//      void	get_random_data(float & data, float & dt) override;
//    };
//
//    //////////////////////////////////////////////////////////////
//    class scale_animate_abc : public ggo::path_animate_abc
//    {
//    public:
//
//            scale_animate_abc(const ggo::pos2_f & pos, ggo::path_abc * path, float scale) : ggo::path_animate_abc(pos, path), _scale(scale) {};
//
//      float	get_scale() const { return _scale; };
//
//    protected:
//
//      float	_scale;
//    };
//
//    //////////////////////////////////////////////////////////////
//    class seed : public scale_animate_abc
//    {
//    public:
//
//            seed(const ggo::pos2_f & pos, ggo::path_abc * path, float scale, float hue);
//
//      bool  update(int frame_index, const ggo::pos2_f & pos) override;
//      void  render(ggo::image & img, const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const override;
//
//    private:
//
//      ggo::array<angle_generator, 1>  _angle_generators;
//      ggo::animator					          _particles_animator;
//      int								              _life;
//      ggo::rgb_8u			                _color;
//      float							              _dangle;
//    };
//
//    //////////////////////////////////////////////////////////////
//    class dust : public scale_animate_abc
//    {
//    public:
//
//            dust(const ggo::pos2_f & pos, ggo::path_abc * path, float scale) : scale_animate_abc(pos, path, scale) {};
//
//      bool  update(int frame_index, const ggo::pos2_f & pos) override;
//      void  render(ggo::image & img, const ggo::rect_int & clipping, int frame_index, const ggo::pos2_f & pos) const override;
//
//    public:
//
//      float	_val;
//      float	_radius;
//      float	_width;
//    };
//
//    //////////////////////////////////////////////////////////////
//    class sinuoid_path : public ggo::path_abc
//    {
//    public:
//
//      ggo::pos2_f	get_pos(int counter) override { return ggo::pos2_f(counter * _dx, _amplitude * std::sin(counter * _frequency)); };
//
//    public:
//
//      float	_dx;
//      float	_frequency;
//      float	_amplitude;
//    };
//
//    //////////////////////////////////////////////////////////////
//    struct bkgd_disc
//    {
//      ggo::pos2_f	_pos;
//      ggo::vec2_f	_vel;
//      float			  _radius;
//    };
//
//  private:
//
//    void	insert_scale_animator(scale_animate_abc * scale_animator);
//
//  private:
//
//    ggo::animator				    _animator;
//    std::vector<bkgd_disc>  _bkgd_discs;
//    float						        _hue;
//  };
//}
//
//#endif
