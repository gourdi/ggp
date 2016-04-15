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
GGO_TEST(paint, multi_paint1)
{
  const int WIDTH = 100;
  const int HEIGHT = 100;
    
  auto disc1 = std::make_shared<ggo::disc_float>(ggo::point2d_float(40.f, 50.f), 30.f);
  auto disc2 = std::make_shared<ggo::disc_float>(ggo::point2d_float(60.f, 50.f), 30.f);
  auto multi_shape = std::make_shared<ggo::multi_shape_float>();
  multi_shape->add_shapes(disc1, disc2);

  ggo::array_uint8 buffer(WIDTH * HEIGHT * 3);
  buffer.fill(0);
  ggo::paint(buffer, WIDTH, HEIGHT, multi_shape, ggo::color::RED, 0.5, std::make_shared<ggo::rgb_alpha_blender>(), ggo::pixel_sampler_4X4());

  ggo::save_bmp("test_multi1.bmp", buffer, WIDTH, HEIGHT);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, multi_paint2)
{
  const int WIDTH = 100;
  const int HEIGHT = 100;

  auto polygon1 = std::make_shared<ggo::polygon2d_float>();
  polygon1->add_point(10, 10);
  polygon1->add_point(50, 10);
  polygon1->add_point(50, 90);
  polygon1->add_point(10, 90);

  auto polygon2 = std::make_shared<ggo::polygon2d_float>();
  polygon2->add_point(90, 10);
  polygon2->add_point(50, 10);
  polygon2->add_point(50, 90);
  polygon2->add_point(90, 90);

  const std::vector<ggo::rgb_layer> layers{ {polygon1, ggo::color::WHITE}, {polygon2, ggo::color::WHITE} };

  ggo::array_uint8 buffer(WIDTH * HEIGHT * 3);
  ggo::fill_solid_rgb(buffer, WIDTH * HEIGHT, ggo::color::BLUE);
  ggo::paint(buffer, WIDTH, HEIGHT, layers, ggo::pixel_sampler_16X16());

  ggo::save_bmp("test_multi2.bmp", buffer, WIDTH, HEIGHT);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, multi_paint3)
{
  const int WIDTH = 60;
  const int HEIGHT = 60;
    
  const std::vector<ggo::gray_layer> layers{ {std::make_shared<ggo::disc_float>(0.5f * WIDTH, 0.5f * HEIGHT, 20.f), 0.8f, 1} };

  ggo::gray_image_buffer_float image_data(WIDTH, HEIGHT, 0.f);
  ggo::paint(image_data, layers);
    
  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 0);
  for (int i = 0; i < WIDTH * HEIGHT; ++i)
  {
    buffer[3 * i + 0] = ggo::to<uint8_t>(255.f * image_data[i]);
    buffer[3 * i + 1] = ggo::to<uint8_t>(255.f * image_data[i]);
    buffer[3 * i + 2] = ggo::to<uint8_t>(255.f * image_data[i]);
  }

  ggo::save_bmp("test_multi3.bmp", buffer, WIDTH, HEIGHT);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, multi_paint4)
{
  const int WIDTH = 60;
  const int HEIGHT = 60;
    
  auto segment = std::make_shared<ggo::extended_segment_float>(ggo::point2d_float(10, 10), ggo::point2d_float(50, 15), 0.48f);

  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 255);
  ggo::paint(buffer, WIDTH, HEIGHT, segment, ggo::color::RED, 1, std::make_shared<ggo::rgb_alpha_blender>(), ggo::pixel_sampler_8X8());

  ggo::save_bmp("test_multi4.bmp", buffer, WIDTH, HEIGHT);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, multi_paint5)
{
  const int WIDTH = 100;
  const int HEIGHT = 100;

  auto color_brush = std::make_shared<ggo::rgb_gradient_brush>();
  color_brush->_value1 = ggo::color::RED;
  color_brush->_value2 = ggo::color::GREEN;
  color_brush->_pos1 = ggo::point2d_float(30, 30);
  color_brush->_pos2 = ggo::point2d_float(50, 50);
  
  auto opacity_brush = std::make_shared<ggo::opacity_solid_brush>(1.f);

  auto disc = std::make_shared<ggo::disc_float>(50.f, 50.f, 40.f);
    
  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 255);
  ggo::paint(buffer, WIDTH, HEIGHT, disc, color_brush, opacity_brush, std::make_shared<ggo::rgb_alpha_blender>(), ggo::pixel_sampler_8X8());

  ggo::save_bmp("test_multi5.bmp", buffer, WIDTH, HEIGHT);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, multi_paint6)
{
  const int WIDTH = 100;
  const int HEIGHT = 100;

  auto shape = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::DIFFERENCE>>();
  shape->add_shape(std::make_shared<ggo::disc_float>(50.f, 50.f, 40.f));
  shape->add_shape(std::make_shared<ggo::disc_float>(70.f, 70.f, 30.f));
    
  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 128);
  ggo::paint(buffer, WIDTH, HEIGHT, shape, ggo::color::RED);

  ggo::save_bmp("test_multi6.bmp", buffer, WIDTH, HEIGHT);
}

////////////////////////////////////////////////////////////////////
GGO_TEST(paint, multi_paint7)
{
  const int WIDTH = 100;
  const int HEIGHT = 100;

  auto shape = std::make_shared<ggo::multi_shape<float, ggo::boolean_mode::INTERSECTION>>();
  shape->add_shape(std::make_shared<ggo::disc_float>(50.f, 50.f, 40.f));
  shape->add_shape(std::make_shared<ggo::disc_float>(70.f, 70.f, 30.f));

  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT, 0);
  ggo::paint(buffer, WIDTH, HEIGHT, shape, ggo::color::YELLOW);

  ggo::save_bmp("test_multi7.bmp", buffer, WIDTH, HEIGHT);
}

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
        float angle = i * 2 * ggo::PI<float>() / _steps;
        float dist = j * _radius / _steps;
        ggo::point2d_float pos = ggo::point2d_float::from_polar(angle, dist);
        float x_f = x + pos.x();
        float y_f = y + pos.y();
        
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
  const int WIDTH = 100;
  const int HEIGHT = 100;
    
  ggo::rgb_image_buffer_uint8 image(WIDTH, HEIGHT, ggo::color::WHITE);
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
    
  ggo::save_bmp("test_blur.bmp", image.get_buffer(), WIDTH, HEIGHT);
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
  
  const int WIDTH = 200;
  const int HEIGHT = 200;
  
  ggo::array_uint8 buffer(3 * WIDTH * HEIGHT);
  for (int y = 0; y < HEIGHT; ++y)
  {
    for (int x = 0; x < HEIGHT; ++x)
    {
      float v = f.evaluate(x - 0.5f * WIDTH, y - 0.5f * HEIGHT);
      buffer[3 * (y * WIDTH + x) + 0] = ggo::to<uint8_t>(v);
      buffer[3 * (y * WIDTH + x) + 1] = ggo::to<uint8_t>(v);
      buffer[3 * (y * WIDTH + x) + 2] = ggo::to<uint8_t>(v);
    }
  }
  
  ggo::save_bmp("test_perlin.bmp", buffer, WIDTH, HEIGHT);
}
