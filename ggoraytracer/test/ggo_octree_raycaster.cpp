#include <ggo_nonreg.h>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_shapes2d.h>
#include <ggo_point_camera.h>
#include <ggo_buffer_paint.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_octree_raycaster.h>

GGO_TEST(octree, test)
{
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::black_32f()));

  // Light.
  scene.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(100.f, -100.f, 200.f));
 // scene.add_point_light(ggo::white<ggo::color_32f>(), ggo::pos3f(-100.f, -100.f, 200.f));

  // Objects.
  constexpr uint32_t discard_flags = ggo::discard_basis | ggo::discard_phong | ggo::discard_roughness;

  for (int z = -2; z <= 2; ++z)
  {
    for (int y = -2; y <= 2; ++y)
    {
      for (int x = -2; x <= 2; ++x)
      {
        ggo::sphere3d_float sphere({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }, 0.4f);
        auto & sphere_obj = scene.add_diffuse_object<discard_flags>(sphere, ggo::red_material());
        sphere_obj.set_reflection_factor(0.5f);
      }
    }
  }

  // Rendering.
  const int width = 640;
  const int height = 480;
  
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 0.75f);
  camera_basis.rotate(ggo::ray3d_float::O_Z(), 0.2f);

  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);
  ggo::mono_sampling_renderer renderer(camera);

  // Brute force rendering.
  ggo::buffer buffer1(3 * width * height);
  renderer.render(buffer1.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);

  // Octree rendering.
  ggo::raytrace_params params;
  ggo::octree_raycaster raycaster(scene);
  params._raycaster = &raycaster;

  ggo::buffer buffer2(3 * width * height);
  renderer.render(buffer2.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene, params);
  ggo::save_bmp("octree_raycaster.bmp", buffer2.data(), ggo::rgb_8u_yu, width, height, 3 * width);

  // Check octree and brute force rendering.
  GGO_CHECK(buffer1 == buffer2);
}