#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/ggo_scene.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/materials/ggo_checker_material.h>
#include <raytracer/objects3d/volumetric_objects/ggo_disc_glow.h>
#include <raytracer/objects3d/volumetric_objects/ggo_sphere_glow.h>

GGO_TEST(glow, disc)
{
  const int width = 400;
  const int height = 400;

  // The camera.
  ggo::basis3d_f camera_basis({ 0.0f, 0.0f, 30.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 0.8f);
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { 0.f, 0.f, 50.f });

  // Objects.
  scene.emplace_volumetric_object<ggo::disc_glow>(ggo::pos3_f(-0.5f, 0.f, 2.f), 0.5f, ggo::white_32f(), 0.2f, 1.f);
  scene.emplace_volumetric_object<ggo::disc_glow>(ggo::pos3_f(0.5f, 0.f, 1.f), 0.8f, ggo::red_32f(), 0.8f, 1.f);

  ggo::checker_xy_material checker_material(ggo::black_32f(), ggo::gray_32f(), 0.5f);
  scene.add_diffuse_object<ggo::discard_basis>(ggo::plane3d_f({ 0.f, 0.f, 1.f }, -1.f), checker_material);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  auto img = renderer.render(scene, { width, height });
  ggo::save_bmp("glow_disc.bmp", img);
}

GGO_TEST(glow, sphere)
{
  const int width = 400;
  const int height = 400;

  // The camera.
  ggo::basis3d_f camera_basis({ 0.0f, 0.0f, 30.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 0.8f);
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { 0.f, 0.f, 50.f });

  // Objects.
  scene.emplace_volumetric_object<ggo::sphere_glow>(ggo::pos3_f(-0.5f, 0.f, 2.f), 0.5f, 0.5f, ggo::white_32f());
  scene.emplace_volumetric_object<ggo::sphere_glow>(ggo::pos3_f(0.5f, 0.f, 1.f), 0.5f, 0.8f, ggo::red_32f());

  ggo::checker_xy_material checker_material(ggo::black_32f(), ggo::gray_32f(), 0.5f);
  scene.add_diffuse_object<ggo::discard_basis>(ggo::plane3d_f({ 0.f, 0.f, 1.f }, -1.f), checker_material);

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  auto img = renderer.render(scene, { width, height });
  ggo::save_bmp("glow_sphere.bmp", img);
}


