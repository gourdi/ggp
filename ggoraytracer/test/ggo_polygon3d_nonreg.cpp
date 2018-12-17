#include <iostream>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/materials/ggo_solid_color_material.h>

void test_polygon3d()
{
  const int width = 400;
  const int height = 400;
  
  // The camera.
  ggo::basis3d_f camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), ggo::pi<float>() / 2);
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);
  
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::black_32f()));

  // Light.  
  scene.add_point_light(ggo::white_32f(), { 0.f, 0.f, 50.f });
  scene.add_point_light(ggo::white_32f(), { 50.f, 0.f, 0.f });

  // Objects.
  auto sphere = ggo::polygon3d_f::create_sphere(2, 16, 8);
  scene.add_diffuse_object<ggo::discard_all>(sphere, ggo::red_material());
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_8u buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("sphere3d.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}
