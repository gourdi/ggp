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

  /////////////////////////////////////////////////////////////////////
  template <typename T>
  std::vector<ggo::pos2<T>> adaptive_grid_sampling(const paintable_shape2d_abc<T> & shape, int samples_count, int iterations_count = 100)
  {
    auto rect_data = shape.get_bounding_rect();
    T width = rect_data._width;
    T height = rect_data._height;
    ggo::pos2<T> center(rect_data._pos.x() + width / 2, rect_data._pos.y() + height / 2);

    if (samples_count == 1)
    {
      std::vector<ggo::pos2<T>> samples;
      samples.emplace_back(center);
      return samples;
    }

    // Get the inf limit.
    T delta_inf = std::max(width, height);
    auto samples_inf = grid_sampling(shape, delta_inf);

    // Decrease delta_sup until we get more samples than required.
    T delta_sup = std::max(width, height);
    auto samples_sup = grid_sampling(shape, delta_sup);
    while (static_cast<int>(samples_sup.size()) < samples_count)
    {
      delta_sup /= 2;
      samples_sup = grid_sampling(shape, delta_sup);
    }

    GGO_ASSERT(samples_inf.size() <= samples_count);
    GGO_ASSERT(samples_sup.size() >= samples_count);

    // Then 'dichotomize'.
    for (int i = 0; i < iterations_count; ++i)
    {
      GGO_ASSERT(samples_inf.size() <= samples_sup.size());

      T delta_middle = (delta_inf + delta_sup) / 2;

      auto samples_middle = grid_sampling(shape, delta_middle);

      if (samples_count > static_cast<int>(samples_middle.size()))
      {
        delta_inf = delta_middle;
        samples_inf = samples_middle;
      }
      else
      {
        if (samples_count < static_cast<int>(samples_middle.size()))
        {
          delta_sup = delta_middle;
          samples_sup = samples_middle;
        }
        else
        {
          GGO_ASSERT(samples_middle.size() == samples_count);
          return samples_middle;
        }
      }
    }

    return samples_sup;
  }
}

#endif
