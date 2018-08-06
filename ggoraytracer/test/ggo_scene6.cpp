#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/ggo_raytracer.h>
#include <raytracer/cameras/ggo_isometric_camera.h>
#include <raytracer/renderers/ggo_global_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>

GGO_TEST(test_scene, scene6)
{
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::red_32f()));
  
  // Light.
  scene.add_sphere_light(ggo::white_32f(), { -20.f, -20.f, 200.f }, 10.f);

  // Objects.
  for (int i = -2; i <= 5; ++i)
  {
    ggo::sphere3d_float sphere({ static_cast<float>(i), 2.f * static_cast<float>(i), 0.f }, 0.5f);
    scene.add_diffuse_object<ggo::discard_all>(sphere, ggo::yellow_material());
  }

  // Rendering.
  const int width = 480;
  const int height = 240;
  const int samples_count = 48;

  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.5);
  ggo::multi_sampling_isometric_camera camera(width, height, camera_basis, 3.f, 50.f, 2.f);

  ggo::global_sampling_renderer renderer(camera, samples_count);
  ggo::array_8u buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("scene6.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}
