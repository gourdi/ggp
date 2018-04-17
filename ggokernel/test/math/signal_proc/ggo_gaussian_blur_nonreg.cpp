//#define GGO_GAUSSIAN_DEBUG 1

#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/math/signal_proc/ggo_gaussian_blur.h>
#include <kernel/memory/ggo_memory_layouts.h>
#include <kernel/memory/ggo_edges_management.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, kernel_floating_point)
{
  auto compute_norm = [](const std::vector<float> & kernel)
  {
    auto norm = std::accumulate(kernel.begin(), kernel.end(), 0.f);
    return std::accumulate(kernel.begin() + 1, kernel.end(), norm);
  };

  std::vector<std::pair<float, float>> stddev_thresholds{ { 1.f, 0.01f }, { 2.f, 0.01f }, { 5.f, 0.01f }, { 5.f, 0.0001f } };

  for (const auto & stddev_threshold : stddev_thresholds)
  {
    auto kernel = ggo::build_gaussian_kernel(stddev_threshold.first, stddev_threshold.second);
    GGO_CHECK_FLOAT_EQ(compute_norm(kernel), 1.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, kernel_fixed_point_16u)
{
  auto compute_norm = [](const std::vector<uint16_t> & kernel)
  {
    auto norm = std::accumulate(kernel.begin(), kernel.end(), 0);
    return std::accumulate(kernel.begin() + 1, kernel.end(), norm);
  };

  std::vector<std::pair<float, float>> stddev_thresholds{ { 1.f, 0.01f },{ 2.f, 0.01f },{ 5.f, 0.01f },{ 5.f, 0.0001f } };

  for (const auto & stddev_threshold : stddev_thresholds)
  {
    auto kernel = ggo::build_fixed_point_gaussian_kernel<uint16_t>(stddev_threshold.first, stddev_threshold.second, 8);
    GGO_CHECK_EQ(compute_norm(kernel), 256);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, kernel_fixed_point_32u)
{
  auto compute_norm = [](const std::vector<uint32_t> & kernel)
  {
    auto norm = std::accumulate(kernel.begin(), kernel.end(), 0);
    return std::accumulate(kernel.begin() + 1, kernel.end(), norm);
  };

  // Beware that here we use double. Floats are not precise enough.
  std::vector<std::pair<double, double>> stddev_thresholds{ { 1., 0.01 },{ 2., 0.01 },{ 5., 0.01 },{ 5., 0.0001 } };

  for (const auto & stddev_threshold : stddev_thresholds)
  {
    auto kernel = ggo::build_fixed_point_gaussian_kernel<uint32_t>(stddev_threshold.first, stddev_threshold.second, 24);
    GGO_CHECK_EQ(compute_norm(kernel), uint32_t(1 << 24));
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 1d_32f_zero)
{
  const float in[7] = { 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f };
  float out[7] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

  auto kernel = ggo::build_gaussian_kernel<float>(0.8f, 0.01f);
  GGO_CHECK_EQ(kernel.size(), 3);

  auto input_it  = ggo::make_read_only_iterator(in);
  auto output_it = ggo::make_write_only_iterator(out);

  auto left   = [&](int x) { return 0.f; };
  auto right  = [&](int x) { return 0.f; };

  ggo::apply_symetric_kernel_1d(input_it, output_it, left, right, 7, kernel.data(), kernel.size());

  GGO_CHECK_FLOAT_EQ(out[0], 0.f);
  GGO_CHECK_FLOAT_EQ(out[1], 0.0219296496f);
  GGO_CHECK_FLOAT_EQ(out[2], 0.228512138f);
  GGO_CHECK_FLOAT_EQ(out[3], 0.499116361f);
  GGO_CHECK_FLOAT_EQ(out[4], 0.228512138f);
  GGO_CHECK_FLOAT_EQ(out[5], 0.0219296496f);
  GGO_CHECK_FLOAT_EQ(out[6], 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 1d_8u_32f_zero)
{
  const uint8_t in[7] = { 0, 0, 0, 255, 0, 0, 0 };
  uint8_t out[7] = { 0, 0, 0, 0, 0, 0, 0 };

  auto kernel = ggo::build_gaussian_kernel<float>(0.8f, 0.01f);
  GGO_CHECK_EQ(kernel.size(), 3);

  auto input_it  = ggo::make_cast_read_only_iterator<float, ggo::cast_mode::regular>(in);
  auto output_it = ggo::make_cast_write_only_iterator<float, ggo::cast_mode::round>(out);

  auto left   = [&](int x) { return 0.f; };
  auto right  = [&](int x) { return 0.f; };

  ggo::apply_symetric_kernel_1d(input_it, output_it, left, right, 7, kernel.data(), kernel.size());

  GGO_CHECK_EQ(out[0], 0);
  GGO_CHECK_EQ(out[1], 6);
  GGO_CHECK_EQ(out[2], 58);
  GGO_CHECK_EQ(out[3], 127);
  GGO_CHECK_EQ(out[4], 58);
  GGO_CHECK_EQ(out[5], 6);
  GGO_CHECK_EQ(out[6], 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 1d_fixed_point_8u_16u_zero)
{
  const uint8_t in[7] = { 0, 0, 0, 255, 0, 0, 0 };
  uint8_t out[7] = { 0, 0, 0, 0, 0, 0, 0 };

  auto kernel = ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(0.8f, 0.01f, 8);
  GGO_CHECK_EQ(kernel.size(), 3);

  auto input_it  = ggo::make_cast_read_only_iterator<uint16_t, ggo::cast_mode::regular>(in);
  auto output_it = ggo::make_fixed_point_write_only_iterator<uint16_t, 8>(out);

  auto left   = [&](int x) { return static_cast<uint16_t>(0); };
  auto right  = [&](int x) { return static_cast<uint16_t>(0); };

  ggo::apply_symetric_kernel_1d(input_it, output_it, left, right, 7, kernel.data(), kernel.size());

  GGO_CHECK_EQ(out[0], 0);
  GGO_CHECK_EQ(out[1], 6);
  GGO_CHECK_EQ(out[2], 58);
  GGO_CHECK_EQ(out[3], 128);
  GGO_CHECK_EQ(out[4], 58);
  GGO_CHECK_EQ(out[5], 6);
  GGO_CHECK_EQ(out[6], 0);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 1d_fixed_point_8u_16u_mirror)
{
  const uint8_t in[9] = { 0, 255, 0, 0, 0, 0, 0, 0, 255 };
  uint8_t out[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  auto kernel = ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(0.8f, 0.01f, 8);
  GGO_CHECK_EQ(kernel.size(), 3);

  auto input_it  = ggo::make_cast_read_only_iterator<uint16_t, ggo::cast_mode::regular>(in);
  auto output_it = ggo::make_fixed_point_write_only_iterator<uint16_t, 8>(out);

  auto get1d  = [&](int x) { return in[x]; };
  auto left   = [&](int x) { return static_cast<uint16_t>(ggo::get1d_mirror(get1d, x, 9)); };
  auto right  = [&](int x) { return static_cast<uint16_t>(ggo::get1d_mirror(get1d, x, 9)); };

  ggo::apply_symetric_kernel_1d(input_it, output_it, left, right, 9, kernel.data(), kernel.size());

  GGO_CHECK_EQ(out[0], 64);
  GGO_CHECK_EQ(out[1], 128);
  GGO_CHECK_EQ(out[2], 58);
  GGO_CHECK_EQ(out[3], 6);
  GGO_CHECK_EQ(out[4], 0);
  GGO_CHECK_EQ(out[5], 0);
  GGO_CHECK_EQ(out[6], 6);
  GGO_CHECK_EQ(out[7], 64);
  GGO_CHECK_EQ(out[8], 185);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 2d_32f_zero)
{
  std::vector<float> in(5 * 5, 0);
  std::vector<float> out(5 * 5, 0);
  std::vector<float> tmp(5 * 5, 0);
  in[2 * 5 + 2] = 1.f;

  auto kernel = ggo::build_gaussian_kernel<float>(0.8f, 0.01f);
  GGO_CHECK_EQ(kernel.size(), 3);

  using memory_layout = ggo::lines_memory_layout<ggo::direction::down, sizeof(float)>;
  using data_reader   = ggo::base_data_reader<float>;
  using data_writer   = ggo::base_data_writer<float>;

  // Horizontal pass.
  {
    auto input_line_iterator  = [&](int y) { return memory_layout::make_horizontal_read_only_iterator<data_reader>(in.data(), y, 5, 5 * sizeof(float)); };
    auto output_line_iterator = [&](int y) { return memory_layout::make_horizontal_write_only_iterator<data_writer>(tmp.data(), y, 5, 5 * sizeof(float)); };

    auto left = [&](int x, int y) { 
      GGO_CHECK(x < 0);
      GGO_CHECK(y >= 0 && y < 5);
      return 0.f;
    };
    auto right = [&](int x, int y) {
      GGO_CHECK(x >= 5);
      GGO_CHECK(y >= 0 && y < 5);
      return 0.f;
    };

    ggo::apply_symetric_kernel_2d_horz(input_line_iterator, output_line_iterator, left, right, 5, 5, kernel.data(), kernel.size());
  }

  // Vertical pass.
  {
    auto input_column_iterator  = [&](int x) { return memory_layout::make_vertical_read_only_iterator<data_reader>(tmp.data(), x, 5, 5 * sizeof(float)); };
    auto output_column_iterator = [&](int x) { return memory_layout::make_vertical_write_only_iterator<data_writer>(out.data(), x, 5, 5 * sizeof(float)); };

    auto bottom = [&](int x, int y) { 
      GGO_CHECK(x >= 0 && x < 5);
      GGO_CHECK(y < 0);
      return 0.f;
    };
    auto top = [&](int x, int y) { 
      GGO_CHECK(x >= 0 && x < 5);
      GGO_CHECK(y >= 5);
      return 0.f;
    };

    ggo::apply_symetric_kernel_2d_vert(input_column_iterator, output_column_iterator, bottom, top, 5, 5, kernel.data(), kernel.size());
  }

  const std::vector<float> ref{
    0.000481f, 0.005011f, 0.010945f, 0.005011f, 0.000481f,
    0.005011f, 0.052218f, 0.114054f, 0.052218f, 0.005011f,
    0.010945f, 0.114054f, 0.249117f, 0.114054f, 0.010945f,
    0.005011f, 0.052218f, 0.114054f, 0.052218f, 0.005011f,
    0.000481f, 0.005011f, 0.010945f, 0.005011f, 0.000481f
  };

  for (size_t i = 0; i < out.size(); ++i)
  {
    GGO_CHECK_FLOAT_EQ(out[i], ref[i]);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 2d_8u_16u_zero)
{
  std::vector<uint8_t> in(5 * 5, 0);
  std::vector<uint8_t> out(5 * 5, 0);
  std::vector<uint8_t> tmp(5 * 5, 0);
  in[2*5+2] = 0xff;

  auto kernel = ggo::build_fixed_point_gaussian_kernel<uint16_t, float>(0.8f, 0.01f, 8);
  GGO_CHECK_EQ(kernel.size(), 3);

  using memory_layout = ggo::lines_memory_layout<ggo::direction::down, sizeof(uint8_t)>;
  using data_reader   = ggo::base_data_reader<uint8_t>;
  using data_write    = ggo::fixed_point_data_writer<uint8_t, uint16_t, 8>;

  // Horizontal pass.
  {
    auto input_line_iterator  = [&](int y) { return memory_layout::make_horizontal_read_only_iterator<data_reader>(in.data(),  y, 5, 5 * sizeof(uint8_t)); };
    auto output_line_iterator = [&](int y) { return memory_layout::make_horizontal_write_only_iterator<data_write>(tmp.data(), y, 5, 5 * sizeof(uint8_t)); };

    auto left = [&](int x, int y) {
      GGO_CHECK(x < 0); 
      GGO_CHECK(y >= 0 && y < 5);
      return static_cast<uint16_t>(0);
    };
    auto right = [&](int x, int y) {
      GGO_CHECK(x >= 5);
      GGO_CHECK(y >= 0 && y < 5);
      return static_cast<uint16_t>(0);
    };
    ggo::apply_symetric_kernel_2d_horz(input_line_iterator, output_line_iterator, left, right, 5, 5, kernel.data(), kernel.size());
  }

  // Vertical pass.
  {
    auto input_column_iterator  = [&](int x) { return memory_layout::make_vertical_read_only_iterator<data_reader>(tmp.data(), x, 5, 5 * sizeof(uint8_t)); };
    auto output_column_iterator = [&](int x) { return memory_layout::make_vertical_write_only_iterator<data_write>(out.data(), x, 5, 5 * sizeof(uint8_t)); };

    auto bottom = [&](int x, int y) {
      GGO_CHECK(x >= 0 && x < 5); 
      GGO_CHECK(y < 0);
      return static_cast<uint16_t>(0);
    };
    auto top = [&](int x, int y) {
      GGO_CHECK(x >= 0 && x < 5);
      GGO_CHECK(y >= 5);
      return static_cast<uint16_t>(0);
    };
    ggo::apply_symetric_kernel_2d_vert(input_column_iterator, output_column_iterator, bottom, top, 5, 5, kernel.data(), kernel.size());
  }

  const std::vector<uint8_t> ref{
    0, 1, 3, 1, 0,
    1, 13, 29, 13, 1,
    3, 29, 64, 29, 3,
    1, 13, 29, 13, 1,
    0, 1, 3, 1, 0 
  };

  for (size_t i = 0; i < out.size(); ++i)
  {
    GGO_CHECK_EQ(static_cast<int>(out[i]), static_cast<int>(ref[i]));
  }
}

