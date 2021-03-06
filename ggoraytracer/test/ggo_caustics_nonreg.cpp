#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/ggo_scene.h>
#include <raytracer/objects3d/ggo_object3d_abc.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/raycasters/ggo_brute_force_raycaster.h>
#include <raytracer/indirect_lighting/ggo_photon_mapping.h>
#include <raytracer/materials/ggo_solid_color_material.h>

GGO_TEST(caustics, test)
{
  const int width = 480;
  const int height = 480;
  const int direct_lighting_samples_count = 96;
  const int photons_count = 1 << 15;

  // The camera.
  ggo::basis3d_f camera_basis({ 0.f, 0.f, 5.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 0.8f);
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, 0.5f, 2.5f, 0.01f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));

  // Light.
  auto & light = scene.add_sphere_light(ggo::rgb_32f(0.9f), { 0.f, 0.f, 20.f }, 1.f);

  // Objects.
  auto & plane = scene.add_diffuse_object<ggo::discard_all>(ggo::plane3d_f({ 0.f, 0.f, 1.f }, -1.f), ggo::red_material());
  auto & sphere = scene.add_transparent_object<ggo::discard_all>(ggo::sphere3d_f({ 0.f, 0.f, 1.f }, 1.f), ggo::white_32f(), 1.1f);
  sphere.set_phong(5.0f, 100.f);

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, direct_lighting_samples_count);

  // Without indirect lighting.
  auto img = renderer.render(scene, { width, height });
  ggo::save_bmp("caustics_off.bmp", img);

  // With indirect lighting.
  std::vector<ggo::pos3_f> target_samples;
  for (int i = 0; i < photons_count; ++i)
  {
    float x = ggo::rand<float>(-1, 1);
    float y = ggo::rand<float>(-1, 1);
    float z = ggo::rand<float>( 0, 2);

    target_samples.push_back({ x, y, z });
  }
  ggo::brute_force_raycaster raycaster(scene.solid_objects());
  ggo::photon_mapping photon_mapping({ &light }, target_samples, sphere, raycaster);
  ggo::raytrace_params raytrace_params;
  raytrace_params._indirect_lighting = &photon_mapping;

  img = renderer.render(scene, { width, height }, raytrace_params);
  ggo::save_bmp("caustics_on.bmp", img);
}