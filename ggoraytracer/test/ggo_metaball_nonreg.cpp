#include <kernel/nonreg/ggo_nonreg.h>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
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
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red_32f()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { 0.f, 0.f, 2.f });

  // Objects.
  ggo::metaball<float> metaball(0.5f);
  std::shared_ptr<ggo::influence_shape3d_abc<float>> sphere(new ggo::sphere3d<float>({ -0.6f, 0.0f, 0.0f }, 1.0f));
  std::shared_ptr<ggo::influence_shape3d_abc<float>> plane(new ggo::influence_plane3d<float>({ 0.f, 1.0f, 0.0f }, 1.5f, 1.0f));
  metaball.add_influence_data(sphere, 1.0f);
  metaball.add_influence_data(plane, 1.0f);
  scene.add_diffuse_object<ggo::discard_all>(metaball, ggo::white_material());

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("metaball.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}