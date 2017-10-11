#include <ggo_nonreg.h>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_linear_fog.h>

GGO_TEST(test_scene, scene2)
{  
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));

  // The fog.
  scene.set_fog(std::make_shared<ggo::linear_fog>(ggo::color_32f(0.5f), 100.f));

  // The light.
  scene.add_point_light(ggo::white_32f(), ggo::pos3f(-20.f, -20.f, 200.f));

  // The objects.
  scene.add_diffuse_object<ggo::discard_all>(ggo::sphere3d_float({ -1.f, 0.f, 0.f }, 1.f), ggo::white_material());
  scene.add_simple_color_object<ggo::discard_basis>(ggo::sphere3d<float>({ 1.f, 0.f, 0.f }, 1.f), ggo::white_material());
  scene.add_diffuse_object<ggo::discard_all>(ggo::plane3d_float({ 0.f, 0.f, 1.f }, -1.f), ggo::white_material());
  
  // Rendering.
  const int width = 400;
  const int height = 400;
  const int samples_count = 92;

  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.5f);
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, 0.1f, 50.f, 0.1f);

  ggo::global_sampling_renderer renderer(camera, samples_count);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("scene2.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}