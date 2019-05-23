#ifndef __GGO_GRID_SAMPLING__
#define __GGO_GRID_SAMPLING__

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::pos2<T>> grid_sampling(const paintable_shape2d_abc<T> & shape, T delta_sample)
  {
    std::vector<ggo::pos2<T>> samples;

    auto rect_data = shape.get_bounding_rect();
    T width = rect_data._width;
    T height = rect_data._height;
    ggo::pos2<T> center(rect_data._pos.x() + width / 2, rect_data._pos.y() + height / 2);

    for (T y = delta_sample / 2; y < height / 2; y += delta_sample)
    {
      for (T x = delta_sample / 2; x < width / 2; x += delta_sample)
      {
        T x1 = center.x() - x;
        T x2 = center.x() + x;
        T y1 = center.y() - y;
        T y2 = center.y() + y;

        if (shape.is_point_inside({ x1, y1 }) == true)
        {
          samples.push_back({ x1, y1 });
        }
        if (shape.is_point_inside({ x1, y2 }) == true)
        {
          samples.push_back({ x1, y2 });
        }
        if (shape.is_point_inside({ x2, y1 }) == true)
        {
          samples.push_back({ x2, y1 });
        }
        if (shape.is_point_inside({ x2, y2 }) == true)
        {
          samples.push_back({ x2, y2 });
        }
      }
    }

    return samples;
  }
}

#endif
