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
  const int GGO_SIZE_X = 480;
  const int GGO_SIZE_Y = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0.0f, 0.f, 50.f);
  camera.set_aperture(0.1f);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::RED));

  // Light.
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(-100, -100, 200));

  // Objects.
  scene_builder.add_object(std::make_shared<ggo::cylinder3d<float>>(ggo::point3d_float(0.f, 0.f, 5.f), ggo::vector3d_float(1.f, 0.f, 0.f), 0.5f), ggo::color::WHITE, false);
  scene_builder.add_object(std::make_shared<ggo::plane3d_float>(ggo::point3d_float(0.f, 0.f, 1.f), 0.f), ggo::color::YELLOW, false);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("cylinder_outside.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}

GGO_TEST(cylinder, ray_inside)
{
  const int GGO_SIZE_X = 480;
  const int GGO_SIZE_Y = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0.0f, 0.f, 50.f);
  camera.set_aperture(0.1f);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::RED));

  // Light.
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(0, 0, 20));

  // Objects.
  scene_builder.add_object(std::make_shared<ggo::cylinder3d<float>>(ggo::point3d_float(0.f, 0.f, 0.f), ggo::vector3d_float(0.f, 0.f, 1.f), 2.f), ggo::color::WHITE, false);
  scene_builder.add_object(std::make_shared<ggo::plane3d_float>(ggo::point3d_float(10.f, 0.f, 1.f), 0.f), ggo::color::YELLOW, false);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("cylinder_inside.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}


