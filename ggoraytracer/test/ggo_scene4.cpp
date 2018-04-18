#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_buffer.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/ggo_raytracer.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/fogs/ggo_z_fog.h>

GGO_TEST(test_scene, scene4)
{
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));

  // The fog.
  scene.set_fog(std::make_shared<ggo::z_fog>(ggo::green_32f(), 0.f, -1.f, 3.f));

  // Light.
  scene.add_point_light(ggo::white_32f(), { -20.f, -20.f, 200.f });

  // Objects.
  scene.add_diffuse_object<ggo::discard_all>(ggo::sphere3d<float>({ 0.f, 0.f, 0.f }, 1.f), ggo::red_material());
  scene.add_diffuse_object<ggo::discard_all>(ggo::box3d_float(-5.f, 5.f, -5.f, 5.f, -2.f, -1.f), ggo::white_material());

  // Rendering.
  const int width = 400;
  const int height = 400;

  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.2f);
  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);

  ggo::mono_sampling_renderer renderer(camera);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("scene4.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}
