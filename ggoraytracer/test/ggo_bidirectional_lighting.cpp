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
  const int GGO_SIZE_X = 480;
  const int GGO_SIZE_Y = 480;
  const int GGO_DIRECT_LIGHTING_SAMPLES_COUNT = 96;
  const int GGO_INDIRECT_LIGHTING_SAMPLES_COUNT = 128;
  
  // The camera.
  ggo::multi_sampling_point_camera camera(GGO_SIZE_X, GGO_SIZE_Y);
  camera.basis().set_pos(0, 0, 2.5);
  camera.set_aperture(0.5);
  camera.set_depth_of_field_factor(0.01f);
  camera.set_depth_of_field(2.5f);
  
  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::color::BLUE));
  
  // Light.
  scene_builder.add_sphere_light(ggo::color(0.9f), 0.2f, ggo::point3d_float(0, 0.75f, 0));

  // Objects.
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(1.f, 0.f, 0.f, 1.f), ggo::color::RED, true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(1.f, 0.f, 0.f, -1.f), ggo::color::GREEN, true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(0.f, 0.f, 1.f, 1.f), ggo::color::WHITE, true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(0.f, 1.f, 0.f, 1.f), ggo::color::WHITE, true);
  scene_builder.add_object(std::make_shared<ggo::plane3d<float>>(0.f, -1.f, 0.f, 1.f), ggo::color::WHITE, true);
  scene_builder.add_object(std::make_shared<ggo::sphere3d<float>>(ggo::point3d_float(0, -0.6f, 0), 0.4f), ggo::color::WHITE, true);

  // Rendering.
  ggo::global_sampling_renderer renderer(camera, GGO_DIRECT_LIGHTING_SAMPLES_COUNT);
  ggo::array_uint8 buffer(3 * GGO_SIZE_X * GGO_SIZE_Y);
  
  // Without indirect lighting.
  renderer.render(buffer, GGO_SIZE_X, GGO_SIZE_Y, scene_builder);
  ggo::save_bmp("bidirectional_lighting_off.bmp", buffer, GGO_SIZE_X, GGO_SIZE_Y);
  
  // With indirect lighting.
  ggo::scene scene = scene_builder.build_scene();
  ggo::brute_force_raycaster raycaster(scene.objects());
  ggo::bidirectional_lighting indirect_lighting(camera, raycaster, GGO_INDIRECT_LIGHTING_SAMPLES_COUNT);
  ggo::raytrace_params raytrace_params;
  raytrace_params._indirect_lighting = &indirect_lighting;
  
  renderer.render(buffer, GGO_SIZE_X, GGO_SIZE_Y, scene, raytrace_params);
  ggo::save_bmp("bidirectional_lighting_on.bmp", buffer, GGO_SIZE_X, GGO_SIZE_Y);
}

