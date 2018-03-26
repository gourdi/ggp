#ifndef __GGO_LAGAUDE_ANIMATION_ARTIST__
#define __GGO_LAGAUDE_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_animator.h>
#include <ggo_animate_abc.h>
#include <ggo_velocity_path.h>
#include <ggo_array.h>
#include <ggo_random_interpolator_abc.h>

namespace ggo
{
  class lagaude_animation_artist : public animation_artist_abc
  {
  public:

          lagaude_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

  private:

    //////////////////////////////////////////////////////////////
    class particle : public ggo::path_animate_abc
    {
    public:

            particle(const ggo::pos2f & pos, ggo::velocity_path * path);

      bool  update(int frame_index, const ggo::pos2f & pos) override;
      void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const override;

    public:

      float		      _angle;
      float		      _dangle;
      float		      _radius;
      float		      _dradius;
      float		      _opacity;
      ggo::color_8u _color;
    };

    //////////////////////////////////////////////////////////////
    class angle_generator : public ggo::random_interpolator_abc<float, float>
    {
      void	get_random_data(float & data, float & dt) override;
    };

    //////////////////////////////////////////////////////////////
    class scale_animate_abc : public ggo::path_animate_abc
    {
    public:

            scale_animate_abc(const ggo::pos2f & pos, ggo::path_abc * path, float scale) : ggo::path_animate_abc(pos, path), _scale(scale) {};

      float	get_scale() const { return _scale; };

    protected:

      float	_scale;
    };

    //////////////////////////////////////////////////////////////
    class seed : public scale_animate_abc
    {
    public:

            seed(const ggo::pos2f & pos, ggo::path_abc * path, float scale, float hue);

      bool  update(int frame_index, const ggo::pos2f & pos) override;
      void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const override;

    private:

      ggo::array<angle_generator, 1>  _angle_generators;
      ggo::animator					          _particles_animator;
      int								              _life;
      ggo::color_8u			              _color;
      float							              _dangle;
    };

    //////////////////////////////////////////////////////////////
    class dust : public scale_animate_abc
    {
    public:

            dust(const ggo::pos2f & pos, ggo::path_abc * path, float scale) : scale_animate_abc(pos, path, scale) {};

      bool  update(int frame_index, const ggo::pos2f & pos) override;
      void  render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf, int frame_index, const ggo::pos2f & pos) const override;

    public:

      float	_val;
      float	_radius;
      float	_width;
    };

    //////////////////////////////////////////////////////////////
    class sinuoid_path : public ggo::path_abc
    {
    public:

      ggo::pos2f	get_pos(int counter) override { return ggo::pos2f(counter * _dx, _amplitude * std::sin(counter * _frequency)); };

    public:

      float	_dx;
      float	_frequency;
      float	_amplitude;
    };

    //////////////////////////////////////////////////////////////
    struct bkgd_disc
    {
      ggo::pos2f	_pos;
      ggo::vec2f	_vel;
      float			  _radius;
    };

  private:

    void	insert_scale_animator(scale_animate_abc * scale_animator);

  private:

    int                     _frame_index;
    ggo::animator				    _animator;
    std::vector<bkgd_disc>  _bkgd_discs;
    float						        _hue;
  };
}

#endif