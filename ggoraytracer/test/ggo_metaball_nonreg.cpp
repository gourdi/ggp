#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, metaball)
{
  const int width = 480;
  const int height = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(width, height, ggo::basis3d_float({ 0.0f, 0.0f, 15.f }), 0.1f);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::red<ggo::color_32f>()));

  // Light.
  scene_builder.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(0.f, 0.f, 2.f));

  // Objects.
  auto metaball = std::make_shared<ggo::metaball<float>>(0.5f);
  std::shared_ptr<ggo::influence_shape3d_abc<float>> sphere(new ggo::sphere3d<float>({ -0.6f, 0.0f, 0.0f }, 1.0f));
  std::shared_ptr<ggo::influence_shape3d_abc<float>> cylinder(new ggo::cylinder3d<float>({ 0.6f, 0.0f, 0.0f }, { 0.f, 1.f, 0.f }, 1.0f));
  std::shared_ptr<ggo::influence_shape3d_abc<float>> plane(new ggo::influence_plane3d<float>({ 0.f, 1.0f, 0.0f }, 1.5f, 1.0f));
  metaball->add_influence_data(sphere, 1.0f);
  metaball->add_influence_data(cylinder, 1.0f);
  metaball->add_influence_data(plane, 1.0f);
  scene_builder.add_object(metaball, ggo::white<ggo::color_32f>(), false);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene_builder);
  ggo::save_bmp("metaball.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}