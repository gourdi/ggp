#include <ggo_nonreg.h>
#include <ggo_buffer.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_antialiasing_renderer.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_background3d_color.h>
#include <ggo_solid_color_material.h>

GGO_TEST(test_scene, scene1)
{ 
  // The scene.
  ggo::scene scene(std::make_shared<ggo::background3d_color>(ggo::blue<ggo::color_32f>()));

  // Light.
  scene.add_point_light(ggo::white_32f(), { -20.f, -20.f, 50.f });

  // Objects.
  ggo::sphere3d_float sphere({ -2.f, 0.f, 0.f }, 1.f);
  auto & sphere_object = scene.add_diffuse_object
    <ggo::discard_basis | ggo::discard_roughness | ggo::discard_reflection, ggo::sphere3d_float, ggo::solid_color_material>
    (sphere, ggo::red_32f());
  sphere_object.set_phong(4, 250);
  
  ggo::aabox3d_float box_shape(-0.7f, 0.7f, -0.8f, 0.8f, -0.9f, 0.9f);
  auto & box_object = scene.add_diffuse_object
    <ggo::discard_roughness | ggo::discard_phong | ggo::discard_reflection, ggo::aabox3d_float, ggo::solid_color_material>
    (box_shape, ggo::green_32f());
  box_object.basis().rotate(ggo::ray3d_float::O_X(), 0.1f);
  box_object.basis().rotate(ggo::ray3d_float::O_Y(), 0.2f);
  box_object.basis().rotate(ggo::ray3d_float::O_Z(), 0.3f);
  box_object.basis().move(2, 0, 1);

  ggo::plane3d_float plane({ 0.f, 0.f, 1.f }, -1.f);
  scene.add_diffuse_object
    <ggo::discard_basis | ggo::discard_roughness | ggo::discard_phong | ggo::discard_reflection, ggo::plane3d_float, ggo::solid_color_material>
    (plane, ggo::white_32f());

  // Rendering.
  const int width = 640;
  const int height = 480;
  const int samples_count = 92;

  ggo::basis3d_float camera_basis({ 0.f, 0.f, 50.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 1.5f);

  ggo::antialiasing_point_camera camera(width, height, camera_basis, 0.1f);
  ggo::antialiasing_renderer renderer(camera);

  ggo::buffer buffer(3 * width * height);
  renderer.render(buffer.data(), width, height, 3 * width, ggo::rgb_8u_yu, scene);

  ggo::save_bmp("scene1.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}