#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_buffer.h>
#include <2d/io/ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_raytracer.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_checker_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(test_scene, scene7)
{
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red_32f()));
  
  // Light.
  scene.add_sphere_light(ggo::white<ggo::color_32f>(), { -20.f, -20.f, 200.f }, 10.f);

  // Objects.
  float roughness = 0.5;
  for (int i = -1; i <= 1; ++i)
  {
    ggo::sphere3d_float sphere({ 2.f * static_cast<float>(i), 0.f, 0.f }, 1.f);
    auto & object = scene.add_diffuse_object<ggo::discard_basis | ggo::discard_phong>(sphere, ggo::white_material());
    object.set_reflection_factor(1);
    object.set_roughness(roughness);
    
    roughness /= 5;
  }
  
  ggo::plane3d_float plane({ 0.f, 0.f, 1.f }, -1.f);
  ggo::checker_xy_material checker_material(ggo::white_32f(), ggo::color_32f(0.5f, 0.5f, 0.5f), 0.5f);

  scene.add_diffuse_object<ggo::discard_all>(plane, checker_material);

  // Rendering.
  const int width = 600;
  const int height = 400;
  const int samples_count = 192;

  ggo::basis3d_float camera_basis({ 0.f, 0.f, 20.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.5);
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, 0.1f, 20.f, 1.f);

  ggo::global_sampling_renderer renderer(camera, samples_count);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("scene7.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

