#include <ggo_buffer.h>
#include <ggo_nonreg.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_scene.h>
#include <ggo_background3d_color.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_checker_material.h>

GGO_TEST(glow, test)
{
  const int width = 400;
  const int height = 400;

  // The camera.
  ggo::basis3d_float camera_basis({ 0.0f, 0.0f, 30.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 0.8f);
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red<ggo::color_32f>()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { 0.f, 0.f, 50.f });

  // Objects.
  scene.add_glow({ -0.5f, 0.f, 2.f }, 0.8f, ggo::white_32f(), 0.8f, 1.f);
  scene.add_glow({ 0.5f, 0.f, 1.f }, 0.8f, ggo::red_32f(), 0.8f, 1.f);

  ggo::checker_xy_material checker_material(ggo::white_32f(), ggo::gray_32f(), 0.5f);
  scene.add_diffuse_object<ggo::discard_basis>(ggo::plane3d_float({ 0.f, 0.f, 1.f }, -1.f), checker_material);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("glow.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}
