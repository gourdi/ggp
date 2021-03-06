#include <kernel/nonreg/ggo_nonreg.h>
#include <kernel/memory/ggo_array.h>
#include <kernel/math/shapes_2d/ggo_shapes2d.h>
#include <2d/io/ggo_bmp.h>
#include <2d/paint/ggo_paint.h>
#include <raytracer/cameras/ggo_point_camera.h>
#include <raytracer/renderers/ggo_mono_sampling_renderer.h>
#include <raytracer/materials/ggo_solid_color_material.h>
#include <raytracer/backgrounds/ggo_background3d_color.h>

GGO_TEST(test_scene, scene3)
{
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::black_32f()));
  
  // Light.
  scene.add_point_light(ggo::white_32f(), ggo::pos3_f(0.f, 0.f, 200.f));

  // Objects.
  constexpr uint32_t discard_flags = ggo::discard_basis | ggo::discard_phong | ggo::discard_roughness;

  ggo::sphere3d_f sphere1({ std::cos(0.f), std::sin(0.f), 0.f }, 0.8f);
  auto & sphere1_obj = scene.add_diffuse_object<discard_flags>(sphere1, ggo::red_material());
  sphere1_obj.set_reflection_factor(0.5f);

  ggo::sphere3d_f sphere2({ std::cos(2.f * ggo::pi<float>() / 3.f), std::sin(2.f * ggo::pi<float>() / 3.f), 0.f }, 0.8f);
  auto & sphere2_obj = scene.add_diffuse_object<discard_flags>(sphere2, ggo::green_material());
  sphere2_obj.set_reflection_factor(0.5f);
  
  ggo::sphere3d_f sphere3({ std::cos(-2.f * ggo::pi<float>() / 3.f), std::sin(-2.f * ggo::pi<float>() / 3.f), 0.f }, 0.8f);
  auto & sphere3_obj = scene.add_diffuse_object<discard_flags>(sphere3, ggo::blue_material());
  sphere3_obj.set_reflection_factor(0.5f);

  // Rendering.
  const int width = 640;
  const int height = 480;

  ggo::mono_sampling_point_camera camera(width, height, ggo::basis3d_f({ 0.f, 0.f, 20.f }), 0.1f);
  ggo::mono_sampling_renderer renderer(camera);
  auto img = renderer.render(scene, { width, height });

  // Overlay, to check that basis::project is working as expected.
  ggo::image_t<ggo::pixel_type::rgb_8u, ggo::lines_order::up> view(img.data(), img.size(), img.line_byte_step());
  ggo::pos2_f proj1 = camera.basis().project(sphere1.center(), camera.get_aperture(), width, height);
  ggo::paint<ggo::sampling_4x4>(view, ggo::disc_f(proj1, 5.f), ggo::white_8u());
  ggo::pos2_f proj2 = camera.basis().project(sphere2.center(), camera.get_aperture(), width, height);
  ggo::paint<ggo::sampling_4x4>(view, ggo::disc_f(proj2, 5.f), ggo::white_8u());

  ggo::save_bmp("scene3.bmp", img);
}