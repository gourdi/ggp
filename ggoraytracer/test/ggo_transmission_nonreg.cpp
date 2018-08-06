#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/ggo_raytracer.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/materials/ggo_checker_material.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/objects3d/solid_objects/ggo_transparent_object3d.h>

GGO_TEST(transmission, test)
{
  const int width = 600;
  const int height = 400;
  const int samples_count = 56;

  // The camera.
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 40.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.3f);
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, 0.1f, 40.f, 0.1f);

  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));

  // Light.
  scene.add_sphere_light(ggo::rgb_32f(1.25f), { -20.f, -100.f, 200.f }, 100.f);

  // Objects.
  std::array<float, 5> densities { 1.0f, 1.01f, 1.05f, 1.1f, 1.5f };
  auto densities_it = densities.cbegin();
  for (int i = -2; i <= 2; ++i)
  {
    scene.add_transparent_object<ggo::discard_basis>(ggo::sphere3d_float({ 2.f * static_cast<float>(i), 0.f, 0.f }, 1.f), ggo::white_32f(), *densities_it++);
  }

  scene.add_diffuse_object<ggo::discard_basis>(ggo::box3d_float(-6.f, 6.f, 5.f, 5.5f, -1.f, -0.5f), ggo::red_material());

  ggo::checker_xy_material checker_material(ggo::white_32f(), ggo::gray_32f(), 0.5f);
  scene.add_diffuse_object<ggo::discard_basis>(ggo::plane3d_float({ 0.f, 0.f, 1.f }, -1.f), checker_material);

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, samples_count);
  ggo::array_8u buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("transmission.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}
