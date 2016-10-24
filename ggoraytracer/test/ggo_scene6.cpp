#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_isometric_camera.h>
#include <ggo_object3d.h>
#include <ggo_raytracer.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, scene6)
{
  const int width = 480;
  const int height = 240;
  const int GGO_SAMPLES_COUNT = 48;

  // The camera.
  ggo::multi_sampling_isometric_camera camera(width, height);
  camera.basis().set_pos(0, 0, 50);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.5);
  camera.set_span(3);
  camera.set_depth_of_field_factor(2);
  camera.set_depth_of_field(50);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color_32f::RED));
  
  // Light.
  scene_builder.add_sphere_light(ggo::color_32f::WHITE, 10.f, ggo::pos3f(-20.f, -20.f, 200.f));

  // Objects.
  for (int i = -2; i < 5; ++i)
  {
    auto sphere = std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(static_cast<float>(i), 2.f * static_cast<float>(i), 0.f), 0.5f);
    scene_builder.add_object(sphere, ggo::color_32f::YELLOW, false);
  }

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, GGO_SAMPLES_COUNT);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, scene_builder);
  ggo::save_bmp("scene6.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}