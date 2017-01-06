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
  const int width = 480;
  const int height = 480;
  const int samples_count = 96;
  
  // The camera.
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.5);
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, 0.1f, 50.f, 2.f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::blue<ggo::color_32f>()));

  // Light.
  scene_builder.add_sphere_light(ggo::white<ggo::color_32f>(), 10.f, ggo::pos3f(-20.f, -20.f, 200.f));

  // Objects.
  for (int i = -2; i < 5; ++i)
  {
    auto sphere = std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(static_cast<float>(i), 2.f * static_cast<float>(i), 0.f), 0.5f);
    scene_builder.add_object(sphere, ggo::red<ggo::color_32f>(), false);
  }

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, samples_count);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene_builder);
  ggo::save_bmp("scene5.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}