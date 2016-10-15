//#define GGO_GAUSSIAN_DEBUG 1

#include <ggo_nonreg.h>
#include <ggo_gaussian_blur.h>

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
    GGO_CHECK_FABS(compute_norm(kernel), 1.f);
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

  std::vector<std::pair<float, float>> stddev_thresholds{ { 1.f, 0.01f },{ 2.f, 0.01f },{ 5.f, 0.01f },{ 5.f, 0.0001f } };

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

  auto left   = [&](int x) { return 0.f; };
  auto center = [&](int x) { return in[x]; };
  auto right  = [&](int x) { return 0.f; };
  auto output = [&](int x, float v) { out[x] = v; };

  ggo::apply_symetric_kernel_1d(left, center, right, output, 7, kernel.data(), kernel.size());

  GGO_CHECK_FABS(out[0], 0.f);
  GGO_CHECK_FABS(out[1], 0.0219296496f);
  GGO_CHECK_FABS(out[2], 0.228512138f);
  GGO_CHECK_FABS(out[3], 0.499116361f);
  GGO_CHECK_FABS(out[4], 0.228512138f);
  GGO_CHECK_FABS(out[5], 0.0219296496f);
  GGO_CHECK_FABS(out[6], 0.f);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 1d_8u_32f_zero)
{
  const uint8_t in[7] = { 0, 0, 0, 255, 0, 0, 0 };
  uint8_t out[7] = { 0, 0, 0, 0, 0, 0, 0 };

  auto kernel = ggo::build_gaussian_kernel<float>(0.8f, 0.01f);
  GGO_CHECK_EQ(kernel.size(), 3);

  auto left   = [&](int x) { return 0.f; };
  auto center = [&](int x) { return ggo::to<float>(in[x]); };
  auto right  = [&](int x) { return 0.f; };
  auto output = [&](int x, float v) { out[x] = ggo::to<uint8_t>(v); };

  ggo::apply_symetric_kernel_1d(left, center, right, output, 7, kernel.data(), kernel.size());

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

  auto left   = [&](int x) { return static_cast<uint16_t>(0); };
  auto center = [&](int x) { return static_cast<uint16_t>(in[x]); };
  auto right  = [&](int x) { return static_cast<uint16_t>(0); };
  auto output = [&](int x, uint16_t v) { out[x] = (v + (1 << 7)) >> 8; };

  ggo::apply_symetric_kernel_1d(left, center, right, output, 7, kernel.data(), kernel.size());

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

  auto left   = [&](int x) { return static_cast<uint16_t>(ggo::get1d_mirror(in, x, 9)); };
  auto center = [&](int x) { return static_cast<uint16_t>(in[x]); };
  auto right  = [&](int x) { return static_cast<uint16_t>(ggo::get1d_mirror(in, x, 9)); };
  auto output = [&](int x, uint16_t v) { out[x] = (v + (1 << 7)) >> 8; };

  ggo::apply_symetric_kernel_1d(left, center, right, output, 9, kernel.data(), kernel.size());

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

  auto read = [&](const void * ptr) { return *static_cast<const float *>(ptr); };
  auto write = [&](void * ptr, float v) { float * ptr_32f = static_cast<float *>(ptr); *ptr_32f = v; };

  // Horizontal pass.
  {
    auto left   = [&](int x, int y) { return 0.f; };
    auto right  = [&](int x, int y) { return 0.f; };
    ggo::apply_symetric_kernel_2d_horz(in.data(), sizeof(float), 5 * sizeof(float), read,
      tmp.data(), sizeof(float), 5 * sizeof(float), write,
      left, right, 5, 5, kernel.data(), kernel.size());
  }

  // Vertical pass.
  {
    auto bottom = [&](int x, int y) { return 0.f; };
    auto top    = [&](int x, int y) { return 0.f; };
    ggo::apply_symetric_kernel_2d_vert<ggo::y_up>(tmp.data(), sizeof(float), 5 * sizeof(float), read,
      out.data(), sizeof(float), 5 * sizeof(float), write,
      bottom, top, 5, 5, kernel.data(), kernel.size());
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
    GGO_CHECK_FABS(out[i], ref[i]);
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

  auto read = [&](const void * ptr) {
    uint8_t v = *static_cast<const uint8_t *>(ptr);
    return static_cast<uint16_t>(v);
  };
  auto write = [&](void * ptr, uint16_t v) { 
    uint8_t * ptr_8u = static_cast<uint8_t *>(ptr); 
    *ptr_8u = static_cast<uint8_t>(ggo::fixed_point_div<8>(v));
  };

  // Horizontal pass.
  {
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
    ggo::apply_symetric_kernel_2d_horz(in.data(), 1, 5, read,
      tmp.data(), 1, 5, write, 
      left, right, 5, 5, kernel.data(), kernel.size());
  }

  // Vertical pass.
  {
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
    ggo::apply_symetric_kernel_2d_vert<ggo::y_up>(tmp.data(), 1, 5, read,
      out.data(), 1, 5, write,
      bottom, top, 5, 5, kernel.data(), kernel.size());
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
