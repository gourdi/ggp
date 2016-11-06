#include <ggo_nonreg.h>
#include <ggo_array.h>
#include <ggo_bmp.h>
#include <ggo_point_camera.h>
#include <ggo_object3d.h>
#include <ggo_brute_force_raycaster.h>
#include <ggo_global_sampling_renderer.h>
#include <ggo_solid_color_material.h>
#include <ggo_background3d_color.h>
#include <ggo_bidirectional_lighting.h>

GGO_TEST(bidirectional_lighting, test)
{
  const int width = 480;
  const int height = 480;
  const int direct_lighting_samples_coun = 96;
  
  // The camera.
  ggo::multi_sampling_point_camera camera(width, height);
  camera.basis().set_pos(0, 0, 2.5);
  camera.set_aperture(0.5);
  camera.set_depth_of_field_factor(0.01f);
  camera.set_depth_of_field(2.5f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::blue<ggo::color_32f>()));
  
  // Light.
  scene_builder.add_sphere_light(ggo::color_32f(0.9f), 0.2f, ggo::pos3f(0.f, 0.75f, 0.f));

  // Objects.
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(ggo::vec3f( 1.f, 0.f, 0.f), -1.f), ggo::red<ggo::color_32f>(), true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(ggo::vec3f(-1.f, 0.f, 0.f), -1.f), ggo::green<ggo::color_32f>(), true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(ggo::vec3f( 0.f, 0.f, 1.f), -1.f), ggo::white<ggo::color_32f>(), true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(ggo::vec3f( 0.f, 1.f, 0.f), -1.f), ggo::white<ggo::color_32f>(), true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(ggo::vec3f( 0.f, -1.f, 0.f), -1.f), ggo::white<ggo::color_32f>(), true);
  scene_builder.add_object(std::make_shared<ggo::sphere3d<float>>(ggo::pos3f(0.f, -0.6f, 0.f), 0.4f), ggo::white<ggo::color_32f>(), true);

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, direct_lighting_samples_coun);
  ggo::array_uint8 buffer(3 * width * height);
  
  // Without indirect lighting.
  renderer.render(buffer.data(), width, height, scene_builder);
  ggo::save_bmp("bidirectional_lighting_off.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
  
  // With indirect lighting.
  ggo::scene scene = scene_builder.build_scene();
  ggo::brute_force_raycaster raycaster(scene.objects());
  ggo::bidirectional_lighting indirect_lighting(raycaster, scene);
  ggo::raytrace_params raytrace_params;
  raytrace_params._indirect_lighting = &indirect_lighting;
  
  renderer.render(buffer.data(), width, height, scene, raytrace_params);
  ggo::save_bmp("bidirectional_lighting_on.bmp", buffer.data(), ggo::rgb_8u_yu, width, height, 3 * width);
}

