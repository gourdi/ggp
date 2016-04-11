#ifndef __GGO_RAH_ANIMATION_ARTIST__
#define __GGO_RAH_ANIMATION_ARTIST__

#include <ggo_animation_artist_abc.h>
#include <ggo_random_interpolator_abc.h>
#include <memory>
#include <functional>

class ggo_rah_animation_artist : public ggo_animation_artist_abc
{
public:

  ggo_rah_animation_artist(int render_width, int render_height);

private:

  struct ggo_rah_item
  {
    float _dist;
    
    virtual void  update(int min_size) = 0;
    virtual void  paint(uint8_t * buffer, int width, int height, float focus_dist) const = 0;
    virtual bool  is_alive(int width, int height, float focus_dist) const = 0;
    virtual bool  is_fog() const = 0;
  };
  
  using ggo_rah_item_ptr = std::shared_ptr<ggo_rah_item>;
  
  //////////////////////////////////////////////////////////////
  // FOG
  struct ggo_fog : public ggo_rah_item
  {
    struct ggo_position_interpolator : public ggo::random_interpolator_abc<ggo::point2d_float, float>
    {
      ggo_position_interpolator(int width, int height) : _width(width), _height(height) {}
    
      void get_random_data(ggo::point2d_float & data, float & dt) override
      {
        data.x() = ggo::rand_float(-0.2f * _width, 1.2f * _width);
        data.y() = ggo::rand_float(-0.2f * _height, 1.2f * _height);
        dt = ggo::rand_float(50, 100);
      }
      
      int _width;
      int _height;
    };
    
          ggo_fog(int width, int height);
  
    void  update(int min_size) override;
    void  paint(uint8_t * buffer, int width, int height, float focus_dist) const override;
    bool  is_alive(int width, int height, float focus_dist) const override;
    bool  is_fog() const override { return true; }
    
    ggo_position_interpolator _position_interpolator;
    ggo::point2d_float        _position;
  };

  //////////////////////////////////////////////////////////////
  // PARTICLE
  struct ggo_particle : public ggo_rah_item
  {
    struct ggo_vertical_offset_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      void get_random_data(float & data, float & dt) override
      {
        data = ggo::rand_float(-0.02f, 0.02f),
        dt = ggo::rand_float(25, 50);
      }
    };
    
          ggo_particle(int render_width, int render_height, float focus_dist);
    
    float blur_radius(int min_size, float focus_dist) const;
    float disc_radius(int min_size) const;
    
    void  update(int min_size) override;
    void  paint(uint8_t * buffer, int width, int height, float focus_dist) const override;
    bool  is_alive(int width, int height, float focus_dist) const override;
    bool  is_fog() const override { return false; }
    
    virtual void fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const = 0;
    
    ggo_vertical_offset_interpolator  _vertical_offset_interpolator;
    ggo::point2d_float                _pos;
    ggo::color                        _color;
    float                             _angle = 0;
  };
  
  //////////////////////////////////////////////////////////////
  // PARTICLE 1
  struct ggo_particle1 : public ggo_particle
  {
          ggo_particle1(int render_width, int render_height, float focus_dist);
  
    void  update(int min_size) override;
    void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const override;
    
    struct ggo_radius_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      void get_random_data(float & data, float & dt) override
      {
        data = ggo::rand_float(0.5, 1),
        dt = ggo::rand_float(10, 20);
      }
    };
    
    ggo::array<ggo_radius_interpolator>   _radius_interpolators;
    std::shared_ptr<ggo::polygon2d_float> _polygon;
  };
  
  //////////////////////////////////////////////////////////////
  // PARTICLE 2
  struct ggo_particle2 : public ggo_particle
  {
          ggo_particle2(int render_width, int render_height, float focus_dist);

    void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const override;
    
    int _point_count;
  };
  
  //////////////////////////////////////////////////////////////
  // PARTICLE 3
  struct ggo_particle3 : public ggo_particle
  {
          ggo_particle3(int render_width, int render_height, float focus_dist);

    void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const override;
    void  update(int min_size) override;
    
    struct ggo_radius_interpolator : public ggo::random_interpolator_abc<float, float>
    {
      void get_random_data(float & data, float & dt) override
      {
        data = ggo::rand_float(0.2f, 1.f),
        dt = ggo::rand_float(10.f, 20.f);
      }
    };
    
    float _width;
    float _height;
    ggo_radius_interpolator _width_interpolator;
    ggo_radius_interpolator _height_interpolator;
  };
  
  //////////////////////////////////////////////////////////////
  // PARTICLE 4
  struct ggo_particle4 : public ggo_particle
  {
          ggo_particle4(int render_width, int render_height, float focus_dist);
    
    void  fill_multi_shapes(ggo::multi_shape_float & borders, ggo::multi_shape_float & shapes, int min_size) const override;
  };
  
  struct ggo_focus_dist_interpolator : public ggo::random_interpolator_abc<float, float>
  {
    float _near;
    float _far;
  
    void get_random_data(float & data, float & dt) override
    {
      data = ggo::rand_float(_near, _near + 0.5f * (_far - _near));
      dt = ggo::rand_float(30.f, 50.f);
    }
  };
  
  std::vector<ggo_rah_item_ptr>                                           _items;
  std::function<bool(const ggo_rah_item_ptr &, const ggo_rah_item_ptr &)> _sort_func;
  ggo_focus_dist_interpolator                                             _focus_dist_interpolator;
    
private:
    
  void    init_sub() override;
  bool    render_next_frame_sub(uint8_t * buffer, int frame_index) override;

  static  std::shared_ptr<ggo_particle>   create_particle(float focus_dist, int render_width, int render_height);
};

#endif
