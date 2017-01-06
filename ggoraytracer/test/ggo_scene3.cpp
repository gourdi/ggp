#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_shapes2d.h>
#include <ggo_point_camera.h>
#include <ggo_buffer_paint.h>
#include <ggo_object3d.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, scene3)
{
  const int width = 640;
  const int height = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(width, height, ggo::basis3d_float({ 0.f, 0.f, 20.f }), 0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::black<ggo::color_32f>()));
  
  // Light.
  scene_builder.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(0.f, 0.f, 200.f));

  // Objects.
  auto sphere1 = std::make_shared<ggo::sphere3d_float>(ggo::pos3f(std::cos(0.f), std::sin(0.f), 0.f), 0.8f);
  auto sphere1_obj = scene_builder.add_object(sphere1, ggo::red<ggo::color_32f>(), false);
  sphere1_obj->set_reflection_factor(0.75f);

  auto sphere2 = std::make_shared<ggo::sphere3d_float>(ggo::pos3f(std::cos(2.f * ggo::pi<float>() / 3.f), std::sin(2.f * ggo::pi<float>() / 3.f), 0.f), 0.8f);
  auto sphere2_obj = scene_builder.add_object(sphere2, ggo::green<ggo::color_32f>(), false);
  sphere2_obj->set_reflection_factor(0.75f);
  
  auto sphere3 = std::make_shared<ggo::sphere3d_float>(ggo::pos3f(std::cos(-2.f * ggo::pi<float>() / 3.f), std::sin(-2.f * ggo::pi<float>() / 3.f), 0.f), 0.8f);
  auto sphere3_obj = scene_builder.add_object(sphere3, ggo::blue<ggo::color_32f>(), false);
  sphere3_obj->set_reflection_factor(0.75f);
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene_builder);

  // Overlay, to check that basis::project is working as expected.
  ggo::pos2f proj1 = camera.basis().project(sphere1->center(), camera.get_aperture(), width, height);
  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, ggo::disc_float(proj1, 5.f), ggo::red<ggo::color_8u>());
  ggo::pos2f proj2 = camera.basis().project(sphere2->center(), camera.get_aperture(), width, height);
  ggo::paint_shape<ggo::rgb_8u_yu, ggo::sampling_4x4>(buffer.data(), width, height, 3 * width, ggo::disc_float(proj2, 5.f), ggo::red<ggo::color_8u>());

  ggo::save_bmp("scene3.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}