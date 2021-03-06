#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/ggo_raytracer.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/materials/ggo_checker_material.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>

GGO_TEST(test_scene, scene7)
{
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red_32f()));
  
  // Light.
  scene.add_sphere_light(ggo::white_32f(), { -20.f, -20.f, 200.f }, 10.f);

  // Objects.
  float roughness = 0.5;
  for (int i = -1; i <= 1; ++i)
  {
    ggo::sphere3d_f sphere({ 2.f * static_cast<float>(i), 0.f, 0.f }, 1.f);
    auto & object = scene.add_diffuse_object<ggo::discard_basis | ggo::discard_phong>(sphere, ggo::white_material());
    object.set_reflection_factor(1);
    object.set_roughness(roughness);
    
    roughness /= 5;
  }
  
  ggo::plane3d_f plane({ 0.f, 0.f, 1.f }, -1.f);
  ggo::checker_xy_material checker_material(ggo::white_32f(), ggo::rgb_32f(0.5f), 0.5f);

  scene.add_diffuse_object<ggo::discard_all>(plane, checker_material);

  // Rendering.
  const int width = 600;
  const int height = 400;
  const int samples_count = 192;

  ggo::basis3d_f camera_basis({ 0.f, 0.f, 20.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 1.5);
  ggo::multi_sampling_point_camera camera(width, height, camera_basis, 0.1f, 20.f, 1.f);

  ggo::global_sampling_renderer renderer(camera, samples_count);
  auto img = renderer.render(scene, { width, height });
  ggo::save_bmp("scene7.bmp", img);
}

