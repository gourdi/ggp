#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, scene1)
{
  const int width = 640;
  const int height = 480;
  const int GGO_SAMPLES_COUNT = 32;

  // The camera.
  ggo::multi_sampling_point_camera camera(width, height);
  camera.basis().set_pos(0.f, 0.f, 50.f);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 1.5f);
  camera.set_aperture(0.1f);
  camera.set_depth_of_field_factor(1.f);
  camera.set_depth_of_field(50.f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color_32f::BLUE));

  // Light.
  scene_builder.add_sphere_light(ggo::color_32f::WHITE, 10.f, { -20.f, -20.f, 50.f });

  // Objects.
  auto sphere = std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(-2.f, 0.f, 0.f), 1.f);
  scene_builder.add_object(sphere, ggo::color_32f::RED, false);
  
  auto box_shape = std::make_shared<ggo::axis_aligned_box3d<float>>(-0.7f, 0.7f, -0.8f, 0.8f, -0.9f, 0.9f);
  auto box_object = scene_builder.add_object(box_shape, ggo::color_32f::GREEN, false);
  box_object->basis().rotate(ggo::ray3d_float::O_X(), 0.1f);
  box_object->basis().rotate(ggo::ray3d_float::O_Y(), 0.2f);
  box_object->basis().rotate(ggo::ray3d_float::O_Z(), 0.3f);
  box_object->basis().move(2, 0, 1);

  auto plane = std::make_shared<ggo::plane3d<float>>(ggo::vec3f(0.f, 0.f, 1.f), -1.f);
  scene_builder.add_object(plane, ggo::color_32f::WHITE, false);

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, GGO_SAMPLES_COUNT);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, scene_builder);
  ggo::save_bmp("scene1.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}