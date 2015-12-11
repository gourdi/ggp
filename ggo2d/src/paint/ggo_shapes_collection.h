#ifndef __GGO_MULTI_SHAPE_COLLECTION__
#define __GGO_MULTI_SHAPE_COLLECTION__

#include <ggo_brush_abc.h>
#include <ggo_blender_abc.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_image_data_abc.h>
#include <ggo_recursive_space_partitionning_2d.h>
#include <ggo_recursive_space_partitionning_2d.h>

namespace ggo
{
  template <typename T>
  class shapes_collection
  {
  public:

    shapes_collection() {}
    
    shapes_collection(ggo::paintable_shape_ptr shape,
                      const T & color,
                      float opacity = 1,
                      std::shared_ptr<const ggo::blender_abc<T>> blender = std::make_shared<ggo::alpha_blender<T>>());

    shapes_collection(ggo::paintable_shape_ptr shape,
                      std::shared_ptr<const ggo::brush_abc<T>> color_brush,
                      std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<ggo::opacity_solid_brush>(1),
                      std::shared_ptr<const ggo::blender_abc<T>> blender = std::make_shared<ggo::alpha_blender<T>>());
    
    void                                  add(ggo::paintable_shape_ptr shape,
                                              const T & color,
                                              float opacity = 1,
                                              std::shared_ptr<const ggo::blender_abc<T>> blender = std::make_shared<ggo::alpha_blender<T>>());

    void                                  add(ggo::paintable_shape_ptr shape,
                                              std::shared_ptr<const ggo::brush_abc<T>> color_brush,
                                              std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush = std::make_shared<ggo::opacity_solid_brush>(1.f),
                                              std::shared_ptr<const ggo::blender_abc<T>> blender = std::make_shared<ggo::alpha_blender<T>>());
    
    ggo::rect_float                       get_bounding_rect() const;
      
    bool                                  get_pixel_rect(int width, int height, float horz_extent, float vert_extent, ggo::pixel_rect & pixel_rect) const;
          
    T                                     get_color_at_pixel(const T & bkgd_color, int x, int y, int width, int height, const ggo::pixel_sampler_abc & sampler, bool sample_shapes) const;
          
    std::vector<ggo::paintable_shape_ptr> get_shapes() const;
      
    void                                  paint(ggo::image_data_abc<T> & image_data,
                                                const ggo::pixel_sampler_abc & sampler = ggo::pixel_sampler_4X4(),
                                                const ggo::space_partitionning_2d_abc & space_partitionning = ggo::recursive_space_partitionning_2d()) const;
      
  private:

    void                                  process_block(ggo::image_data_abc<T> & image_data, const ggo::pixel_rect & pixel_rect, const ggo::pixel_sampler_abc & sampler, bool sample_shapes) const;

  private:
  
    struct layer
    {
      ggo::paintable_shape_ptr                      _shape;
      std::shared_ptr<const ggo::brush_abc<T>>      _color_brush;
      std::shared_ptr<const ggo::opacity_brush_abc> _opacity_brush;
      std::shared_ptr<const ggo::blender_abc<T>>    _blender;
    };

    std::vector<layer> _layers;
  };

#include <ggo_shapes_collection.imp.h>

  using shapes_collection_gray = shapes_collection<float>;
  using shapes_collection_rgb  = shapes_collection<ggo::color>;
}

#endif

