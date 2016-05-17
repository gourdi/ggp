#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_raytracer.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_z_fog.h>

GGO_TEST(test_scene, scene4)
{
  const int GGO_SIZE_X = 400;
  const int GGO_SIZE_Y = 400;

  // The camera.
  ggo::mono_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0, 0, 50);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.2f);
  camera.set_aperture(0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLUE));

  // The fog.
  scene_builder.set_fog(std::make_shared<ggo::z_fog>(ggo::color::GREEN, 0.f, -1.f, 3.f));

  // Light.
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(-20, -20, 200));

  // Objects.
  auto sphere = std::make_shared<ggo::sphere3d<float>>(ggo::point3d_float(0.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere, ggo::color::RED, false);
  
  auto aabox3d = std::make_shared<ggo::aabox3d_float>(-5.f, 5.f, -5.f, 5.f, -2.f, -1.f);
  scene_builder.add_object(aabox3d, ggo::color::WHITE, false);
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("scene4.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}
