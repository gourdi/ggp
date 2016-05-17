#include <iostream>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_background3d_color.h>
#include <ggo_object3d.h>

void test_polygon3d()
{
  const int GGO_SIZE_X = 400;
  const int GGO_SIZE_Y = 400;
  
  // The camera.
  ggo::mono_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0, 0, 50);
  camera.basis().rotate(ggo::ray3d_float::O_X(), ggo::PI<float>() / 2);
  camera.set_aperture(0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLACK));

  // Light.  
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(0, 0, 50));
  scene_builder.add_point_light(ggo::color::WHITE, ggo::point3d_float(50, 0, 0));

  // Objects.
  auto sphere = ggo::polygon3d_float::create_sphere(2, 16, 8);
  auto polygon = std::make_shared<ggo::polygon3d_float>(sphere);
  scene_builder.add_object(polygon, ggo::color::RED, false);
  
  // Rendering.
  ggo::mono_sampling_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  renderer.render(buffer.data(), GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("sphere3d.bmp", buffer.data(), GGO_SIZE_X, GGO_SIZE_Y);
}
