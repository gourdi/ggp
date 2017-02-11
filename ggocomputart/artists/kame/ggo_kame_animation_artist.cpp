#include "ggo_kame_animation_artist.h"
#include <ggo_link.h>
#include <ggo_buffer_fill.h>
#include <ggo_multi_shape_paint.h>

namespace
{
  const int frames_count = 300;
  const float projection_range = 4.0f;
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
  _frame_index = -1;

  _vertices.clear();
  _triangles.clear();
  _neighbors.clear();

  _color = ggo::from_hsv<ggo::color_32f>(ggo::rand<float>(), ggo::rand<float>(0.25f, 1.f), 1.f);

  // Glows.
  _foreground_glows.clear();
  _background_glows.clear();
  float width_32f = static_cast<float>(get_width());
  for (int i = 0; i < 200; ++i)
  {
    _foreground_glows.push_back({
      ggo::pos2f(ggo::rand<float>(-width_32f, width_32f), get_height() * ggo::rand<float>(-0.1f, 1.1f)),
      ggo::rand<float>(0.01f, 0.02f) * get_min_size(),
      ggo::rand<float>(128.f, 256.f),
      ggo::rand<float>(0.01f, 0.02f) * get_min_size() });
  }
  for (int i = 0; i < 200; ++i)
  {
    _background_glows.push_back({
      ggo::pos2f(ggo::rand<float>(-width_32f, width_32f), get_height() * ggo::rand<float>(-0.1f, 1.1f)),
      ggo::rand<float>(0.01f, 0.02f) * get_min_size(),
      ggo::rand<float>(128.f, 256.f),
      ggo::rand<float>(0.01f, 0.02f) * get_min_size() });
  }

  // Vertices.
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

  // Triangles.
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[0].get(), _vertices[2].get(), _vertices[4].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[0].get(), _vertices[2].get(), _vertices[5].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[0].get(), _vertices[4].get(), _vertices[8].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[0].get(), _vertices[5].get(), _vertices[10].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[0].get(), _vertices[8].get(), _vertices[10].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[1].get(), _vertices[3].get(), _vertices[6].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[1].get(), _vertices[3].get(), _vertices[7].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[1].get(), _vertices[6].get(), _vertices[8].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[1].get(), _vertices[7].get(), _vertices[10].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[1].get(), _vertices[8].get(), _vertices[10].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[2].get(), _vertices[4].get(), _vertices[9].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[2].get(), _vertices[5].get(), _vertices[11].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[2].get(), _vertices[9].get(), _vertices[11].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[3].get(), _vertices[6].get(), _vertices[9].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[3].get(), _vertices[7].get(), _vertices[11].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[3].get(), _vertices[9].get(), _vertices[11].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[4].get(), _vertices[6].get(), _vertices[8].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[4].get(), _vertices[6].get(), _vertices[9].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[5].get(), _vertices[7].get(), _vertices[10].get()));
  _triangles.push_back(std::make_unique<timed_triangle>(_vertices[5].get(), _vertices[7].get(), _vertices[11].get()));

  _triangles = split_triangles(_triangles, _vertices);
  _triangles = split_triangles(_triangles, _vertices);
  _triangles = split_triangles(_triangles, _vertices);

  for (auto & vertex : _vertices)
  {
    vertex->set_length(2.f);
  }

  // Make normals point outwards.
  for (auto & triangle : _triangles)
  {
    auto barycenter = triangle->_v1->_prv + triangle->_v2->_prv + triangle->_v3->_prv;

    if (ggo::dot(barycenter, triangle->get_normal()) < 0.f)
    {
      std::swap(triangle->_v2, triangle->_v3);
    }
  }

  // Add some random bump.
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

  // Neighbors.
  for (const auto & vertex : _vertices)
  {
    for (const auto & triangle : _triangles)
    {
      if (triangle->_v1 == vertex.get())
      {
        ggo::push_once(_neighbors[vertex.get()], triangle->_v2);
        ggo::push_once(_neighbors[vertex.get()], triangle->_v3);
      }
      else if (triangle->_v2 == vertex.get())
      {
        ggo::push_once(_neighbors[vertex.get()], triangle->_v1);
        ggo::push_once(_neighbors[vertex.get()], triangle->_v3);
      }
      else if (triangle->_v3 == vertex.get())
      {
        ggo::push_once(_neighbors[vertex.get()], triangle->_v1);
        ggo::push_once(_neighbors[vertex.get()], triangle->_v2);
      }
    }
  }
}

//////////////////////////////////////////////////////////////
bool ggo::kame_animation_artist::update()
{
  ++_frame_index;

  if (_frame_index >= frames_count)
  {
    return false;
  }

  // Update glows.
  for (auto & glow : _background_glows)
  {
    glow._pos.x() += glow._speed;
  }
  for (auto & glow : _foreground_glows)
  {
    glow._pos.x() += glow._speed;
  }

  // Wave equation.
  for (auto & vertex : _vertices)
  {
    float laplacian = 0.f;
    const auto & neighbors = _neighbors[vertex.get()];
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

  // Averaging.
  for (auto & vertex : _vertices)
  {
    float neightbors_mean = 0.f;
    const auto & neighbors = _neighbors[vertex.get()];
    for (const auto * neighbor : neighbors)
    {
      neightbors_mean += neighbor->_laplacian.get_length();
    }
    neightbors_mean /= neighbors.size();

    vertex->_smoothed.set_length(0.8f * vertex->_laplacian.get_length() + 0.2f * neightbors_mean);
  }

  for (auto & vertex : _vertices)
  {
    vertex->_prv = vertex->_cur;
    vertex->_cur = vertex->_smoothed;
  }

  return true;
}

//////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<ggo::kame_animation_artist::timed_triangle>> ggo::kame_animation_artist::split_triangles(
  const std::vector<std::unique_ptr<timed_triangle>> & triangles,
  std::vector<std::unique_ptr<timed_vertex>> & vertices)
{
  std::vector<std::unique_ptr<timed_triangle>> result;

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
    result.push_back(std::make_unique<timed_triangle>(
      triangle->_v1,
      new_vertex(0.5f * (triangle->_v1->_cur + triangle->_v2->_cur)),
      new_vertex(0.5f * (triangle->_v1->_cur + triangle->_v3->_cur))));

    result.push_back(std::make_unique<timed_triangle>(
      triangle->_v2,
      new_vertex(0.5f * (triangle->_v2->_cur + triangle->_v1->_cur)),
      new_vertex(0.5f * (triangle->_v2->_cur + triangle->_v3->_cur))));

    result.push_back(std::make_unique<timed_triangle>(
      triangle->_v3,
      new_vertex(0.5f * (triangle->_v3->_cur + triangle->_v1->_cur)),
      new_vertex(0.5f * (triangle->_v3->_cur + triangle->_v2->_cur))));

    result.push_back(std::make_unique<timed_triangle>(
      new_vertex(0.5f * (triangle->_v1->_cur + triangle->_v2->_cur)),
      new_vertex(0.5f * (triangle->_v1->_cur + triangle->_v3->_cur)),
      new_vertex(0.5f * (triangle->_v2->_cur + triangle->_v3->_cur))));
  }

  return result;
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::render_frame(void * buffer, const ggo::pixel_rect & clipping)
{
  ggo::fill_solid<ggo::rgb_8u_yu>(buffer, get_width(), get_height(), get_line_step(), ggo::black_8u(), clipping);

  std::vector<ggo::dyn_paint_shape<float, ggo::color_8u, ggo::color_8u>> shapes;

  const float size = 0.001f * get_min_size();

  std::vector<ggo::link<const pos3f *>> edges;
  for (const auto & triangle : _triangles)
  {
    if (triangle->get_normal().z() > 0.0f)
    {
      ggo::push_once(edges, ggo::link<const pos3f *>(&triangle->_v1->_prv, &triangle->_v2->_prv));
      ggo::push_once(edges, ggo::link<const pos3f *>(&triangle->_v2->_prv, &triangle->_v3->_prv));
      ggo::push_once(edges, ggo::link<const pos3f *>(&triangle->_v3->_prv, &triangle->_v1->_prv));
    }
  }

  std::vector<ggo::solid_color_shape<ggo::triangle2d_float, ggo::color_8u>> triangles;
  for (const auto & triangle : _triangles)
  {
    auto proj1 = map_fit(ggo::pos2f(triangle->_v1->_prv.x(), triangle->_v1->_prv.y()), -projection_range, projection_range);
    auto proj2 = map_fit(ggo::pos2f(triangle->_v2->_prv.x(), triangle->_v2->_prv.y()), -projection_range, projection_range);
    auto proj3 = map_fit(ggo::pos2f(triangle->_v3->_prv.x(), triangle->_v3->_prv.y()), -projection_range, projection_range);

    triangles.emplace_back(ggo::triangle2d_float(proj1, proj2, proj3), ggo::black_8u());
  }

  for (const auto & glow : _background_glows)
  {
    paint_glow(glow, buffer);
  }

  ggo::paint_shapes<ggo::rgb_8u_yu, ggo::sampling_1>(buffer, get_width(), get_height(), get_line_step(), triangles.begin(), triangles.end(), clipping);

  for (const auto & edge : edges)
  {
    paint_glow_segment(*edge._v1, *edge._v2, buffer);
  }

  for (const auto & glow : _foreground_glows)
  {
    paint_glow(glow, buffer);
  }
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::paint_glow_segment(const ggo::pos3f & p1, const ggo::pos3f & p2, void * buffer) const
{
  auto proj1 = map_fit(ggo::pos2f(p1.x(), p1.y()), -projection_range, projection_range);
  auto proj2 = map_fit(ggo::pos2f(p2.x(), p2.y()), -projection_range, projection_range);

  const float radius = 0.01f * get_min_size();
  const float sq_radius = radius * radius;
  const float normalization = 128.f / sq_radius;

  ggo::rect<float> bounding_rect(proj1, proj2);
  bounding_rect.inflate(5.f);

  ggo::pixel_rect pixel_rect(bounding_rect.data());
  if (pixel_rect.clip(get_width(), get_height()) == false)
  {
    return;
  }

  const ggo::segment<float> segment(proj1, proj2);

  pixel_rect.for_each_pixel([&](int x, int y)
  {
    const ggo::pos2f p(static_cast<float>(x), static_cast<float>(y));
    const float hypot = segment.hypot_to_point(p);

    if (hypot < sq_radius)
    {
      const float dist = std::sqrt(hypot);
      const ggo::color_32f color = (normalization * ggo::square(dist - radius)) * _color;

      ggo::color_8u pixel8u = ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step());
      pixel8u.r() = ggo::to<uint8_t>(pixel8u.r() + color.r());
      pixel8u.g() = ggo::to<uint8_t>(pixel8u.g() + color.g());
      pixel8u.b() = ggo::to<uint8_t>(pixel8u.b() + color.b());

      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), pixel8u);
    }
  });
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::paint_glow(const glow & glow, void * buffer) const
{
  const float sq_radius = glow._radius * glow._radius;

  ggo::rect<float> bounding_rect(glow._pos, glow._pos);
  bounding_rect.inflate(glow._radius);

  ggo::pixel_rect pixel_rect(bounding_rect.data());
  if (pixel_rect.clip(get_width(), get_height()) == false)
  {
    return;
  }

  pixel_rect.for_each_pixel([&](int x, int y)
  {
    const ggo::pos2f p(static_cast<float>(x), static_cast<float>(y));
    const float hypot = ggo::hypot(p, glow._pos);

    if (hypot < sq_radius)
    {
      const float dist = std::sqrt(hypot);
      const ggo::color_32f color = (glow._intensity * ggo::square(dist - glow._radius) / sq_radius) * _color;

      ggo::color_8u pixel8u = ggo::read_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step());
      pixel8u.r() = ggo::to<uint8_t>(pixel8u.r() + color.r());
      pixel8u.g() = ggo::to<uint8_t>(pixel8u.g() + color.g());
      pixel8u.b() = ggo::to<uint8_t>(pixel8u.b() + color.b());

      ggo::write_pixel<ggo::rgb_8u_yu>(buffer, x, y, get_height(), get_line_step(), pixel8u);
    }
  });
}


