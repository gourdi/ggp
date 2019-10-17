#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <2d/io/ggo_bmp.h>
#include <2d/paint/ggo_paint.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>
#include <raytracer/raycasters/ggo_octree_raycaster.h>

GGO_TEST(octree, test)
{
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::black_32f()));

  // Light.
  scene.add_point_light(ggo::white_32f(), ggo::pos3_f(100.f, -100.f, 200.f));

  // Objects.
  constexpr uint32_t discard_flags = ggo::discard_basis | ggo::discard_phong | ggo::discard_roughness;

  for (int z = -2; z <= 2; ++z)
  {
    for (int y = -2; y <= 2; ++y)
    {
      for (int x = -2; x <= 2; ++x)
      {
        ggo::sphere3d_f sphere({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(z) }, 0.4f);
        auto & sphere_obj = scene.add_diffuse_object<discard_flags>(sphere, ggo::red_material());
        sphere_obj.set_reflection_factor(0.5f);
      }
    }
  }

  // Rendering.
  const int width = 640;
  const int height = 480;
  
  ggo::basis3d_f camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_f::O_X(), 0.75f);
  camera_basis.rotate(ggo::ray3d_f::O_Z(), 0.5f);

  ggo::mono_sampling_point_camera camera(width, height, camera_basis, 0.1f);
  ggo::mono_sampling_renderer renderer(camera);

  // Brute force rendering.
  auto img1 = renderer.render(scene, { width, height });

  // Octree rendering.
  ggo::raytrace_params params;
  ggo::octree_raycaster raycaster(scene.solid_objects());
  params._raycaster = &raycaster;

  auto img2 = renderer.render(scene, { width, height }, params);
  ggo::save_bmp("octree_raycaster.bmp", img2);

  // Check octree and brute force rendering.
  GGO_CHECK_EQ(img1.size(), img2.size());
  GGO_CHECK_EQ(img1.pixel_type(), img2.pixel_type());
  GGO_CHECK_EQ(img1.memory_lines_order(), img2.memory_lines_order());
  GGO_CHECK_EQ(img1.line_byte_step(), 3 * width);
  GGO_CHECK_EQ(img2.line_byte_step(), 3 * width);
  GGO_CHECK(memcmp(img1.data(), img2.data(), img1.height() * img1.line_byte_step()) == 0);
}