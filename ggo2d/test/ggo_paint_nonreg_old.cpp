#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_helpers.h>
#include <ggo_paint.h>
#include <ggo_pixel_sampler_abc.h>
#include <ggo_blur_pixel_sampler.h>
#include <ggo_bmp.h>
#include <ggo_fill.h>
#include <ggo_nonreg.h>
#include <ggo_gradient_brush.h>
#include <ggo_perlin_noise_field_2d.h>
#include <string.h>


////////////////////////////////////////////////////////////////////
class ggo_blur_sampler2 : public ggo::pixel_sampler_abc
{
public:

        ggo_blur_sampler2(float radius, int steps) : _radius(radius), _steps(steps) {}

  float get_horz_extent() const override { return _radius; }
  float get_vert_extent() const override { return _radius; }
  bool  is_pixel_aligned() const override { return false; }
  int   get_samples_count() const override { return _steps * _steps; }
  
  void  sample_pixel(int x, int y, ggo::sample_func fct) const override
  {
    for (int i = 0; i < _steps; ++i)
    {
      for (int j = 0; j < _steps; ++j)
      {
        float angle = i * 2 * ggo::pi<float>() / _steps;
        float dist = j * _radius / _steps;
        ggo::pos2f pos = ggo::from_polar(angle, dist);
        float x_f = x + pos.get<0>();
        float y_f = y + pos.get<1>();
        
        fct(x_f, y_f);
      }
    }
  }
    
private:

  int   _steps;
  float _radius;
};

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, blur)
{
  const int width = 100;
  const int height = 100;
    
  ggo::rgb_image_buffer_uint8 image(width, height, ggo::color::WHITE);
  ggo::fill_checker(image, ggo::color::WHITE, ggo::color::BLACK, 10);
    
  {
    auto disc = std::make_shared<ggo::disc_float>(10.f, 10.f, 30.f);

    const std::vector<ggo::rgb_layer> layers{ { disc, ggo::color::BLUE, 1.f } };

    ggo::paint(image, layers, ggo::blur_pixel_sampler(10, 30));
  }
    
  {
    auto disc1 = std::make_shared<ggo::disc_float>(80.f, 80.f, 30.f);
    auto disc2 = std::make_shared<ggo::disc_float>(80.f, 80.f, 15.f);
        
    const std::vector<ggo::rgb_layer> layers{ {disc1, ggo::color::RED}, {disc2, ggo::color::YELLOW} };
        
    ggo::paint(image, layers, ggo_blur_sampler2(10, 40));
  }
    
  ggo::save_bmp("test_blur.bmp", image.data(), width, height);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, perlin)
{
  ggo::perlin_noise_field2d<float> f;

  float scale = 20;
  float amplitude = 128;
  for (int i = 0; i < 5; ++i)
  {
    float angle = 1.2f * i;
    f.add_octave(scale, amplitude, angle);
    scale *= 0.5f;
    amplitude *= 0.5f;
  }
  
  const int width = 200;
  const int height = 200;
  
  ggo::array_uint8 buffer(3 * width * height);
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < height; ++x)
    {
      float v = f.evaluate(x - 0.5f * width, y - 0.5f * height);
      buffer(3 * (y * width + x) + 0) = ggo::to<uint8_t>(v);
      buffer(3 * (y * width + x) + 1) = ggo::to<uint8_t>(v);
      buffer(3 * (y * width + x) + 2) = ggo::to<uint8_t>(v);
    }
  }
  
  ggo::save_bmp("test_perlin.bmp", buffer.data(), width, height);
}
