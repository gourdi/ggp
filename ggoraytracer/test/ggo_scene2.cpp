#include <ggo_nonreg.h>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_linear_fog.h>

GGO_TEST(test_scene, scene2)
{
  const int width = 400;
  const int height = 400;
  const int samples_count = 92;

  // The camera.
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.5f);
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, 0.1f, 50.f, 0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::blue<ggo::color_32f>()));

  // The fog.
  scene_builder.set_fog(std::make_shared<ggo::linear_fog>(ggo::color_32f(0.5f), 100.F));

  // The light.
  scene_builder.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(-20.f, -20.f, 200.f));

  // The objects.
  auto sphere1 = std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(-2.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere1, ggo::red<ggo::color_32f>(), false);
  
  auto sphere2 = std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(0.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere2, ggo::white<ggo::color_32f>(), false);
  
  auto sphere3 = std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(2.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere3, ggo::black<ggo::color_32f>(), false);

  auto plane = std::make_shared<ggo::plane3d<float>>(ggo::vec3f(0.f, 0.f, 1.f), -1.f);
  scene_builder.add_object(plane, ggo::white<ggo::color_32f>(), false);
  
  // Rendering.
  ggo::global_sampling_renderer renderer(camera, samples_count);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene_builder);
  ggo::save_bmp("scene2.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}