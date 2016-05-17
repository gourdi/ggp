#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_linear_fog.h>

GGO_TEST(test_scene, scene2)
{
  const int GGO_SIZE_X = 400;
  const int GGO_SIZE_Y = 400;
  const int GGO_SAMPLES_COUNT = 92;

  // The camera.
  ggo::multi_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0.f, 0.f, 50.f);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.5f);
  camera.set_aperture(0.1f);
  camera.set_depth_of_field(50.f);
  camera.set_depth_of_field_factor(0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLUE));

  // The fog.
  scene_builder.set_fog(std::make_shared<ggo::linear_fog>(ggo::color(0.5f), 100.F));

  // The light.
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(-20, -20, 200));

  // The objects.
  auto sphere1 = std::make_shared<ggo::sphere3d<float>>(ggo::point3d_float(-2.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere1, ggo::color::RED, false);
  
  auto sphere2 = std::make_shared<ggo::sphere3d<float>>(ggo::point3d_float(0.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere2, ggo::color::WHITE, false);
  
  auto sphere3 = std::make_shared<ggo::sphere3d<float>>(ggo::point3d_float(2.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere3, ggo::color::BLACK, false);

  auto plane = std::make_shared<ggo::plane3d<float>>(ggo::vector3d_float(0.f, 0.f, 1.f), -1.f);
  scene_builder.add_object(plane, ggo::color::WHITE, false);
  
  // Rendering.
  ggo::global_sampling_renderer renderer(camera, GGO_SAMPLES_COUNT);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("scene2.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}