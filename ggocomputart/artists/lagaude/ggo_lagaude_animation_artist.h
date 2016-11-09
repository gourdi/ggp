#ifndef __GGO_LAGAUDE_ANIMATION_ARTIST__
#define __GGO_LAGAUDE_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_animator.h>
#include <ggo_animate_abc.h>
#include <ggo_linear_path.h>
#include <ggo_array.h>
#include <ggo_random_interpolator_abc.h>

namespace ggo
{
  class lagaude_animation_artist : public animation_artist_abc
  {
  public:

    lagaude_animation_artist(int render_width, int render_height);

    void	init_sub() override;
    bool	render_next_frame_sub(void * buffer, int frame_index) override;

  private:

    //////////////////////////////////////////////////////////////
    class particle : public ggo_path_animate_abc
    {
    public:

      particle(const ggo::pos2f & pos, ggo_linear_path * path);

      bool  update(void * buffer, int width, int height, int counter, const ggo::pos2f & pos) override;

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
    class scale_animate_abc : public ggo_path_animate_abc
    {
    public:

      scale_animate_abc(const ggo::pos2f & pos, ggo_path_abc * path, float scale) : ggo_path_animate_abc(pos, path), _scale(scale) {};

      float	get_scale() const { return _scale; };

    protected:

      float	_scale;
    };

    //////////////////////////////////////////////////////////////
    class seed : public scale_animate_abc
    {
    public:

      seed(const ggo::pos2f & pos, ggo_path_abc * path, float scale, float hue);

      bool	update(void * buffer, int width, int height, int counter, const ggo::pos2f & pos) override;

    private:

      ggo::array<angle_generator, 1>  _angle_generators;
      ggo_animator					          _particles_animator;
      int								              _life;
      ggo::color_8u			              _color;
      float							              _dangle;
    };

    //////////////////////////////////////////////////////////////
    class dust : public scale_animate_abc
    {
    public:

      dust(const ggo::pos2f & pos, ggo_path_abc * path, float scale) : scale_animate_abc(pos, path, scale) {};

      bool	update(void * buffer, int width, int height, int counter, const ggo::pos2f & pos) override;

    public:

      float	_val;
      float	_radius;
      float	_width;
    };

    //////////////////////////////////////////////////////////////
    class sinuoid_path : public ggo_path_abc
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

    ggo_animator				    _animator;
    std::vector<bkgd_disc>  _bkgd_discs;
    float						        _hue;
  };
}

#endif
