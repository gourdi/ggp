#pragma once

#include <kernel/ggo_rect_int.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <optional>

// Here, we are dealing with samples. A sample is a point. In particular, it has no span.
// The first sample has index 0 and coordinate 0.0.
// The second sample has index 1 and coordinate 1.0.
// etc...
namespace ggo
{
  //////////////////////////////////////////////////////////////
  // Given a scalar coordinate, return the index of the greatest sample that is below it.
  template <typename data_t>
  constexpr int lower_sample_index(data_t v)
  {
    static_assert(std::is_floating_point<data_t>::value);

    return (static_cast<float>(static_cast<int>(v)) == v)
      ? static_cast<int>(v)
      : static_cast<int>(v) + ((v > 0) ? 0 : -1);
  }

  static_assert(ggo::lower_sample_index(-2.0f) == -2);
  static_assert(ggo::lower_sample_index(-1.9f) == -2);
  static_assert(ggo::lower_sample_index(-1.1f) == -2);
  static_assert(ggo::lower_sample_index(-1.0f) == -1);
  static_assert(ggo::lower_sample_index(-0.9f) == -1);
  static_assert(ggo::lower_sample_index(-0.1f) == -1);
  static_assert(ggo::lower_sample_index(0.0f) == 0);
  static_assert(ggo::lower_sample_index(0.1f) == 0);
  static_assert(ggo::lower_sample_index(0.9f) == 0);
  static_assert(ggo::lower_sample_index(1.0f) == 1);
  static_assert(ggo::lower_sample_index(1.1f) == 1);
  static_assert(ggo::lower_sample_index(1.9f) == 1);
  static_assert(ggo::lower_sample_index(2.0f) == 2);
  static_assert(ggo::lower_sample_index(2.1f) == 2);
  static_assert(ggo::lower_sample_index(2.9f) == 2);
  static_assert(ggo::lower_sample_index(3.0f) == 3);

  //////////////////////////////////////////////////////////////
  // Given a scalar coordinate, return the index of the lowest sample that is above it.
  template <typename data_t>
  constexpr int upper_sample_index(data_t v)
  {
    static_assert(std::is_floating_point<data_t>::value);

    return (static_cast<float>(static_cast<int>(v)) == v)
      ? static_cast<int>(v)
      : static_cast<int>(v) + (v > 0 ? 1 : 0);
  }

  static_assert(upper_sample_index(-2.0f) == -2);
  static_assert(upper_sample_index(-1.9f) == -1);
  static_assert(upper_sample_index(-1.1f) == -1);
  static_assert(upper_sample_index(-1.0f) == -1);
  static_assert(upper_sample_index(-0.9f) == 0);
  static_assert(upper_sample_index(-0.1f) == 0);
  static_assert(upper_sample_index(0.0f) == 0);
  static_assert(upper_sample_index(0.1f) == 1);
  static_assert(upper_sample_index(0.9f) == 1);
  static_assert(upper_sample_index(1.0f) == 1);
  static_assert(upper_sample_index(1.1f) == 2);
  static_assert(upper_sample_index(1.9f) == 2);
  static_assert(upper_sample_index(2.0f) == 2);
  static_assert(upper_sample_index(2.1f) == 3);
  static_assert(upper_sample_index(2.9f) == 3);
  static_assert(upper_sample_index(3.0f) == 3);

  //////////////////////////////////////////////////////////////
// Given a scalar coordinate, return the index of the greatest sample that is below it.
  template <typename data_t>
  constexpr data_t sample_coordinate(int sample_index)
  {
    static_assert(std::is_floating_point<data_t>::value);

    return static_cast<data_t>(sample_index);
  }

  static_assert(sample_coordinate<float>(-7) == -7.f);
  static_assert(sample_coordinate<float>(-1) == -1.f);
  static_assert(sample_coordinate<float>(0) == 0.f);
  static_assert(sample_coordinate<float>(1) == 1.f);
  static_assert(sample_coordinate<float>(7) == 7.f);
}

// Here, we are dealing with pixel. A pixel is a 1x1 square.
// The pixel (0, 0) is the square [0.0; 1.0] x [0.0; 1.0].
// The pixel (1, 0) is the square [1.0; 2.0] x [0.0; 1.0].
// The pixel (0, 1) is the square [0.0; 1.0] x [1.0; 2.0].
// etc...
namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t>
  constexpr int discretize(data_t v)
  {
    static_assert(std::is_floating_point<data_t>::value);

    return v >= 0 ? static_cast<int>(v) : static_cast<int>(v) - 1;
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  constexpr data_t get_discrete_step_center(int v)
  {
    static_assert(std::is_floating_point<data_t>::value);

    return static_cast<data_t>(v) + data_t(0.5);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  constexpr ggo::pos2_i discretize(const ggo::pos2<data_t> & p)
  {
    static_assert(std::is_floating_point<data_t>::value);

    return { ggo::discretize(p.x()), ggo::discretize(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  constexpr ggo::pos2<data_t> get_pixel_center(const pos2_i & p)
  {
    static_assert(std::is_floating_point<data_t>::value);

    return { ggo::get_discrete_step_center<data_t>(p.x()), ggo::get_discrete_step_center<data_t>(p.y()) };
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_int discretize(const ggo::rect_data<data_t> & rect)
  {
    static_assert(std::is_floating_point<data_t>::value);

    int left = static_cast<int>(std::floor(rect._pos.x()));
    int past_right = static_cast<int>(std::ceil(rect._pos.x() + rect._width));
    int bottom = static_cast<int>(std::floor(rect._pos.y()));
    int past_top = static_cast<int>(std::ceil(rect._pos.y() + rect._height));

    return ggo::rect_int::from_left_right_bottom_top(left, past_right - 1, bottom, past_top - 1);
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t>
  ggo::rect_data<data_t> to_continuous(const ggo::rect_int & rect)
  {
    static_assert(std::is_floating_point<data_t>::value);

    data_t left   = static_cast<data_t>(rect.left());
    data_t right  = static_cast<data_t>(rect.right()) + 1;
    data_t bottom = static_cast<data_t>(rect.bottom());
    data_t top    = static_cast<data_t>(rect.top()) + 1;

    return ggo::rect_data<data_t>::from_left_right_bottom_top(left, right, bottom, top);
  }
}
