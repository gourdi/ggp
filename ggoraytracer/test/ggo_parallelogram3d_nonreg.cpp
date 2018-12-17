#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <raytracer/cameras/ggo_isometric_camera.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>

GGO_TEST(parallelogram3d, test)
{
  const int size = 400;
  
  // The camera.
  ggo::mono_sampling_isometric_camera camera(size, size, ggo::basis3d_f({ 0.f, 0.f, 50.f }), 20.f);
  
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue_32f()));
  
  // Light.
  scene.add_point_light(ggo::white_32f(), ggo::pos3_f(-20.f, -20.f, 200.f));

  // Objects.
  ggo::parallelogram3d_f parallelogram1({ 0.f, 0.f, 1.f }, { 5.f, 0.f, 0.f }, { 5.f, 5.f, 0.f });
  scene.add_diffuse_object<ggo::discard_all>(parallelogram1, ggo::red_material());
  
  ggo::parallelogram3d_f parallelogram2({ -11.f, 0.f, 1.f }, { 5.f, 0.f, 0.f }, { 5.f, 5.f, 0.f });
  scene.add_diffuse_object<ggo::discard_all>(parallelogram2, ggo::red_material());
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_8u buffer(3 * size * size);
  renderer.render(buffer.data(), size, size, 3 * size, ggo::rgb_8u_yu, scene);
  ggo::save_bmp("parallelogram3d.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}
