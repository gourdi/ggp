#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_isometric_camera.h>
#include <ggo_background3d_color.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_object3d.h>

GGO_TEST(parallelogram3d, test)
{
  const int size = 400;
  
  // The camera.
  ggo::mono_sampling_isometric_camera camera(size, size);
  camera.basis().set_pos(0, 0, 50);
  camera.set_span(20);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::blue<ggo::color_32f>()));
  
  // Light.
  scene_builder.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(-20.f, -20.f, 200.f));

  // Objects.
  auto parallelogram1 = std::make_shared<ggo::parallelogram3d_float>(ggo::pos3f(0.f, 0.f, 1.f), ggo::pos3f(5.f, 0.f, 0.f), ggo::pos3f(5.f, 5.f, 0.f));
  scene_builder.add_object(parallelogram1, ggo::red<ggo::color_32f>(), false);
  
  auto parallelogram2 = std::make_shared<ggo::parallelogram3d_float>(ggo::pos3f(-11.f, 0.f, 1.f), ggo::pos3f(5.f, 0.f, 0.f), ggo::pos3f(5.f, 5.f, 0.f));
  scene_builder.add_object(parallelogram2, ggo::red<ggo::color_32f>(), false);
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * size * size);
  renderer.render(buffer.data(), size, size, scene_builder);
  ggo::save_bmp("parallelogram3d.bmp", buffer.data(), ggo::rgb_8u_yu, size, size, 3 * size);
}
