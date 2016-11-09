#include "ggo_hexa_artist.h"
#include <ggo_array.h>
#include <ggo_distance.h>
#include <ggo_perlin_noise_field_2d.h>
#include <ggo_gaussian_field_2d.h>
#include <ggo_best_match.h>
#include <ggo_object3d.h>
#include <ggo_renderer_abc.h>
#include <ggo_raycaster_abc.h>
#include <ggo_linear_fog.h>
#include <ggo_raytracer_global.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_brute_force_raycaster.h>
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
  class hexa
  {
  public:

    hexa() = delete;
    hexa(const hexa & hexa) = delete;
    hexa & operator=(const hexa & hexa) = delete;

    hexa(float x, float y, float height,
      ggo::scene_builder & scene_builder,
      std::shared_ptr<const ggo::material_abc> material_top,
      std::shared_ptr<const ggo::material_abc> material_side)
    {
      const float hexa_height = 15;
      float z_inf = height - hexa_height;
      float z_sup = height;

      _bounding_box.x_min() = x;
      _bounding_box.x_max() = x;
      _bounding_box.y_min() = y;
      _bounding_box.y_max() = y;
      _bounding_box.z_min() = z_inf;
      _bounding_box.z_max() = z_sup;

      for (int i = 0; i < 6; ++i)
      {
        float angle1 = i * ggo::pi<float>() / 3;
        float angle2 = (i + 1) * ggo::pi<float>() / 3;

        create_triangle(ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_sup),
          ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_sup),
          ggo::pos3f(x, y, z_sup), scene_builder, material_top);

        /*  create_rectangle(ggo::pos2f(x + std::cos(angle1), y + std::sin(angle1)),
                           ggo::pos2f(x + std::cos(angle2), y + std::sin(angle2)),
                           z_inf, z_sup, scene, material_side);*/

        create_triangle(ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_sup),
          ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_sup),
          ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_inf), scene_builder, material_side);

        create_triangle(ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_inf),
          ggo::pos3f(x + std::cos(angle1), y + std::sin(angle1), z_sup),
          ggo::pos3f(x + std::cos(angle2), y + std::sin(angle2), z_inf), scene_builder, material_side);
      }

      _brute_force_raycaster.reset(new ggo::brute_force_raycaster(_objects));
    }

    const ggo::object3d * intersect_ray(const ggo::ray3d_float & ray,
      float & dist,
      ggo::ray3d_float & normal,
      const ggo::object3d * exclude_object1,
      const ggo::object3d * exclude_object2) const
    {
      if (_bounding_box.intersect_ray(ray) == false)
      {
        return nullptr;
      }

      return _brute_force_raycaster->hit_test(ray, dist, normal, normal, exclude_object1, exclude_object2);
    }

    const ggo::axis_aligned_box3d<float> & bounding_box() const { return _bounding_box; }

  private:

    void create_triangle(const ggo::pos3f & v1, const ggo::pos3f & v2, const ggo::pos3f & v3,
      ggo::scene_builder & scene_builder,
      std::shared_ptr<const ggo::material_abc> material)
    {
      auto triangle = std::make_shared<ggo::triangle3d<float, true>>(v1, v2, v3);
      auto object = scene_builder.add_object(triangle, material, true);

      _objects.push_back(object);

      // Update the bounding box too.
      _bounding_box.x_min() = ggo::min(_bounding_box.x_min(), v1.get<0>(), v2.get<0>(), v3.get<0>());
      _bounding_box.x_max() = ggo::max(_bounding_box.x_max(), v1.get<0>(), v2.get<0>(), v3.get<0>());
      _bounding_box.y_min() = ggo::min(_bounding_box.y_min(), v1.get<1>(), v2.get<1>(), v3.get<1>());
      _bounding_box.y_max() = ggo::max(_bounding_box.y_max(), v1.get<1>(), v2.get<1>(), v3.get<1>());
    }

    void create_rectangle(const ggo::pos2f & p1,
      const ggo::pos2f & p2,
      float z_inf, float z_sup,
      ggo::scene_builder & scene_builder,
      std::shared_ptr<const ggo::material_abc> material)
    {
      ggo::pos3f center(0.5f * (p1.get<0>() + p2.get<0>()), 0.5f * (p1.get<1>() + p2.get<1>()), 0.5f * (z_inf + z_sup));
      ggo::vec3f v1(0.5f * (p2.get<0>() - p1.get<0>()), 0.5f * (p2.get<1>() - p1.get<1>()), 0.f);
      ggo::vec3f v2(0.f, 0.f, 0.5f * (z_sup - z_inf));

      auto rectangle = std::make_shared<ggo::parallelogram3d<float>>(center, v1, v2);
      auto object = scene_builder.add_object(rectangle, material, true);

      _objects.push_back(object);

      // Update the bounding box too.
      _bounding_box.x_min() = ggo::min(_bounding_box.x_min(), p1.get<0>(), p2.get<0>());
      _bounding_box.x_max() = ggo::max(_bounding_box.x_max(), p1.get<0>(), p2.get<0>());
      _bounding_box.y_min() = ggo::min(_bounding_box.y_min(), p1.get<1>(), p2.get<1>());
      _bounding_box.y_max() = ggo::max(_bounding_box.y_max(), p1.get<1>(), p2.get<1>());
    }

  private:

    std::vector<std::shared_ptr<const ggo::object3d>> _objects;
    ggo::axis_aligned_box3d<float>                    _bounding_box;
    std::unique_ptr<ggo::brute_force_raycaster>       _brute_force_raycaster;
  };
}

//////////////////////////////////////////////////////////////
// HEXA GROUP
namespace
{
  class hexa_group
  {
  public:

    hexa_group(const std::vector<std::shared_ptr<hexa>> & hexas)
    {
      GGO_ASSERT(hexas.empty() == false);

      _hexas = hexas;

      // Merge bounding boxes.
      _bounding_box = _hexas.front()->bounding_box();
      for (const auto & hexa : _hexas)
      {
        _bounding_box.merge_with(hexa->bounding_box());
      }
    }

    const ggo::object3d * intersect_ray(const ggo::ray3d_float & ray,
      float & dist,
      ggo::ray3d_float & normal,
      const ggo::object3d * exclude_object1,
      const ggo::object3d * exclude_object2) const
    {
      if (_bounding_box.intersect_ray(ray) == false)
      {
        return nullptr;
      }

      const ggo::object3d * hit_object = nullptr;
      dist = std::numeric_limits<float>::max();

      for (const auto & hexa : _hexas)
      {
        GGO_ASSERT(hexa);

        float dist_cur = 0;
        ggo::ray3d_float normal_cur;

        const ggo::object3d * hit_object_tmp = hexa->intersect_ray(ray, dist_cur, normal_cur, exclude_object1, exclude_object2);
        if ((hit_object_tmp != nullptr) && (dist_cur < dist))
        {
          dist = dist_cur;
          hit_object = hit_object_tmp;
          normal = normal_cur;
        }
      }

      return hit_object;
    }

  private:

    std::vector<std::shared_ptr<hexa>>  _hexas;
    ggo::axis_aligned_box3d<float>      _bounding_box;
  };
}

//////////////////////////////////////////////////////////////
// HEXA RAYCASTER
namespace
{
  class hexa_raycaster : public ggo::raycaster_abc
  {
  public:

    hexa_raycaster(const std::vector<std::shared_ptr<hexa_group>> & hexa_groups) : _hexa_groups(hexa_groups) {}

  private:

    bool check_visibility(const ggo::ray3d_float & ray,
      float dist_max,
      const ggo::object3d * exclude_object1,
      const ggo::object3d * exclude_object2) const override
    {
      ggo::ray3d_float normal_tmp;

      for (const auto & hexa_group : _hexa_groups)
      {
        float dist = 0;

        if ((hexa_group->intersect_ray(ray, dist, normal_tmp, exclude_object1, exclude_object2) != nullptr) && (dist < dist_max))
        {
          return true;
        }
      }

      return false;
    }

    const ggo::object3d * hit_test(const ggo::ray3d_float & ray,
      float & dist,
      ggo::ray3d_float & local_normal,
      ggo::ray3d_float & world_normal,
      const ggo::object3d * exclude_object1,
      const ggo::object3d * exclude_object2) const override
    {
      const ggo::object3d * hit_object = nullptr;
      ggo::ray3d_float normal_tmp;

      dist = std::numeric_limits<float>::max();

      for (const auto & hexa_group : _hexa_groups)
      {
        float dist_cur = 0;
        const ggo::object3d * hit_object_tmp = hexa_group->intersect_ray(ray, dist_cur, normal_tmp, exclude_object1, exclude_object2);

        if ((hit_object_tmp != nullptr) && (dist_cur < dist))
        {
          dist = dist_cur;
          hit_object = hit_object_tmp;
          local_normal = normal_tmp;
          world_normal = normal_tmp;
        }
      }

      return hit_object;
    }

  private:

    const std::vector<std::shared_ptr<hexa_group>> &  _hexa_groups;
  };
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

      std::ostringstream oss;
      oss << ((x + 1000) / 4) << ':' << ((y + 1000) / 4);
      const std::string key(oss.str());
      
      hexas_infos.push_back({x1, y1, height1, key});
      hexas_infos.push_back({x2, y2, height2, key});
    }
  }
  
  return hexas_infos;
}

//////////////////////////////////////////////////////////////
void ggo::hexa_artist::render(void * buffer, int width, int height,
                              const std::vector<hexa_info> & hexa_infos,
                              const ggo::color_32f & color_top, const ggo::color_32f & color_side,
                              const std::vector<ggo::pos3f> & lights_pos,
                              const ggo::color_32f & fog_color,
                              ggo::renderer_abc & renderer)
{
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(fog_color));

  // Objects.
  std::map<std::string, std::vector<std::shared_ptr<hexa>>> hexa_groups_map;
  
  auto material_top = std::make_shared<const ggo::solid_color_material>(color_top);
  auto material_side = std::make_shared<const ggo::solid_color_material>(color_side);

  for (const auto & hexa_info : hexa_infos)
  {
    auto new_hexa = std::make_shared<hexa>(hexa_info._x, hexa_info._y, hexa_info._height, scene_builder, material_top, material_side);

    hexa_groups_map[hexa_info._key].push_back(new_hexa);
  }

  std::vector<std::shared_ptr<hexa_group>> hexa_groups;
  for (auto it = hexa_groups_map.begin(); it != hexa_groups_map.end(); ++it)
  {
    hexa_groups.push_back(std::make_shared<hexa_group>(it->second));
  }

  // Add lights to scene.
  for (const auto & light_pos : lights_pos)
  {
    scene_builder.add_sphere_light(ggo::white<ggo::color_32f>(), 100.f, light_pos);
  }

  // Fog and ambient color.
  scene_builder.set_fog(std::make_shared<ggo::linear_fog>(fog_color, 1000.f));
  scene_builder.set_ambient_color(ggo::color_32f(0.1f));

  // Raytracer parameters.
  hexa_raycaster raycaster(hexa_groups);
  ggo::raytrace_params raytrace_params;
  raytrace_params._raycaster = &raycaster;

  // Rendering.
  renderer.render(static_cast<uint8_t *>(buffer), width, height, scene_builder, raytrace_params);
}