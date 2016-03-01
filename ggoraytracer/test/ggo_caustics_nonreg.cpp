#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_scene.h>
#include <ggo_background3d_color.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_brute_force_raycaster.h>
#include <ggo_photon_mapping.h>

GGO_TEST(caustics, test)
{
  const int GGO_SIZE_X = 480;
  const int GGO_SIZE_Y = 480;
  const int GGO_DIRECT_LIGHTING_SAMPLES_COUNT = 96;
  const int GGO_PHOTONS_COUNT = 1 << 15;

  // The camera.
  ggo::multi_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0, 0, 5);
  camera.set_aperture(0.5);
  camera.set_depth_of_field_factor(0.01f);
  camera.set_depth_of_field(2.5f);
  camera.basis().rotate(ggo::ray3d_float::O_X(), 0.8f);

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLUE));

  // Light.
  auto light = scene_builder.add_sphere_light(ggo::color(0.9f), 1.f, ggo::point3d_float(0, 0, 20));

  // Objects.
  auto plane  = scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(ggo::vector3d_float(0.f, 0.f, 1.f), -1.f), ggo::color::RED, true);
  auto sphere = scene_builder.add_object(std::make_shared<ggo::sphere3d<float>>(ggo::point3d_float(0.f, 0.f, 1.f), 1.f), ggo::color::WHITE, true);
  sphere->set_transparent(true);
  sphere->set_density(1.1f);
  sphere->set_phong_factor(5.0f);
  sphere->set_phong_shininess(100.f);

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, GGO_DIRECT_LIGHTING_SAMPLES_COUNT);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);

  // Without indirect lighting.
  renderer.render(buffer, GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("caustics_off.bmp", buffer, GGO_SIZE_X, GGO_SIZE_Y);

  // With indirect lighting.
  std::vector<ggo::point3d_float> target_samples;
  for (int i = 0; i < GGO_PHOTONS_COUNT; ++i)
  {
    float x = ggo::rand_float(-1, 1);
    float y = ggo::rand_float(-1, 1);
    float z = ggo::rand_float( 0, 2);

    target_samples.push_back({ x, y, z });
  }
  std::vector<std::shared_ptr<const ggo::object3d>> lights;
  lights.push_back(light);
  ggo::scene scene = scene_builder.build_scene();
  ggo::brute_force_raycaster raycaster(scene.objects());
  ggo::photon_mapping photon_mapping(lights, target_samples, *sphere, raycaster);
  ggo::raytrace_params raytrace_params;
  raytrace_params._indirect_lighting = &photon_mapping;

  renderer.render(buffer, GGO_SIZE_X, GGO_SIZE_Y, scene, raytrace_params);
  ggo::save_bmp("caustics_on.bmp", buffer, GGO_SIZE_X, GGO_SIZE_Y);
}