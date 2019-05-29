#include "ggo_kame_animation_artist.h"
#include <kernel/memory/ggo_array.h>
#include <kernel/ggo_unordered_pair.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/ggo_color.h>
#include <2d/processing/ggo_gaussian_blur.h>
#include <2d/ggo_blit.h>

namespace
{
  ggo::rgb_8u sub_blend(ggo::rgb_8u bkgd_color, ggo::rgb_8u brush_color)
  {
    int r = static_cast<int>(bkgd_color.r()) - static_cast<int>(brush_color.r());
    int g = static_cast<int>(bkgd_color.g()) - static_cast<int>(brush_color.g());
    int b = static_cast<int>(bkgd_color.b()) - static_cast<int>(brush_color.b());

    return {
      static_cast<uint8_t>(std::max(0, r)),
      static_cast<uint8_t>(std::max(0, g)),
      static_cast<uint8_t>(std::max(0, b)) };
  }
}

//////////////////////////////////////////////////////////////
ggo::kame_animation_artist::kame_animation_artist(int width, int height, int line_step, ggo::image_format format)
:
fixed_frames_count_animation_artist_abc(width, height, line_step, format, 200)
{
  _bkgd_colors[0] = ggo::rand<uint8_t>(0xdd, 0xff);
  _bkgd_colors[1] = ggo::rand<uint8_t>(0xdd, 0xff);
  _bkgd_colors[2] = ggo::rand<uint8_t>(0xdd, 0xff);
  _bkgd_colors[3] = ggo::rand<uint8_t>(0xdd, 0xff);

  for (int i = 256; i > 0; --i)
  {
    auto kame = create_kame();
    kame->_center = i == 1 ? get_center() : get_random_point();
    kame->_scale = 1.5f * min_size() / (i + 16);
    kame->_thickness = 0.01f * min_size() / (i + 16);

    _kames.emplace_back(std::move(kame));
  }
}

//////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<ggo::kame_animation_artist::timed_triangle>> ggo::kame_animation_artist::split_triangles(
  const std::vector<std::unique_ptr<timed_triangle>> & triangles,
  std::vector<std::unique_ptr<timed_vertex>> & vertices)
{
  std::vector<std::unique_ptr<timed_triangle>> result;

  // Find a create a vertex.
  auto new_vertex = [&](const ggo::pos3_f & p)
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
std::unique_ptr<ggo::kame_animation_artist::kame> ggo::kame_animation_artist::create_kame()
{
  auto new_kame = std::make_unique<kame>();

  // Vertices.
  const float phi = (1.f + std::sqrt(5.f)) / 2.f;

  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(+phi, +1.f, 0.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(-phi, +1.f, 0.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(+phi, -1.f, 0.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(-phi, -1.f, 0.f)));

  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(+1.f, 0.f, +phi)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(+1.f, 0.f, -phi)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(-1.f, 0.f, +phi)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(-1.f, 0.f, -phi)));

  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(0.f, +phi, +1.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(0.f, -phi, +1.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(0.f, +phi, -1.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3_f(0.f, -phi, -1.f)));

  // Triangles.
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[0].get(), new_kame->_vertices[2].get(), new_kame->_vertices[4].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[0].get(), new_kame->_vertices[2].get(), new_kame->_vertices[5].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[0].get(), new_kame->_vertices[4].get(), new_kame->_vertices[8].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[0].get(), new_kame->_vertices[5].get(), new_kame->_vertices[10].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[0].get(), new_kame->_vertices[8].get(), new_kame->_vertices[10].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[1].get(), new_kame->_vertices[3].get(), new_kame->_vertices[6].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[1].get(), new_kame->_vertices[3].get(), new_kame->_vertices[7].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[1].get(), new_kame->_vertices[6].get(), new_kame->_vertices[8].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[1].get(), new_kame->_vertices[7].get(), new_kame->_vertices[10].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[1].get(), new_kame->_vertices[8].get(), new_kame->_vertices[10].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[2].get(), new_kame->_vertices[4].get(), new_kame->_vertices[9].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[2].get(), new_kame->_vertices[5].get(), new_kame->_vertices[11].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[2].get(), new_kame->_vertices[9].get(), new_kame->_vertices[11].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[3].get(), new_kame->_vertices[6].get(), new_kame->_vertices[9].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[3].get(), new_kame->_vertices[7].get(), new_kame->_vertices[11].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[3].get(), new_kame->_vertices[9].get(), new_kame->_vertices[11].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[4].get(), new_kame->_vertices[6].get(), new_kame->_vertices[8].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[4].get(), new_kame->_vertices[6].get(), new_kame->_vertices[9].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[5].get(), new_kame->_vertices[7].get(), new_kame->_vertices[10].get()));
  new_kame->_triangles.push_back(std::make_unique<timed_triangle>(new_kame->_vertices[5].get(), new_kame->_vertices[7].get(), new_kame->_vertices[11].get()));

  new_kame->_triangles = split_triangles(new_kame->_triangles, new_kame->_vertices);
  new_kame->_triangles = split_triangles(new_kame->_triangles, new_kame->_vertices);
  new_kame->_triangles = split_triangles(new_kame->_triangles, new_kame->_vertices);

  for (auto & vertex : new_kame->_vertices)
  {
    vertex->set_length(2.f);
  }

  // Make normals point outwards.
  for (auto & triangle : new_kame->_triangles)
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
    ggo::vec3_f main_dir{ ggo::rand<float>(-1.f, 1.f), ggo::rand<float>(-1.f, 1.f), ggo::rand<float>(-1.f, 1.f) };
    main_dir = normalize(main_dir);

    const float scale_max = ggo::rand(0.3f, 0.6f);
    const float angle_max = ggo::rand(0.6f, 1.2f);

    for (auto & vertex : new_kame->_vertices)
    {
      const float angle = std::acos(ggo::dot(normalize(vertex->_cur), main_dir));
      const float scale = angle > angle_max ? 1.f : 1.f + scale_max * ggo::ease_inout(ggo::map(angle, 0.f, angle_max, 1.f, 0.f));
      const float length = scale * ggo::length(vertex->_cur);

      vertex->set_length(length);
    }
  }

  // Neighbors.
  for (const auto & vertex : new_kame->_vertices)
  {
    for (const auto & triangle : new_kame->_triangles)
    {
      if (triangle->_v1 == vertex.get())
      {
        ggo::push_once(new_kame->_neighbors[vertex.get()], triangle->_v2);
        ggo::push_once(new_kame->_neighbors[vertex.get()], triangle->_v3);
      }
      else if (triangle->_v2 == vertex.get())
      {
        ggo::push_once(new_kame->_neighbors[vertex.get()], triangle->_v1);
        ggo::push_once(new_kame->_neighbors[vertex.get()], triangle->_v3);
      }
      else if (triangle->_v3 == vertex.get())
      {
        ggo::push_once(new_kame->_neighbors[vertex.get()], triangle->_v1);
        ggo::push_once(new_kame->_neighbors[vertex.get()], triangle->_v2);
      }
    }
  }

  return new_kame;
}

//////////////////////////////////////////////////////////////
ggo::pos2_f ggo::kame_animation_artist::kame::proj(const ggo::pos2_f & p) const
{
  return _center + _scale * (p + _disp);
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::kame::update()
{
  _disp = _disp_interpolator.update(1);

  // Wave equation.
  for (auto & vertex : _vertices)
  {
    float laplacian = 0.f;
    const auto & neighbors = _neighbors[vertex.get()];
    for (const auto * neighbor : neighbors)
    {
      laplacian += ggo::length(neighbor->_cur);
    }
    laplacian /= neighbors.size();
    laplacian -= ggo::length(vertex->_cur);

    const float dist_prv = ggo::length(vertex->_prv);
    const float dist_cur = ggo::length(vertex->_cur);

    vertex->_laplacian = (2 * dist_cur - dist_prv + laplacian) * ggo::normalize(vertex->_laplacian);
  }

  // Averaging.
  for (auto & vertex : _vertices)
  {
    float neightbors_mean = 0.f;
    const auto & neighbors = _neighbors[vertex.get()];
    for (const auto * neighbor : neighbors)
    {
      neightbors_mean += ggo::length(neighbor->_laplacian);
    }
    neightbors_mean /= neighbors.size();

    vertex->_smoothed = (0.8f * ggo::length(vertex->_laplacian) + 0.2f * neightbors_mean) * normalize(vertex->_smoothed);
  }

  for (auto & vertex : _vertices)
  {
    vertex->_prv = vertex->_cur;
    vertex->_cur = vertex->_smoothed;
  }
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::kame::paint(void * buffer, const animation_artist_abc & artist) const
{
  std::vector<ggo::unordered_pair<const pos3_f *>> edges;

  std::vector<ggo::static_paint_shape<ggo::triangle2d_f, uint8_t>> triangles;

  for (const auto & triangle : _triangles)
  {
    if (triangle->get_normal().z() > 0.0f)
    {
      ggo::push_once(edges, ggo::unordered_pair<const pos3_f *>(&triangle->_v1->_prv, &triangle->_v2->_prv));
      ggo::push_once(edges, ggo::unordered_pair<const pos3_f *>(&triangle->_v2->_prv, &triangle->_v3->_prv));
      ggo::push_once(edges, ggo::unordered_pair<const pos3_f *>(&triangle->_v3->_prv, &triangle->_v1->_prv));

      auto proj1 = proj({ triangle->_v1->_prv.x(), triangle->_v1->_prv.y() });
      auto proj2 = proj({ triangle->_v2->_prv.x(), triangle->_v2->_prv.y() });
      auto proj3 = proj({ triangle->_v3->_prv.x(), triangle->_v3->_prv.y() });

      auto normal = ggo::normalize(ggo::cross(triangle->_v1->_prv - triangle->_v3->_prv, triangle->_v2->_prv - triangle->_v3->_prv));
      uint8_t color = ggo::round_to<uint8_t>(std::pow(std::abs(normal.z()), 0.5f) * 0xff);

      triangles.emplace_back(ggo::triangle2d_f(proj1, proj2, proj3), color);
    }
  }

  ggo::paint_shapes<ggo::y_8u_yu, ggo::sampling_1>(buffer, artist.width(), artist.height(), artist.width(), triangles);

  using paint_shape_t = ggo::static_paint_shape<ggo::capsule_f, uint8_t>;

  std::vector<paint_shape_t> shapes;
  for (const auto & edge : edges)
  {
    auto proj1 = proj({ edge._first->x(), edge._first->y() });
    auto proj2 = proj({ edge._second->x(), edge._second->y() });

    shapes.push_back(paint_shape_t({ proj1, proj2, _thickness }, 0x00));
  }

  ggo::paint_shapes<ggo::y_8u_yu, ggo::sampling_16x16>(buffer, artist.width(), artist.height(), artist.width(), shapes);
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::render_frame(void * buffer, int frame_index, float time_step)
{
  for (auto & kame : _kames)
  {
    kame->update();
  }

  if (buffer != nullptr)
  {
    ggo::array_8u buffer_gray(width() * height());

    ggo::fill_4_colors<ggo::y_8u_yu>(buffer_gray.data(), width(), height(), width(),
      _bkgd_colors[0], _bkgd_colors[1], _bkgd_colors[2], _bkgd_colors[3]);

    float stddev = 0.001f * min_size();

    for (const auto & kame : _kames)
    {
      kame->paint(buffer_gray.data(), *this);

      ggo::gaussian_blur<ggo::y_8u_yu>(buffer_gray.data(), size(), width(), stddev);
    }

    ggo::blit<ggo::y_8u_yu, ggo::rgb_8u_yu>(buffer_gray.data(), width(), height(), width(),
      buffer, width(), height(), line_step(), 0, 0);
  }
}

