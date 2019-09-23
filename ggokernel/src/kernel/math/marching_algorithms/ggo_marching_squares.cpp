#include "ggo_marching_squares.h"
#include <kernel/ggo_assert.h>

namespace
{
  float interpolate(float x1, float v1, float x2, float v2)
  {
    GGO_ASSERT(x1 < x2);

    // First we find the equation of the line y = a * x + b where:
    // v1 = a * x1 + b
    // v2 = a * x2 + b
    float den = (x2 - x1);
    if (std::abs(den) < 0.0001f)
    {
      return 0.5f * (x1 + x2);
    }
    float a = (v1 - v2) / den;
    if (std::abs(a) < 0.0001f)
    {
      return 0.5f * (x1 + x2);
    }
    float b = (x1 * v2 - x2 * v1) / den;

    // And the, we find the absicca where the line cancels.
    float x = -b / a;
    return x;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  std::vector<ggo::marching_square_cell> marching_squares(ggo::func2d func, ggo::pos2_f inf, int steps, float step_size)
  {
    std::vector<ggo::marching_square_cell> cells;

    for (int y = 0; y < steps; ++y)
    {
      float y_inf = inf.y() + y * step_size;
      float y_sup = inf.y() + (y + 1) * step_size;

      for (int x = 0; x < steps; ++x)
      {
        float x_inf = inf.x() + x * step_size;
        float x_sup = inf.x() + (x + 1) * step_size;

        float v00 = func(x_inf, y_inf);
        float v01 = func(x_inf, y_sup);
        float v10 = func(x_sup, y_inf);
        float v11 = func(x_sup, y_sup);

        ggo::marching_square_cell cell;
        cell._x = x;
        cell._y = y;

        // ++ or -- 
        // -+    +-
        if ((v00 <= 0.f && v10 >= 0.f && v01 >= 0.f && v11 >= 0.f) ||
            (v00 >= 0.f && v10 <= 0.f && v01 <= 0.f && v11 <= 0.f))
        {
          ggo::pos2_f p1{ interpolate(x_inf, v00, x_sup, v10), y_inf };
          ggo::pos2_f p2{ x_inf, interpolate(y_inf, v00, y_sup, v01) };
          cell._segments.emplace_back(p1, p2);
          cells.push_back(cell);
          continue;
        }

        // ++ or -- 
        // +-    -+
        if ((v00 >= 0.f && v10 <= 0.f && v01 >= 0.f && v11 >= 0.f) ||
            (v00 <= 0.f && v10 >= 0.f && v01 <= 0.f && v11 <= 0.f))
        {
          ggo::pos2_f p1{ interpolate(x_inf, v00, x_sup, v10), y_inf };
          ggo::pos2_f p2{ x_sup, interpolate(y_inf, v10, y_sup, v11) };
          cell._segments.emplace_back(p1, p2);
          cells.push_back(cell);
          continue;
        }

        // -+ or +- 
        // ++    --
        if ((v00 >= 0.f && v10 >= 0.f && v01 <= 0.f && v11 >= 0.f) ||
            (v00 <= 0.f && v10 <= 0.f && v01 >= 0.f && v11 <= 0.f))
        {
          ggo::pos2_f p1{ interpolate(x_inf, v01, x_sup, v11), y_sup };
          ggo::pos2_f p2{ x_inf, interpolate(y_inf, v00, y_sup, v01) };
          cell._segments.emplace_back(p1, p2);
          cells.push_back(cell);
          continue;
        }

        // +- or -+ 
        // ++    --
        if ((v00 >= 0.f && v10 >= 0.f && v01 >= 0.f && v11 <= 0.f) ||
            (v00 <= 0.f && v10 <= 0.f && v01 <= 0.f && v11 >= 0.f))
        {
          ggo::pos2_f p1{ interpolate(x_inf, v01, x_sup, v11), y_sup };
          ggo::pos2_f p2{ x_sup, interpolate(y_inf, v10, y_sup, v11) };
          cell._segments.emplace_back(p1, p2);
          cells.push_back(cell);
          continue;
        }

        // -+ or +- 
        // -+    +-
        if ((v00 <= 0.f && v10 >= 0.f && v01 <= 0.f && v11 >= 0.f) ||
            (v00 >= 0.f && v10 <= 0.f && v01 >= 0.f && v11 <= 0.f))
        {
          ggo::pos2_f p1{ interpolate(x_inf, v00, x_sup, v10), y_inf };
          ggo::pos2_f p2{ interpolate(x_inf, v01, x_sup, v11), y_sup };
          cell._segments.emplace_back(p1, p2);
          cells.push_back(cell);
          continue;
        }

        // ++ or -- 
        // --    ++
        if ((v00 <= 0.f && v10 <= 0.f && v01 >= 0.f && v11 >= 0.f) ||
            (v00 >= 0.f && v10 >= 0.f && v01 <= 0.f && v11 <= 0.f))
        {
          ggo::pos2_f p1{ x_inf, interpolate(y_inf, v00, y_sup, v01) };
          ggo::pos2_f p2{ x_sup, interpolate(y_inf, v10, y_sup, v11) };
          cell._segments.emplace_back(p1, p2);
          cells.push_back(cell);
          continue;
        }

        // -+ or +- 
        // +-    -+
        // Here we have a choice, so we pick up arbritrarily the 2 segments like this:
        // -\+
        // \ \ 
        // +\-
        if ((v00 >= 0.f && v10 <= 0.f && v01 <= 0.f && v11 >= 0.f) ||
            (v00 <= 0.f && v10 >= 0.f && v01 >= 0.f && v11 <= 0.f))
        {
          ggo::pos2_f p1{ interpolate(x_inf, v00, x_sup, v10), y_inf };
          ggo::pos2_f p2{ x_inf, interpolate(y_inf, v00, y_sup, v01) };
          cell._segments.emplace_back(p1, p2);

          ggo::pos2_f p3{ interpolate(x_inf, v01, x_sup, v11), y_sup };
          ggo::pos2_f p4{ x_sup, interpolate(y_inf, v01, y_sup, v01) };
          cell._segments.emplace_back(p3, p4);

          cells.push_back(cell);
          continue;
        }
      }
    }

    return cells;
  }
}