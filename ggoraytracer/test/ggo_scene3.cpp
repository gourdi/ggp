#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_shapes2d.h>
#include <ggo_paint.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, scene3)
{
  const int GGO_SIZE_X = 640;
  const int GGO_SIZE_Y = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0.f, 0.f, 20.f);
  camera.set_aperture(0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLACK));
  
  // Light.
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(0, 0, 200));

  // Objects.
  auto sphere1 = std::make_shared<ggo::sphere3d_float>(ggo::point3d_float(std::cos(0.f), std::sin(0.f), 0), 0.8f);
  auto sphere1_obj = scene_builder.add_object(sphere1, ggo::color::RED, false);
  sphere1_obj->set_reflection_factor(0.75f);

  auto sphere2 = std::make_shared<ggo::sphere3d_float>(ggo::point3d_float(std::cos(2.f * ggo::PI<float>() / 3.f), std::sin(2.f * ggo::PI<float>() / 3.f), 0.f), 0.8f);
  auto sphere2_obj = scene_builder.add_object(sphere2, ggo::color::GREEN, false);
  sphere2_obj->set_reflection_factor(0.75f);
  
  auto sphere3 = std::make_shared<ggo::sphere3d_float>(ggo::point3d_float(std::cos(-2.f * ggo::PI<float>() / 3.f), std::sin(-2.f * ggo::PI<float>() / 3.f), 0.f), 0.8f);
  auto sphere3_obj = scene_builder.add_object(sphere3, ggo::color::BLUE, false);
  sphere3_obj->set_reflection_factor(0.75f);
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);

  // Overlay, to check that basis::project is working as expected.
  ggo::pos2f proj1 = camera.basis().project(sphere1->center(), camera.get_aperture(), GGO_SIZE_X, GGO_SIZE_Y);
  ggo::paint(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, std::make_shared<ggo::disc_float>(proj1, 5.f), ggo::color::RED);
  ggo::pos2f proj2 = camera.basis().project(sphere2->center(), camera.get_aperture(), GGO_SIZE_X, GGO_SIZE_Y);
  ggo::paint(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, std::make_shared<ggo::disc_float>(proj2, 5.f), ggo::color::RED);

  ggo::save_bmp("scene3.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}