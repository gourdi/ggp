#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(cylinder, ray_outside)
{
  const int width = 480;
  const int height = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(width, height);
  camera.basis().set_pos(0.0f, 0.f, 50.f);
  camera.set_aperture(0.1f);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color_32f::red));

  // Light.
  scene_builder.add_point_light(ggo::color_32f::white, ggo::pos3f(-100.f, -100.f, 200.f));

  // Objects.
  scene_builder.add_object(std::make_shared<ggo::cylinder3d<float>>(ggo::pos3f(0.f, 0.f, 5.f), ggo::vec3f(1.f, 0.f, 0.f), 0.5f), ggo::color_32f::white, false);
  scene_builder.add_object(std::make_shared<ggo::plane3d_float>(ggo::pos3f(0.f, 0.f, 1.f), 0.f), ggo::color_32f::yellow, false);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, scene_builder);
  ggo::save_bmp("cylinder_outside.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

GGO_TEST(cylinder, ray_inside)
{
  const int width = 480;
  const int height = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(width, height);
  camera.basis().set_pos(0.0f, 0.f, 50.f);
  camera.set_aperture(0.1f);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color_32f::red));

  // Light.
  scene_builder.add_point_light(ggo::color_32f::white, ggo::pos3f(0.f, 0.f, 20.f));

  // Objects.
  scene_builder.add_object(std::make_shared<ggo::cylinder3d<float>>(ggo::pos3f(0.f, 0.f, 0.f), ggo::vec3f(0.f, 0.f, 1.f), 2.f), ggo::color_32f::white, false);
  scene_builder.add_object(std::make_shared<ggo::plane3d_float>(ggo::pos3f(10.f, 0.f, 1.f), 0.f), ggo::color_32f::yellow, false);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, scene_builder);
  ggo::save_bmp("cylinder_inside.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}


