/////////////////////////////////////////////////////////////////////
template <typename color_type>
shapes_collection<color_type>::shapes_collection(ggo::paintable_shape_ptr shape,
                                                 const color_type & color,
                                                 float opacity,
                                                 std::shared_ptr<const ggo::blender_abc<color_type>> blender)
{
  add(shape, color, opacity, blender);
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
shapes_collection<color_type>::shapes_collection(ggo::paintable_shape_ptr shape,
                                                 std::shared_ptr<const ggo::brush_abc<color_type>> color_brush,
                                                 std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
                                                 std::shared_ptr<const ggo::blender_abc<color_type>> blender)
{
  add(shape, color_brush, opacity_brush, blender);
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
void shapes_collection<color_type>::add(ggo::paintable_shape_ptr shape,
                                        const color_type & color,
                                        float opacity,
                                        std::shared_ptr<const ggo::blender_abc<color_type>> blender)
{
  add(shape, std::make_shared<ggo::solid_brush<color_type>>(color), std::make_shared<ggo::opacity_solid_brush>(opacity), blender);
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
void shapes_collection<color_type>::add(ggo::paintable_shape_ptr shape,
                                        std::shared_ptr<const ggo::brush_abc<color_type>> color_brush,
                                        std::shared_ptr<const ggo::opacity_brush_abc> opacity_brush,
                                        std::shared_ptr<const ggo::blender_abc<color_type>> blender)
{
  ggo::shapes_collection<color_type>::layer layer;
  
  layer._shape = shape,
  layer._color_brush = color_brush,
  layer._opacity_brush = opacity_brush;
  layer._blender = blender;
  
  _layers.push_back(layer);
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
ggo::rect_float shapes_collection<color_type>::get_bounding_rect() const
{
  if (_layers.empty() == true)
  {
    return ggo::rect_float({ {0, 0}, 0, 0 });
  }
  
  auto it         = _layers.begin();
  auto rect_data  = it->_shape->get_bounding_rect();
  it++;
  
  while (it != _layers.end())
  {
    rect_data = ggo::rect_data_union(rect_data, it->_shape->get_bounding_rect());
    
    it++;
  }
  
  return ggo::rect_float(rect_data);
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
bool shapes_collection<color_type>::get_pixel_rect(int width,
                                                   int height,
                                                   float horz_extent,           
                                                   float vert_extent, 
                                                   ggo::pixel_rect & pixel_rect) const
{
  ggo::rect_float bounding_rect = get_bounding_rect();

  bounding_rect.inflate(horz_extent, vert_extent);

  pixel_rect = ggo::pixel_rect(bounding_rect);

  return pixel_rect.crop(width, height);
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
std::vector<ggo::paintable_shape_ptr> shapes_collection<color_type>::get_shapes() const
{
  std::vector<ggo::paintable_shape_ptr> result;

  for (const auto & layer : _layers)
  {
    result.push_back(layer._shape);
  }

  return result;
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
void shapes_collection<color_type>::process_block(ggo::image_data_abc<color_type> & image_data,
                                                  const ggo::pixel_rect & pixel_rect,
                                                  const ggo::pixel_sampler_abc & sampler,
                                                  bool sample_shapes) const
{
  pixel_rect.for_each_pixel([&](int x, int y)
  {
    color_type color = image_data.unpack(x, y);
    color = get_color_at_pixel(color, x, y, image_data.get_width(), image_data.get_height(), sampler, sample_shapes);
    image_data.pack(x, y, color);
  });
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
color_type shapes_collection<color_type>::get_color_at_pixel(const color_type & bkgd_color,
                                                             int x,
                                                             int y,
                                                             int width,
                                                             int height,
                                                             const ggo::pixel_sampler_abc & sampler,
                                                             bool sample_shapes) const
{
  color_type pixel_color(0);
  
  sampler.sample_pixel(x, y, [&](float x_f, float y_f)
  {
    color_type sample_color(bkgd_color);

    for (const auto & layer : _layers)
    {
      // The current sample is inside the layer's shape.
      if (sample_shapes == false || layer._shape->is_point_inside(x_f, y_f) == true)  
      {
        float opacity = layer._opacity_brush->get(x_f, y_f, *layer._shape, width, height);
        
        // Then get brush color and blend it.
        if (opacity > 0)
        {
          color_type shape_color = layer._color_brush->get(x_f, y_f, *layer._shape, width, height);
          sample_color = layer._blender->blend(sample_color, opacity, shape_color);
        }
      }
    }
    
    pixel_color += sample_color;
  });

  return pixel_color / static_cast<float>(sampler.get_samples_count());
}

/////////////////////////////////////////////////////////////////////
template <typename color_type>
void shapes_collection<color_type>::paint(ggo::image_data_abc<color_type> & image_data,
                                          const ggo::pixel_sampler_abc & sampler,
                                          const ggo::space_partitionning_2d_abc & space_partitionning) const
{
  ggo::pixel_rect pixel_rect;
  if (get_pixel_rect(image_data.get_width(), image_data.get_height(), sampler.get_horz_extent(), sampler.get_vert_extent(), pixel_rect) == false)
  {
    return;
  }

  if (sampler.is_pixel_aligned() == true)
  {
    space_partitionning.process_shapes(get_shapes(), pixel_rect,
      [&](const ggo::pixel_rect & fill_pixel_rect) // Fill block.
      {
        process_block(image_data, fill_pixel_rect, sampler, false);
      }, 
      [&](int x, int y, bool sample_shapes) // Fill pixel.
      {
        color_type color = image_data.unpack(x, y);
        color = get_color_at_pixel(color, x, y, image_data.get_width(), image_data.get_height(), sampler, sample_shapes);
        image_data.pack(x, y, color);
      });
  }
  else
  {
    process_block(image_data, pixel_rect, sampler, true);
  }
}


