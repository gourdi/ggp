#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_buffer.h>
#include <2d/io/ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_brute_force_raycaster.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_bidirectional_lighting.h>

GGO_TEST(bidirectional_lighting, test)
{
  const int width = 480;
  const int height = 480;
  const int direct_lighting_samples_count = 96;
  
  // The camera.
  ggo::multi_sampling_point_camera camera(width, height, ggo::basis3d_float({ 0.f, 0.f, 2.5f }), 0.5f, 2.5f, 0.01f);
  
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));
  
  // Light.
  scene.add_sphere_light(ggo::color_32f(0.9f), { 0.f, 0.75f, 0.f }, 0.2f);

  // Objects.
  constexpr uint32_t flags = ggo::discard_all;
  scene.add_diffuse_object<flags>(ggo::plane3d_float({ 1.f,  0.f, 0.f}, -1.f), ggo::red_material());
  scene.add_diffuse_object<flags>(ggo::plane3d_float({-1.f,  0.f, 0.f}, -1.f), ggo::green_material());
  scene.add_diffuse_object<flags>(ggo::plane3d_float({ 0.f,  0.f, 1.f}, -1.f), ggo::white_material());
  scene.add_diffuse_object<flags>(ggo::plane3d_float({ 0.f,  1.f, 0.f}, -1.f), ggo::white_material());
  scene.add_diffuse_object<flags>(ggo::plane3d_float({ 0.f, -1.f, 0.f }, -1.f), ggo::white_material());
  scene.add_diffuse_object<flags>(ggo::plane3d_float({ 0.f, -0.6f, 0.f }, 0.4f), ggo::white_material());

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, direct_lighting_samples_count);
  ggo::buffer buffer(3 * width * height);

  // Without indirect lighting.
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("bidirectional_lighting_off.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
  
  // With indirect lighting.
  ggo::brute_force_raycaster raycaster(scene);
  ggo::bidirectional_lighting indirect_lighting(raycaster, scene);
  ggo::raytrace_params raytrace_params;
  raytrace_params._indirect_lighting = &indirect_lighting;
  
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene, raytrace_params);
  ggo::save_bmp("bidirectional_lighting_on.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

