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
  const int GGO_SIZE_X = 480;
  const int GGO_SIZE_Y = 480;

  // The camera.
  ggo::mono_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0.0f, 0.0f, 15.f);
  camera.set_aperture(0.1f);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::RED));

  // Light.
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(0, 0, 2));

  // Objects.
  auto metaball = std::make_shared<ggo::metaball<float>>(0.5f);
  std::shared_ptr<ggo::influence_shape3d_abc<float>> sphere(new ggo::sphere3d<float>({ -0.6f, 0.0f, 0.0f }, 1.0f));
  std::shared_ptr<ggo::influence_shape3d_abc<float>> cylinder(new ggo::cylinder3d<float>({ 0.6f, 0.0f, 0.0f }, { 0.f, 1.f, 0.f }, 1.0f));
  std::shared_ptr<ggo::influence_shape3d_abc<float>> plane(new ggo::influence_plane3d<float>({ 0.f, 1.0f, 0.0f }, 1.5f, 1.0f));
  metaball->add_influence_data(sphere, 1.0f);
  metaball->add_influence_data(cylinder, 1.0f);
  metaball->add_influence_data(plane, 1.0f);
  scene_builder.add_object(metaball, ggo::color::WHITE, false);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("metaball.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}