#include <array>
#include <ggo_nonreg.h>
#include <ggo_marching_cubes.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_object3d.h>
#include <ggo_point_camera.h>
#include <ggo_antialiasing_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(marching_cubes, test)
{
  const int width = 512;
  const int height = 512;
  
  // The camera.
  ggo::antialiasing_point_camera camera(width, height);
  camera.basis().set_pos(0, 0, 15);
  camera.basis().rotate(ggo::ray3d_float::O_X(), ggo::pi<float>() / 4);
  camera.basis().rotate(ggo::ray3d_float::O_Z(), -ggo::pi<float>() / 4);
  camera.set_aperture(0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color_32f::BLUE));
  
  // Light.
  scene_builder.add_point_light(ggo::color_32f::WHITE, camera.basis().pos());
  
  // Objects.
  auto cells = ggo::marching_cubes([](float x, float y, float z) { return x * x + y * y + z * z - 1; }, ggo::pos3f(-2.f, -2.f, -2.f), 10, 0.4f);

  for (const auto & cell : cells)
  {
    for (const auto & triangle : cell._triangles)
    {
      auto face_ptr = std::make_shared<ggo::face3d<float, true>>(ggo::vertex<float>(triangle.v1(), triangle.v1()),
                                                                 ggo::vertex<float>(triangle.v2(), triangle.v2()),
                                                                 ggo::vertex<float>(triangle.v3(), triangle.v3()));

      scene_builder.add_object(face_ptr, ggo::color_32f::WHITE, true);
    }
  }
  
  // Rendering.
  ggo::antialiasing_renderer renderer(camera);
  ggo::array_uint8 buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, scene_builder);
  ggo::save_bmp("marching_cubes.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}
