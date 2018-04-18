#ifndef __GGO_SEED_PAINT__
#define __GGO_SEED_PAINT__

namespace ggo
{
  template <typename seed_shape_t, typename paint_pixel_func>
  void paint_seed_shape(int width, int height, const seed_shape_t & shape, paint_pixel_func paint_pixel);
}

/////////////////////////////////////////////////////////////////////
// Implementation.
namespace ggo
{
  template <typename seed_shape_t, typename paint_pixel_func>
  void paint_seed_shape(int width, int height, const seed_shape_t & shape, paint_pixel_func paint_pixel)
  {
    using real_t = typename seed_shape_t::real_t;

    const ggo::rect<real_t> rect = shape.get_seed_rect();

    int seed_left = ggo::clamp<int>(static_cast<int>(rect.left()), 0, width - 1);
    int seed_right = ggo::clamp<int>(static_cast<int>(rect.right() + 1), 0, width - 1);
    int seed_bottom = ggo::clamp<int>(static_cast<int>(rect.bottom()), 0, height - 1);
    int seed_top = ggo::clamp<int>(static_cast<int>(rect.top() + 1), 0, height - 1);

    // First paint the seed.
    for (int y = seed_bottom; y <= seed_top; ++y)
    {
      for (int x = seed_left; x <= seed_right; ++x)
      {
        paint_pixel(x, y);
      }
    }

    // Then make it grow.
    while (true)
    {
      bool done = true;

      // Left
      if (seed_left > 0)
      {
        seed_left -= 1;
        for (int y = seed_bottom; y <= seed_top; ++y)
        {
          if (paint_pixel(seed_left, y) == true)
          {
            done = false;
          }
        }
      }

      // Right
      if (seed_right < width - 1)
      {
        seed_right += 1;
        for (int y = seed_bottom; y <= seed_top; ++y)
        {
          if (paint_pixel(seed_right, y) == true)
          {
            done = false;
          }
        }
      }

      // Top
      if (seed_top < height - 1)
      {
        seed_top += 1;
        for (int x = seed_left; x <= seed_right; ++x)
        {
          if (paint_pixel(x, seed_top) == true)
          {
            done = false;
          }
        }
      }

      // Bottom
      if (seed_bottom > 0)
      {
        seed_bottom -= 1;
        for (int x = seed_left; x <= seed_right; ++x)
        {
          if (paint_pixel(x, seed_bottom) == true)
          {
            done = false;
          }
        }
      }

      if (done == true)
      {
        break;
      }
    }
  }
}

#endif
