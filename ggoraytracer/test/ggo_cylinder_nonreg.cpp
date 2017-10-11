#include <ggo_nonreg.h>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(cylinder, ray_outside)
{
  const int width = 480;
  const int height = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(width, height, ggo::basis3d_float({ 0.0f, 0.f, 50.f }), 0.1f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red_32f()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { -100.f, -100.f, 200.f });

  // Objects.
  scene.add_diffuse_object<ggo::discard_all>(ggo::cylinder3d<float>({ 0.f, 0.f, 5.f }, { 1.f, 0.f, 0.f }, 0.5f), ggo::white_material());
  scene.add_diffuse_object<ggo::discard_all>(ggo::plane3d_float({ 0.f, 0.f, 1.f }, 0.f), ggo::yellow_material());

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("cylinder_outside.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

GGO_TEST(cylinder, ray_inside)
{
  const int width = 480;
  const int height = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(width, height, ggo::basis3d_float({ 0.0f, 0.f, 50.f }), 0.1f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red_32f()));

  // Light.
  scene.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(0.f, 0.f, 20.f));

  // Objects.
  scene.add_diffuse_object<ggo::discard_all>(ggo::cylinder3d<float>({ 0.f, 0.f, 0.f }, { 0.f, 0.f, 1.f }, 2.f), ggo::white_material());
  scene.add_diffuse_object<ggo::discard_all>(ggo::plane3d_float({ 10.f, 0.f, 1.f }, 0.f), ggo::yellow_material());

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("cylinder_inside.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}


