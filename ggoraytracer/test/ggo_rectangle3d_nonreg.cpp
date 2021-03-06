#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>

GGO_TEST(rectangle3d, test)
{
  const int size = 400;

  // The camera.
  ggo::basis3d_f camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 1.1f);
  camera_basis.rotate(ggo::ray3d_f::O_Z(), 0.5f);
  ggo::multi_sampling_point_camera camera(size, size, camera_basis, 0.1f, 50.f, 1.f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { -20.f, -20.f, 200.f });

  // Objects.
  ggo::rectangle3d<-1, 0, 0, float> side1({ -1.f, 0.f, 0.f }, 1.f, 1.f);
  ggo::rectangle3d<1, 0, 0, float>  side2({ 1.f, 0.f, 0.f }, 1.f, 1.f);

  ggo::rectangle3d<0, -1, 0, float> side3({ 0.f, -1.f, 0.f }, 1.f, 1.f);
  ggo::rectangle3d<0, 1, 0, float>  side4({ 0.f, 1.f, 0.f }, 1.f, 1.f);

  ggo::rectangle3d<0, 0, 1, float>  side5({ 0.f, 0.f, 1.f }, 1.f, 1.f);

  scene.add_diffuse_object<ggo::discard_all>(side1, ggo::red_material());
  scene.add_diffuse_object<ggo::discard_all>(side2, ggo::red_material());
  scene.add_diffuse_object<ggo::discard_all>(side3, ggo::red_material());
  scene.add_diffuse_object<ggo::discard_all>(side4, ggo::red_material());
  scene.add_diffuse_object<ggo::discard_all>(side5, ggo::red_material());

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, 56);
  auto img = renderer.render(scene, { size, size });
  ggo::save_bmp("rectangle3d.bmp", img);
}
