#include <array>
#include <ggo_nonreg.h>
#include <ggo_marching_cubes.h>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_object3d.h>
#include <ggo_point_camera.h>
#include <ggo_antialiasing_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>

GGO_TEST(marching_cubes, test)
{
#if 0
  const int width = 512;
  const int height = 512;
  
  // The camera.
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 15.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), ggo::pi<float>() / 4);
  camera_basis.rotate(ggo::ray3d_float::O_Z(), -ggo::pi<float>() / 4);
  ggo::antialiasing_point_camera camera(width, height, camera_basis, 0.1f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::blue<ggo::color_32f>()));
  
  // Light.
  scene_builder.add_point_light(ggo::white<ggo::color_32f>(), camera.basis().pos());
  
  // Objects.
  auto cells = ggo::marching_cubes([](float x, float y, float z) { return x * x + y * y + z * z - 1; }, ggo::pos3f(-2.f, -2.f, -2.f), 10, 0.4f);

  for (const auto & cell : cells)
  {
    for (const auto & triangle : cell._triangles)
    {
      auto face_ptr = std::make_shared<ggo::face3d<float, true>>(ggo::vertex<float>(triangle.v1(), triangle.v1()),
                                                                 ggo::vertex<float>(triangle.v2(), triangle.v2()),
                                                                 ggo::vertex<float>(triangle.v3(), triangle.v3()));

      scene_builder.add_object(face_ptr, ggo::white<ggo::color_32f>(), true);
    }
  }
  
  // Rendering.
  ggo::antialiasing_renderer renderer(camera);
  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene_builder);
  ggo::save_bmp("marching_cubes.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
#endif
}
