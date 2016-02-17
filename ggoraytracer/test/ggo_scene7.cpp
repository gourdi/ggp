#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_raytracer.h>
#include <ggo_object3d.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_checker_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, scene7)
{
  const int GGO_SIZE_X = 600;
  const int GGO_SIZE_Y = 400;
  const int GGO_SAMPLES_COUNT = 192;

  // The camera.
  ggo::multi_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0, 0, 20);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.5);
  camera.set_aperture(0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::RED));
  
  // Light.
  scene_builder.add_sphere_light(ggo::color::WHITE, 10.f, ggo::point3d_float(-20.f, -20.f, 200.f));

  // Objects.
  float roughness = 0.5;
  for (int i = -1; i <= 1; ++i)
  {
    auto sphere = std::make_shared<ggo::sphere3d<float>>(ggo::point3d_float(2.f * static_cast<float>(i), 0.f, 0.f), 1.f);
    auto object = scene_builder.add_object(sphere, ggo::color::RED, false);
    object->set_reflection_factor(1);
    object->set_roughness(roughness);
    
    roughness /= 5;
  }
  
  auto plane = std::make_shared<ggo::plane3d<float>>(0.f, 0.f, 1.f, 1.f);
  auto checker_material = std::make_shared<ggo::checker_xy_material>(ggo::color::WHITE, ggo::color(0.5f, 0.5f, 0.5f), 0.5f);
  scene_builder.add_object(plane, checker_material, false);

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, GGO_SAMPLES_COUNT);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer, GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("scene7.bmp", buffer, GGO_SIZE_X, GGO_SIZE_Y);
}