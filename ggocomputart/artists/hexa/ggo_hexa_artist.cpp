#include "ggo_hexa_artist.h"
#include <ggo_array.h>
#include <ggo_distance.h>
#include <ggo_perlin_noise_field_2d.h>
#include <ggo_gaussian_field_2d.h>
#include <ggo_best_match.h>
#include <ggo_renderer_abc.h>
#include <ggo_raycaster_abc.h>
#include <ggo_linear_fog.h>
#include <ggo_raytracer_global.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_octree_raycaster.h>
#include <sstream>
#include <map>

//////////////////////////////////////////////////////////////
// HEXA SCALAR FIELD
namespace
{
  struct hexa_line_field : public ggo::scalar_field_2d_abc<float>
  {
    float evaluate(float x, float y) const override
    {
      if (_a * x + _b * y + _c > 0)
      {
        return 0;
      }
      else
      {
        return _height;
      }
    }

    float _a;
    float _b;
    float _c;
    float _height;
  };

  struct gaussian_field : public ggo::gaussian_field2d<float>
  {
    float evaluate(float x, float y) const override
    {
      return _negative ? -ggo::gaussian_field2d<float>::evaluate(x, y) : ggo::gaussian_field2d<float>::evaluate(x, y);
    }

    bool _negative;
  };

  class hexa_scalar_field : public ggo::scalar_field_2d_abc<float>
  {
  public:

    hexa_scalar_field()
    {
      // First the Perlin noise.
      auto perlin_field = std::make_shared<ggo::perlin_noise_field2d<float>>();

      float scale = 50;
      float amplitude = 5;
      for (int i = 0; i < 10; ++i)
      {
        perlin_field->add_octave(scale, amplitude, ggo::rand<float>(0, ggo::pi<float>()));

        scale *= 0.9f;
        amplitude *= 0.9f;
      }

      _fields.push_back(perlin_field);

      // The gaussians.
      std::vector<ggo::pos2f> centers;
      float ratio = ggo::rand<float>(5, 10);
      float sum = 0;
      do
      {
        float dist = 0;
        gaussian_field gaussian2d;

        for (int i = 0; i < 64; ++i)
        {
          gaussian_field gaussian2d_cur;

          gaussian2d_cur._x = ggo::rand<float>(-200, 200);
          gaussian2d_cur._y = ggo::rand<float>(-250, 250);
          gaussian2d_cur._amp = ggo::rand<float>(10, 50);
          gaussian2d_cur._var = ratio * gaussian2d_cur._amp;
          gaussian2d_cur._negative = ggo::rand_bool();

          float dist_cur = std::numeric_limits<float>::max();
          for (const auto & center : centers)
          {
            dist_cur = std::min(dist_cur, ggo::distance(center, { gaussian2d_cur._x, gaussian2d_cur._y }));
          }

          if (dist_cur > dist)
          {
            gaussian2d = gaussian2d_cur;
            dist = dist_cur;
          }
        }

        _fields.push_back(std::make_shared<gaussian_field>(gaussian2d));

        sum += gaussian2d._amp * gaussian2d._var;
        centers.push_back({ gaussian2d._x, gaussian2d._y });
      } while (sum < 150000);

      // Line.
      sum = 0;
      do
      {
        auto line_field = std::make_shared<hexa_line_field>();
        line_field->_a = ggo::rand<float>(-1, 1);
        line_field->_b = ggo::rand<float>(-1, 1);
        line_field->_c = ggo::rand<float>(-50, 50);
        line_field->_height = ggo::rand<float>(-2, 2);
        _fields.push_back(line_field);

        sum += std::abs(line_field->_height);
      } while (sum < 20);
    }

    float evaluate(float x, float y) const override
    {
      float z = 0;

      for (const auto & field : _fields)
      {
        z += field->evaluate(x, y);
      }

      return z;
    }

  private:

    std::vector<std::shared_ptr<ggo::scalar_field_2d_abc<float>>> _fields;
  };
}

//////////////////////////////////////////////////////////////
// HEXA
namespace
{
  void create_hexa(float x, float y, float height, ggo::scene & scene, const ggo::color_32f & color_top, const ggo::color_32f & color_side)
  {
    auto create_triangle = [](const ggo::pos3f & v1, const ggo::pos3f & v2, const ggo::pos3f & v3, ggo::scene & scene, const ggo::color_32f & color)
    {
      ggo::triangle3d<float, true> triangle(v1, v2, v3);
      constexpr uint32_t flags = ggo::discard_all;
      const auto & object = scene.add_diffuse_object<flags, ggo::triangle3d<float, true>, ggo::solid_color_material>(triangle, ggo::solid_color_material(color));
    };

    const float hexa_height = 15;
    float z_inf = height - hexa_height;
    float z_sup = height;

    for (int i = 0; i < 6; ++i)
    {
      float angle1 = i * ggo::pi<float>() / 3;
      float angle2 = (i + 1) * ggo::pi<float>() / 3;

      create_triangle(ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_sup),
        ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_sup),
        ggo::pos3f(x, y, z_sup), scene, color_top);

      create_triangle(ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_sup),
        ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_sup),
        ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_inf), scene, color_side);

      create_triangle(ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_inf),
        ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_sup),
        ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_inf), scene, color_side);
    }
  }
}

//////////////////////////////////////////////////////////////
// HEXA ARTIST
std::pair<ggo::color_32f, ggo::color_32f> ggo::hexa_artist::generate_colors()
{
  float hue1 = ggo::rand<float>();
  auto candidate_func = []() { return ggo::rand<float>(); };
  auto evaluate_func = [&](float hue) { return ggo::min(std::abs(hue1 - hue), std::abs(hue1 + 1 - hue), std::abs(hue1 - hue - 1)); };
  float hue2 = ggo::best_match_max<float>(candidate_func, evaluate_func, 5); // We want a second hue far from the first one.
  float sat1 = ggo::rand<float>(0.75, 1);
  float sat2 = ggo::rand<float>(0.75, 1);
  float val1 = 0.5f + 0.5f * sat1;
  float val2 = 0.5f + 0.5f * sat2;
  
  return std::make_pair(2.5f * ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), sat1, val1),
                        2.5f * ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), sat2, val2));
}

//////////////////////////////////////////////////////////////
std::vector<ggo::pos3f> ggo::hexa_artist::generate_light_positions()
{
  std::vector<ggo::pos3f> pos;
  
  for (int i = 0; i < 3; ++i)
  {
    float angle = ggo::rand<float>(0, 2 * ggo::pi<float>());
    pos.push_back( { 500 * std::cos(angle), 500 * std::sin(angle), 250.f } );
  }
  
  return pos;
}

//////////////////////////////////////////////////////////////    
std::vector<ggo::hexa_artist::hexa_info> ggo::hexa_artist::generate_hexa_infos()
{
  std::vector<hexa_info> hexas_infos;
  
  hexa_scalar_field scalar_field;
  
  for (int x = -60; x < 60; ++x)
  {
    for (int y = -140; y < 140; ++y)
    {
      float x1 = 3.f * x;
      float y1 = y * std::sqrt(3.f);
      float height1 = scalar_field.evaluate(x1, y1);

      float x2 = 3 * x + 1.5f;
      float y2 = (2 * y + 1) * std::sqrt(3.f) / 2;
      float height2 = scalar_field.evaluate(x2, y2);

      hexas_infos.push_back({x1, y1, height1});
      hexas_infos.push_back({x2, y2, height2});
    }
  }
  
  return hexas_infos;
}

//////////////////////////////////////////////////////////////
void ggo::hexa_artist::render(void * buffer, int width, int height, int line_step, ggo::pixel_buffer_format pbf,
                              const std::vector<hexa_info> & hexa_infos,
                              const ggo::color_32f & color_top, const ggo::color_32f & color_side,
                              const std::vector<ggo::pos3f> & lights_pos,
                              const ggo::color_32f & fog_color,
                              ggo::renderer_abc & renderer)
{
  ggo::scene scene(std::make_shared<ggo::background3d_color>(fog_color));

  // Objects.
  for (const auto & hexa_info : hexa_infos)
  {
    create_hexa(hexa_info._x, hexa_info._y, hexa_info._height, scene, color_top, color_side);
  }

  // Add lights to scene.
  for (const auto & light_pos : lights_pos)
  {
    scene.add_sphere_light(ggo::white<ggo::color_32f>(), light_pos, 100.f);
  }

  // Fog and ambient color.
  scene.set_fog(std::make_shared<ggo::linear_fog>(fog_color, 1000.f));
  scene.set_ambient_color(ggo::color_32f(0.1f));

  // Raytracer parameters.
  ggo::octree_raycaster raycaster(scene);
  ggo::raytrace_params raytrace_params;
  raytrace_params._raycaster = &raycaster;

  // Rendering.
  renderer.render(buffer, width, height, line_step, pbf, scene, raytrace_params);
}