#include <iostream>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_background3d_color.h>
#include <ggo_object3d.h>

void test_polygon3d()
{
#if 0
  const int width = 400;
  const int height = 400;
  
  // The camera.
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), ggo::pi<float>() / 2);
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::black<ggo::color_32f>()));

  // Light.  
  scene_builder.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(0.f, 0.f, 50.f));
  scene_builder.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(50.f, 0.f, 0.f));

  // Objects.
  auto sphere = ggo::polygon3d_float::create_sphere(2, 16, 8);
  auto polygon = std::make_shared<ggo::polygon3d_float>(sphere);
  scene_builder.add_object(polygon, ggo::red<ggo::color_32f>(), false);
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene_builder);
  ggo::save_bmp("sphere3d.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
#endif
}
