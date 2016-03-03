//#define GGO_GAUSSIAN_DEBUG 1

#include <ggo_nonreg.h>
#include <ggo_gaussian_blur.h>
#include <complex>

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, filter)
{
  auto compute_norm = [](const std::vector<float> & filter)
  {
    auto it = filter.begin();
    float norm = *it++;
    for (; it != filter.end(); ++it)
    {
      norm += 2 * (*it);
    }
    return norm;
  };

  std::vector<std::pair<float, float>> stddev_threasholds{ { 1.f, 0.01f }, { 2.f, 0.01f }, { 5.f, 0.01f }, { 5.f, 0.0001f } };

  for (const auto & stddev_threashold : stddev_threasholds)
  {
    auto filter = ggo::build_gaussian_filter(stddev_threashold.first, stddev_threashold.second);
    GGO_CHECK_FABS(compute_norm(filter), 1.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 1d)
{
  // uint8_t
  {
    std::vector<uint8_t> in(7, 0);
    std::vector<uint8_t> out(7, 0);
    in[3] = 0xff;
    ggo::gaussian_blur_1d_uint8(&in[0], &out[0], static_cast<int>(in.size()), 0.8f);
    GGO_CHECK(out[0] == 0);
    GGO_CHECK(out[1] == 6);
    GGO_CHECK(out[2] == 58);
    GGO_CHECK(out[3] == 127);
    GGO_CHECK(out[4] == 58);
    GGO_CHECK(out[5] == 6);
    GGO_CHECK(out[6] == 0);
  }

  // float
  {
    std::vector<float> in(7, 0);
    std::vector<float> out(7, 0);
    in[3] = 255.f;
    ggo::gaussian_blur_1d(&in[0], &out[0], static_cast<int>(in.size()), 0.8f);
    GGO_CHECK_FABS(out[0], 0.f);
    GGO_CHECK_FABS(out[1], 5.59206057f);
    GGO_CHECK_FABS(out[2], 58.2705956f);
    GGO_CHECK_FABS(out[3], 127.274673f);
    GGO_CHECK_FABS(out[4], 58.2705956f);
    GGO_CHECK_FABS(out[5], 5.59206057f);
    GGO_CHECK_FABS(out[6], 0.f);
  }

  // std::complex<double>
  {
    std::vector<std::complex<double>> in(7, 0);
    std::vector<std::complex<double>> out(7, 0);
    in[3] = { 1.0, 2.0 };
    ggo::gaussian_blur_1d<std::complex<double>, double, std::complex<double>, std::complex<double>>(&in[0], &out[0], static_cast<int>(in.size()), 0.8f);
    GGO_CHECK_FABS(out[0].real(), 0.f);
    GGO_CHECK_FABS(out[0].imag(), 0.f);
    GGO_CHECK_FABS(out[1].real(), 0.021929646581818646);
    GGO_CHECK_FABS(out[1].imag(), 0.043859293163637292);
    GGO_CHECK_FABS(out[2].real(), 0.22851214883994622);
    GGO_CHECK_FABS(out[2].imag(), 0.45702429767989244);
    GGO_CHECK_FABS(out[3].real(), 0.49911640915647038);
    GGO_CHECK_FABS(out[3].imag(), 0.99823281831294075);
    GGO_CHECK_FABS(out[4].real(), 0.22851214883994622);
    GGO_CHECK_FABS(out[4].imag(), 0.45702429767989244);
    GGO_CHECK_FABS(out[5].real(), 0.021929646581818646);
    GGO_CHECK_FABS(out[5].imag(), 0.043859293163637292);
    GGO_CHECK_FABS(out[6].real(), 0.f);
    GGO_CHECK_FABS(out[6].imag(), 0.f);
  }
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(gaussian_blur, 2d)
{
  // uint8_t
  {
    std::vector<uint8_t> in(5 * 5, 0);
    std::vector<uint8_t> out(5 * 5, 0);
    in[2*5+2] = 0xff;
    ggo::gaussian_blur_2d_uint8(&in[0], &out[0], 5, 5, 0.8f);

    const std::vector<uint8_t> ref{
      0, 1, 3, 1, 0,
      1, 13, 29, 13, 1,
      3, 29, 64, 29, 3,
      1, 13, 29, 13, 1,
      0, 1, 3, 1, 0 
    };

    for (size_t i = 0; i < out.size(); ++i)
    {
      GGO_CHECK(out[i] == ref[i]);
    }
  }
}

