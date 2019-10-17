#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>

GGO_TEST(cylinder, test)
{
  const int width = 480;
  const int height = 480;

  // The camera.
  ggo::basis3d_f camera_basis({ 0.0f, 0.f, 30.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 0.5f);
  camera_basis.rotate(ggo::ray3d_f::O_Z(), 0.5f);
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red_32f()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { -100.f, -100.f, 50.f });

  // Objects.
  scene.add_diffuse_object<ggo::discard_all>(ggo::cylinder3d_f({ 0.f, 0.f, 2.f }, { 0.f, 0.f, 1.f }, 1.f, 0.5f), ggo::white_material());
  scene.add_diffuse_object<ggo::discard_all>(ggo::plane3d_f({ 0.f, 0.f, 1.f }, 0.f), ggo::yellow_material());

  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  auto img = renderer.render(scene, { width, height });
  ggo::save_bmp("cylinder.bmp", img);
}
