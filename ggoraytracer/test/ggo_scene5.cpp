#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_raytracer.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, scene5)
{
  const int GGO_SIZE_X = 480;
  const int GGO_SIZE_Y = 480;
  const int GGO_SAMPLES_COUNT = 96;
  
  // The camera.
  ggo::multi_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0, 0, 50);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.5);
  camera.set_aperture(0.1f);
  camera.set_depth_of_field_factor(2.f);
  camera.set_depth_of_field(50.f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLUE));

  // Light.
  scene_builder.add_sphere_light(ggo::color::WHITE, 10.f, ggo::pos3f(-20.f, -20.f, 200.f));

  // Objects.
  for (int i = -2; i < 5; ++i)
  {
    auto sphere = std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(static_cast<float>(i), 2.f * static_cast<float>(i), 0.f), 0.5f);
    scene_builder.add_object(sphere, ggo::color::RED, false);
  }

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, GGO_SAMPLES_COUNT);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("scene5.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}