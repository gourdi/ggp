#include <ggo_nonreg.h>
#include <ggo_shape_sampling.h>
#include <ggo_buffer.h>
#include <ggo_poisson_sampling.h>
#include <ggo_bmp.h>
#include <ggo_buffer_paint.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, disc_uniform_sampling)
{
  const int size = 500;
  ggo::buffer8u buffer(size * size * 3, 0);
  
  for (int i = 0; i < (1<<14); ++i)
  {
    auto sample = ggo::disc_uniform_sampling<float>();
    
    int x = ggo::round_to<int>(0.4 * size * sample.get<0>() + size / 2);
    int y = ggo::round_to<int>(0.4 * size * sample.get<1>() + size / 2);
    
    uint8_t * ptr = buffer.data() + 3 * (y * size + x);
    ptr[0] = 0xFF;
    ptr[1] = 0xFF;
    ptr[2] = 0xFF;
  }
  
  ggo::save_bmp("test_disc_uniform_sampling.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, disc_poisson_sampling)
{
  const int size = 500;
  const ggo::pos2f center(float(size) / 2, float(size) / 2);
  
  auto samples = ggo::poisson_sampling<ggo::pos2f, float>(
    [&]()
    {
      auto sample = ggo::disc_uniform_sampling<float>();
      return center + sample * float(size) * 0.4f;
    },
    [](const ggo::pos2f & p1, const ggo::pos2f & p2)
    {
      return ggo::distance(p1, p2);
    },
    8, 1 << 10);
  
  ggo::buffer8u buffer(size * size * 3, 0);

  for (const auto & sample : samples)
  {
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_8x8>(buffer.data(), size, size, 3 * size, ggo::disc_float(sample, 2.f), ggo::white_8u());
  }
  
  ggo::save_bmp("test_disc_poisson_sampling.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, hemisphere_uniform_sampling)
{
  const float angle = 1.2f;
  const int size = 1000;
  ggo::buffer8u buffer(size * size * 3, 0);
  
  for (int i = 0; i < 1 << 10; ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>();
    
    float x_f = sample.x();
    float y_f = sample.y() * std::cos(angle) + sample.z() * std::sin(angle);
    
    x_f = 0.2f * size * x_f + size / 4.f;
    y_f = 0.2f * size * y_f + size / 4.f;
    
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_16x16>(buffer.data(), size, size, 3 * size, ggo::disc_float(x_f, y_f, 2.f), ggo::white_8u());
  }
  
  for (int i = 0; i < (1<<20); ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>();
    
    float x_f = sample.get<0>();
    float y_f = sample.get<1>();
    
    int x_i = ggo::round_to<int>(0.2 * size * x_f + 3 * size / 4);
    int y_i = ggo::round_to<int>(0.2 * size * y_f + size / 4);
    
    uint8_t * ptr = buffer.data() + 3 * (y_i * size + x_i);
    ptr[0] = std::min<int>(ptr[0] + 8, 0xFF);
    ptr[1] = std::min<int>(ptr[1] + 8, 0xFF);
    ptr[2] = std::min<int>(ptr[2] + 8, 0xFF);
  }
  
  for (int i = 0; i < (1<<20); ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>();
    
    float x_f = sample.get<0>();
    float y_f = sample.get<1>() * std::cos(angle) + sample.get<2>() * std::sin(angle);
    
    int x_i = ggo::round_to<int>(0.2 * size * x_f + size / 4);
    int y_i = ggo::round_to<int>(0.2 * size * y_f + 3 * size / 4);
    
    uint8_t * ptr = buffer.data() + 3 * (y_i * size + x_i);
    ptr[0] = std::min<int>(ptr[0] + 8, 0xFF);
    ptr[1] = std::min<int>(ptr[1] + 8, 0xFF);
    ptr[2] = std::min<int>(ptr[2] + 8, 0xFF);
  }
  
  for (int i = 0; i < (1<<18); ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>(ggo::pi<float>() / 4);
    
    float x_f = sample.get<0>();
    float y_f = sample.get<1>() * std::cos(angle) + sample.get<2>() * std::sin(angle);
    
    int x_i = ggo::round_to<int>(0.2 * size * x_f + 3 * size / 4);
    int y_i = ggo::round_to<int>(0.2 * size * y_f + 3 * size / 4);
    
    uint8_t * ptr = buffer.data() + 3 * (y_i * size + x_i);
    ptr[0] = std::min<int>(ptr[0] + 8, 0xFF);
    ptr[1] = std::min<int>(ptr[1] + 8, 0xFF);
    ptr[2] = std::min<int>(ptr[2] + 8, 0xFF);
  }
  
  ggo::save_bmp("test_hemisphere_uniform_sampling.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, hemisphere_poisson_sampling)
{
  const int size = 500;
  const float angle = 1.2f;
  
  auto samples = ggo::poisson_sampling<ggo::pos3f, float>(
    []()
    {
        return ggo::hemisphere_uniform_sampling<float>();
    },
    [](const ggo::pos3f & p1, const ggo::pos3f & p2)
    {
        return ggo::distance(p1, p2);
    },
    0.05f, 1 << 10);
  
  ggo::buffer8u buffer(size * size * 3, 0);
  
  for (const auto & sample : samples)
  {
    float x_f = sample.x();
    float y_f = sample.y() * std::cos(angle) + sample.z() * std::sin(angle);
    
    x_f = 0.4f * size * x_f + size / 2;
    y_f = 0.4f * size * y_f + size / 2;
    
    ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_16x16>(buffer.data(), size, size, 3 * size, ggo::disc_float(x_f, y_f, 2.f), ggo::white_8u());
  }
  
  ggo::save_bmp("test_hemisphere_poisson_sampling.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}

