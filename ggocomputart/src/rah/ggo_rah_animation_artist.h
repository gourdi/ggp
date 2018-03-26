#ifndef __GGO_RAH_ANIMATION_ARTIST__
#define __GGO_RAH_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_random_interpolator_abc.h>
#include <functional>

namespace ggo
{
  class rah_animation_artist : public animation_artist_abc
  {
  public:

    rah_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt);

  private:

    struct rah_item
    {
      float _dist;

      virtual void  update(int min_size) = 0;
      virtual void  paint(void * buffer, int width, int height, float focus_dist) const = 0;
      virtual bool  is_alive(int width, int height, float focus_dist) const = 0;
      virtual bool  is_fog() const = 0;
    };

    using rah_item_ptr = std::shared_ptr<rah_item>;

    //////////////////////////////////////////////////////////////
    // FOG
    struct fog : public rah_item
    {
      struct position_interpolator : public ggo::random_interpolator_abc<ggo::pos2f, float>
      {
        position_interpolator(int width, int height) : _width(width), _height(height) {}

        void get_random_data(ggo::pos2f & data, float & dt) override
        {
          data.get<0>() = ggo::rand<float>(-0.2f * _width, 1.2f * _width);
          data.get<1>() = ggo::rand<float>(-0.2f * _height, 1.2f * _height);
          dt = ggo::rand<float>(50, 100);
        }

        int _width;
        int _height;
      };

            fog(int width, int height);

      void  update(int min_size) override;
      void  paint(void * buffer, int width, int height, float focus_dist) const override;
      bool  is_alive(int width, int height, float focus_dist) const override;
      bool  is_fog() const override { return true; }

      position_interpolator _position_interpolator;
      ggo::pos2f            _position;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE
    struct particle : public rah_item
    {
      struct ggo_vertical_offset_interpolator : public ggo::random_interpolator_abc<float, float>
      {
        void get_random_data(float & data, float & dt) override
        {
          data = ggo::rand<float>(-0.02f, 0.02f),
            dt = ggo::rand<float>(25, 50);
        }
      };

            particle(int width, int height, float focus_dist);

      float blur_radius(int min_size, float focus_dist) const;
      float disc_radius(int min_size) const;

      void  update(int min_size) override;
      void  paint(void * buffer, int width, int height, float focus_dist) const override;
      bool  is_alive(int width, int height, float focus_dist) const override;
      bool  is_fog() const override { return false; }

      virtual void fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const = 0;

      ggo_vertical_offset_interpolator  _vertical_offset_interpolator;
      ggo::pos2f                        _pos;
      ggo::color_8u                     _color;
      float                             _angle = 0;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 1
    struct particle1 : public particle
    {
            particle1(int width, int height, float focus_dist);

      void  update(int min_size) override;
      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;

      struct ggo_radius_interpolator : public ggo::random_interpolator_abc<float, float>
      {
        void get_random_data(float & data, float & dt) override
        {
          data = ggo::rand<float>(0.5, 1),
            dt = ggo::rand<float>(10, 20);
        }
      };

      ggo::array<ggo_radius_interpolator, 1>  _radius_interpolators;
      std::shared_ptr<ggo::polygon2d_float>   _polygon;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 2
    struct particle2 : public particle
    {
            particle2(int width, int height, float focus_dist);

      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;

      int _point_count;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 3
    struct particle3 : public particle
    {
            particle3(int width, int height, float focus_dist);

      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;
      void  update(int min_size) override;

      struct ggo_radius_interpolator : public ggo::random_interpolator_abc<float, float>
      {
        void get_random_data(float & data, float & dt) override
        {
          data = ggo::rand<float>(0.2f, 1.f),
            dt = ggo::rand<float>(10.f, 20.f);
        }
      };

      float _width;
      float _height;
      ggo_radius_interpolator _width_interpolator;
      ggo_radius_interpolator _height_interpolator;
    };

    //////////////////////////////////////////////////////////////
    // PARTICLE 4
    struct particle4 : public particle
    {
            particle4(int width, int height, float focus_dist);

      void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & bodies, int min_size) const override;
    };

    struct focus_dist_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      float _near;
      float _far;

      void get_random_data(float & data, float & dt) override
      {
        data = ggo::rand<float>(_near, _near + 0.5f * (_far - _near));
        dt = ggo::rand<float>(30.f, 50.f);
      }
    };

    int                                                             _frame_index;
    std::vector<rah_item_ptr>                                       _items;
    std::function<bool(const rah_item_ptr &, const rah_item_ptr &)> _sort_func;
    focus_dist_interpolator                                         _focus_dist_interpolator;
    float                                                           _focus_dist;

  private:

    void  init_animation() override;
    bool  prepare_frame() override;
    void  render_frame(void * buffer, const ggo::rect_int & clipping) override;

    static  std::shared_ptr<particle>   create_particle(float focus_dist, int width, int height);
  };
}

#endif