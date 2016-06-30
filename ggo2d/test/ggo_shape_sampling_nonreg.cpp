#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_vec.h>
#include <ggo_set3.h>
#include <ggo_shape_sampling.h>
#include <ggo_poisson_sampling.h>
#include <ggo_paint.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_bmp.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, disc_uniform_sampling)
{
  const int SIZE = 500;
  ggo::array_uint8 buffer(SIZE * SIZE * 3, 0);
  
  for (int i = 0; i < (1<<14); ++i)
  {
    auto sample = ggo::disc_uniform_sampling<float>();
    
    int x = ggo::to<int>(0.4 * SIZE * sample.get<0>() + SIZE / 2);
    int y = ggo::to<int>(0.4 * SIZE * sample.get<1>() + SIZE / 2);
    
    uint8_t * ptr = buffer.data() + 3 * (y * SIZE + x);
    ptr[0] = 0xFF;
    ptr[1] = 0xFF;
    ptr[2] = 0xFF;
  }
  
  ggo::save_bmp("test_disc_uniform_sampling.bmp", buffer.data(), SIZE, SIZE);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, disc_poisson_sampling)
{
  const int SIZE = 500;
  const ggo::pos2f CENTER(float(SIZE) / 2, float(SIZE) / 2);
  
  auto samples = ggo::poisson_sampling<ggo::pos2f, float>(
    [&]()
    {
      auto sample = ggo::disc_uniform_sampling<float>();
      return CENTER + sample * float(SIZE) * 0.4f;
    },
    [](const ggo::pos2f & p1, const ggo::pos2f & p2)
    {
      return ggo::distance(p1, p2);
    },
    8, 1 << 10);
  
  ggo::array_uint8 buffer(SIZE * SIZE * 3, 0);

  for (const auto & sample : samples)
  {
    auto disc = std::make_shared<ggo::disc_float>(sample, 2.f);
    ggo::paint(buffer.data(), SIZE, SIZE, disc, ggo::color::WHITE);
  }
  
  ggo::save_bmp("test_disc_poisson_sampling.bmp", buffer.data(), SIZE, SIZE);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, hemisphere_uniform_sampling)
{
  const float ANGLE = 1.2f;
  const int SIZE = 1000;
  ggo::array_uint8 buffer(SIZE * SIZE * 3, 0);
  
  for (int i = 0; i < 1 << 10; ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>();
    
    float x_f = sample.x();
    float y_f = sample.y() * std::cos(ANGLE) + sample.z() * std::sin(ANGLE);
    
    x_f = 0.2f * SIZE * x_f + SIZE / 4.f;
    y_f = 0.2f * SIZE * y_f + SIZE / 4.f;
    
    auto disc = std::make_shared<ggo::disc_float>(x_f, y_f, 2.f);
    ggo::paint(buffer.data(), SIZE, SIZE, disc, ggo::color::WHITE);
  }
  
  for (int i = 0; i < (1<<20); ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>();
    
    float x_f = sample.x();
    float y_f = sample.y();
    
    int x_i = ggo::to<int>(0.2 * SIZE * x_f + 3 * SIZE / 4);
    int y_i = ggo::to<int>(0.2 * SIZE * y_f + SIZE / 4);
    
    uint8_t * ptr = buffer.data() + 3 * (y_i * SIZE + x_i);
    ptr[0] = std::min<int>(ptr[0] + 8, 0xFF);
    ptr[1] = std::min<int>(ptr[1] + 8, 0xFF);
    ptr[2] = std::min<int>(ptr[2] + 8, 0xFF);
  }
  
  for (int i = 0; i < (1<<20); ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>();
    
    float x_f = sample.x();
    float y_f = sample.y() * std::cos(ANGLE) + sample.z() * std::sin(ANGLE);
    
    int x_i = ggo::to<int>(0.2 * SIZE * x_f + SIZE / 4);
    int y_i = ggo::to<int>(0.2 * SIZE * y_f + 3 * SIZE / 4);
    
    uint8_t * ptr = buffer.data() + 3 * (y_i * SIZE + x_i);
    ptr[0] = std::min<int>(ptr[0] + 8, 0xFF);
    ptr[1] = std::min<int>(ptr[1] + 8, 0xFF);
    ptr[2] = std::min<int>(ptr[2] + 8, 0xFF);
  }
  
  for (int i = 0; i < (1<<18); ++i)
  {
    auto sample = ggo::hemisphere_uniform_sampling<float>(ggo::PI<float>() / 4);
    
    float x_f = sample.x();
    float y_f = sample.y() * std::cos(ANGLE) + sample.z() * std::sin(ANGLE);
    
    int x_i = ggo::to<int>(0.2 * SIZE * x_f + 3 * SIZE / 4);
    int y_i = ggo::to<int>(0.2 * SIZE * y_f + 3 * SIZE / 4);
    
    uint8_t * ptr = buffer.data() + 3 * (y_i * SIZE + x_i);
    ptr[0] = std::min<int>(ptr[0] + 8, 0xFF);
    ptr[1] = std::min<int>(ptr[1] + 8, 0xFF);
    ptr[2] = std::min<int>(ptr[2] + 8, 0xFF);
  }
  
  ggo::save_bmp("test_hemisphere_uniform_sampling.bmp", buffer.data(), SIZE, SIZE);
}

/////////////////////////////////////////////////////////////////////
GGO_TEST(shape_sampling, hemisphere_poisson_sampling)
{
  const int SIZE = 500;
  const float ANGLE = 1.2f;
  
  auto samples = ggo::poisson_sampling<ggo::point3d_float, float>(
    []()
    {
        return ggo::hemisphere_uniform_sampling<float>();
    },
    [](const ggo::point3d_float & p1, const ggo::point3d_float & p2)
    {
        return ggo::distance(p1, p2);
    },
    0.05f, 1 << 10);
  
  ggo::array_uint8 buffer(SIZE * SIZE * 3, 0);
  
  for (const auto & sample : samples)
  {
    float x_f = sample.x();
    float y_f = sample.y() * std::cos(ANGLE) + sample.z() * std::sin(ANGLE);
    
    x_f = 0.4f * SIZE * x_f + SIZE / 2;
    y_f = 0.4f * SIZE * y_f + SIZE / 2;
    
    auto disc = std::make_shared<ggo::disc_float>(x_f, y_f, 2.f);
    ggo::paint(buffer.data(), SIZE, SIZE, disc, ggo::color::WHITE);
  }
  
  ggo::save_bmp("test_hemisphere_poisson_sampling.bmp", buffer.data(), SIZE, SIZE);
}
