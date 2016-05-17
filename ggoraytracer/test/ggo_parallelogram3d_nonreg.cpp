#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_isometric_camera.h>
#include <ggo_background3d_color.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_object3d.h>

GGO_TEST(parallelogram3d, test)
{
  const int SIZE = 400;
  
  // The camera.
  ggo::mono_sampling_isometric_camera camera(SIZE, SIZE);
  camera.basis().set_pos(0, 0, 50);
  camera.set_span(20);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLUE));
  
  // Light.
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(-20, -20, 200));

  // Objects.
  auto parallelogram1 = std::make_shared<ggo::parallelogram3d_float>(ggo::point3d_float(0.f, 0.f, 1.f), ggo::point3d_float(5.f, 0.f, 0.f), ggo::point3d_float(5.f, 5.f, 0.f));
  scene_builder.add_object(parallelogram1, ggo::color::RED, false);
  
  auto parallelogram2 = std::make_shared<ggo::parallelogram3d_float>(ggo::point3d_float(-11.f, 0.f, 1.f), ggo::point3d_float(5.f, 0.f, 0.f), ggo::point3d_float(5.f, 5.f, 0.f));
  scene_builder.add_object(parallelogram2, ggo::color::RED, false);
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * SIZE * SIZE);
  renderer.render(buffer.data(), SIZE, SIZE, scene_builder);
  ggo::save_bmp("parallelogram3d.bmp", buffer.data(), SIZE, SIZE);
}
