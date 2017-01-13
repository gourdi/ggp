#include "ggo_kame_animation_artist.h"
#include <ggo_point_camera.h>
#include <ggo_antialiasing_renderer.h>
#include <ggo_mono_sampling_renderer.h>
#include <ggo_background3d_color.h>

namespace
{
  const int frames_count = 300;

  class kame_raycaster : public ggo::raycaster_abc
  {
    const ggo::object3d * hit_test(const ggo::ray3d_float & ray, float & dist, ggo::ray3d_float & local_normal, ggo::ray3d_float & world_normal, const ggo::object3d * exclude_object1 = nullptr, const ggo::object3d * exclude_object2 = nullptr) const override
    {
      return nullptr;
    }

    bool check_visibility(const ggo::ray3d_float & ray, float dist_max, const ggo::object3d * exclude_object1 = nullptr, const ggo::object3d * exclude_object2 = nullptr) const override
    {
      return false;
    }
  };
}

//////////////////////////////////////////////////////////////
ggo::kame_animation_artist::kame_animation_artist(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
animation_artist_abc(width, height, line_step, pbf, rt)
{

}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::init()
{
  _vertices.clear();
  _triangles.clear();

  const float phi = (1.f + std::sqrt(5.f)) / 2.f;

  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+phi, +1.f, 0.f)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-phi, +1.f, 0.f)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+phi, -1.f, 0.f)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-phi, -1.f, 0.f)));

  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+1.f, 0.f, +phi)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+1.f, 0.f, -phi)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-1.f, 0.f, +phi)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-1.f, 0.f, -phi)));

  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, +phi, +1.f)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, -phi, +1.f)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, +phi, -1.f)));
  _vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, -phi, -1.f)));

  _triangles.push_back({ _vertices[0].get(), _vertices[2].get(), _vertices[4].get() });
  _triangles.push_back({ _vertices[0].get(), _vertices[2].get(), _vertices[5].get() });
  _triangles.push_back({ _vertices[0].get(), _vertices[4].get(), _vertices[8].get() });
  _triangles.push_back({ _vertices[0].get(), _vertices[5].get(), _vertices[10].get() });
  _triangles.push_back({ _vertices[0].get(), _vertices[8].get(), _vertices[10].get() });
  _triangles.push_back({ _vertices[1].get(), _vertices[3].get(), _vertices[6].get() });
  _triangles.push_back({ _vertices[1].get(), _vertices[3].get(), _vertices[7].get() });
  _triangles.push_back({ _vertices[1].get(), _vertices[6].get(), _vertices[8].get() });
  _triangles.push_back({ _vertices[1].get(), _vertices[7].get(), _vertices[10].get() });
  _triangles.push_back({ _vertices[1].get(), _vertices[8].get(), _vertices[10].get() });
  _triangles.push_back({ _vertices[2].get(), _vertices[4].get(), _vertices[9].get() });
  _triangles.push_back({ _vertices[2].get(), _vertices[5].get(), _vertices[11].get() });
  _triangles.push_back({ _vertices[2].get(), _vertices[9].get(), _vertices[11].get() });
  _triangles.push_back({ _vertices[3].get(), _vertices[6].get(), _vertices[9].get() });
  _triangles.push_back({ _vertices[3].get(), _vertices[7].get(), _vertices[11].get() });
  _triangles.push_back({ _vertices[3].get(), _vertices[9].get(), _vertices[11].get() });
  _triangles.push_back({ _vertices[4].get(), _vertices[6].get(), _vertices[8].get() });
  _triangles.push_back({ _vertices[4].get(), _vertices[6].get(), _vertices[9].get() });
  _triangles.push_back({ _vertices[5].get(), _vertices[7].get(), _vertices[10].get() });
  _triangles.push_back({ _vertices[5].get(), _vertices[7].get(), _vertices[11].get() });

  _triangles = split_triangles(_triangles, _vertices);
  _triangles = split_triangles(_triangles, _vertices);
  _triangles = split_triangles(_triangles, _vertices);

  for (auto & vertex : _vertices)
  {
    vertex->set_length(2.f);
  }

  for (int i = 0; i < 2; ++i)
  {
    ggo::vec3f main_dir{ ggo::rand<float>(-1.f, 1.f), ggo::rand<float>(-1.f, 1.f), ggo::rand<float>(-1.f, 1.f) };
    main_dir.normalize();

    const float scale_max = ggo::rand(0.2f, 0.4f);
    const float angle_max = ggo::rand(0.6f, 1.2f);

    for (auto & vertex : _vertices)
    {
      const float angle = std::acos(ggo::dot(vertex->_cur.get_normalized(), main_dir));
      const float scale = angle > angle_max ? 1.f : 1.f + scale_max * ggo::ease_inout(ggo::map(angle, 0.f, angle_max, 1.f, 0.f));
      const float length = scale * vertex->_cur.get_length();

      vertex->set_length(length);
    }
  }

  _frame_index = -1;
}

//////////////////////////////////////////////////////////////
bool ggo::kame_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  for (auto & vertex : _vertices)
  {
    std::vector<const timed_vertex *> neighbors;
    for (const auto & triangle : _triangles)
    {
      if (triangle._v1 == vertex.get())
      {
        ggo::push_once(neighbors, triangle._v2);
        ggo::push_once(neighbors, triangle._v3);
      }
      else if (triangle._v2 == vertex.get())
      {
        ggo::push_once(neighbors, triangle._v1);
        ggo::push_once(neighbors, triangle._v3);
      }
      else if (triangle._v3 == vertex.get())
      {
        ggo::push_once(neighbors, triangle._v1);
        ggo::push_once(neighbors, triangle._v2);
      }
    }

    float laplacian = 0.f;
    for (const auto * neighbor : neighbors)
    {
      laplacian += neighbor->_cur.get_length();
    }
    laplacian /= neighbors.size();
    laplacian -= vertex->_cur.get_length();

    const float dist_prv = vertex->_prv.get_length();
    const float dist_cur = vertex->_cur.get_length();

    vertex->_laplacian.set_length(2 * dist_cur - dist_prv + laplacian);
  }

  for (auto & vertex : _vertices)
  {
    std::vector<const timed_vertex *> neighbors;
    for (const auto & triangle : _triangles)
    {
      if (triangle._v1 == vertex.get())
      {
        ggo::push_once(neighbors, triangle._v2);
        ggo::push_once(neighbors, triangle._v3);
      }
      else if (triangle._v2 == vertex.get())
      {
        ggo::push_once(neighbors, triangle._v1);
        ggo::push_once(neighbors, triangle._v3);
      }
      else if (triangle._v3 == vertex.get())
      {
        ggo::push_once(neighbors, triangle._v1);
        ggo::push_once(neighbors, triangle._v2);
      }
    }

    float neightbors_mean = 0.f;
    for (const auto * neighbor : neighbors)
    {
      neightbors_mean += neighbor->_laplacian.get_length();
    }
    neightbors_mean /= neighbors.size();

    vertex->_smoothed.set_length(0.9f * vertex->_laplacian.get_length() + 0.1f * neightbors_mean);
  }

  for (auto & vertex : _vertices)
  {
    vertex->_prv = vertex->_cur;
    vertex->_cur = vertex->_smoothed;
  }

  return true;
}

//////////////////////////////////////////////////////////////
std::vector<ggo::kame_animation_artist::timed_triangle> ggo::kame_animation_artist::split_triangles(const std::vector<timed_triangle> & triangles, std::vector<std::unique_ptr<timed_vertex>> & vertices)
{
  std::vector<timed_triangle> result;

  // Find a create a vertex.
  auto new_vertex = [&](const ggo::pos3f & p)
  {
    for (const auto & vertex : vertices)
    {
      if (ggo::distance(p, vertex->_cur) < 0.001f)
      {
        return vertex.get();
      }
    }

    vertices.push_back(std::make_unique<timed_vertex>(p));
    return vertices.back().get();
  };

  // Split each triangle.
  for (const auto & triangle : triangles)
  {
    result.push_back({
      triangle._v1,
      new_vertex(0.5f * (triangle._v1->_cur + triangle._v2->_cur)),
      new_vertex(0.5f * (triangle._v1->_cur + triangle._v3->_cur)) });

    result.push_back({ 
      triangle._v2,
      new_vertex(0.5f * (triangle._v2->_cur + triangle._v1->_cur)),
      new_vertex(0.5f * (triangle._v2->_cur + triangle._v3->_cur)) });

    result.push_back({
      triangle._v3,
      new_vertex(0.5f * (triangle._v3->_cur + triangle._v1->_cur)),
      new_vertex(0.5f * (triangle._v3->_cur + triangle._v2->_cur)) });

    result.push_back({
      new_vertex(0.5f * (triangle._v1->_cur + triangle._v2->_cur)),
      new_vertex(0.5f * (triangle._v1->_cur + triangle._v3->_cur)),
      new_vertex(0.5f * (triangle._v2->_cur + triangle._v3->_cur)) });
  }

  return result;
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping)
{
  const float aperture = 0.1f;
  ggo::basis3d_float camera_basis({ 0.f, 0.f, 45.f });
  camera_basis.rotate(ggo::ray3d_float::O_X(), 0.4f * ggo::pi<float>());
#ifdef GGO_KAME_ANTIALIASING
  ggo::antialiasing_point_camera camera(get_width(), get_height(), camera_basis, aperture);
#else
  ggo::mono_sampling_point_camera camera(get_width(), get_height(), camera_basis, aperture);
#endif

  // The scene.
  ggo::scene_builder scene_builder(std::make_shared<ggo::background3d_color>(ggo::blue<ggo::color_32f>()));

  // Light.
  scene_builder.add_sphere_light(ggo::white<ggo::color_32f>(), 10.f, { 5.f, -20.f, 50.f });

  // Objects.
  for (const auto triangle : _triangles)
  {
    auto face = std::make_shared<ggo::triangle3d<float, true>>(triangle._v1->_prv, triangle._v2->_prv, triangle._v3->_prv);
    auto object = scene_builder.add_object(face, ggo::red_32f(), true);
    object->set_phong_factor(5.0f);
    object->set_phong_shininess(100.f);
  }
  auto plane = std::make_shared<ggo::const_plane3d<float, 0, 0, 1, -4>>();
  auto object = scene_builder.add_object(plane, ggo::white_32f(), true);
  object->set_roughness(0.25f);

  // Rendering.
#ifdef GGO_KAME_ANTIALIASING
  ggo::antialiasing_renderer renderer(camera);
#else
  ggo::mono_sampling_renderer renderer(camera);
#endif
  renderer.render(buffer, get_width(), get_height(), 3 * get_width(), ggo::rgb_8u_yu, scene_builder);
}

