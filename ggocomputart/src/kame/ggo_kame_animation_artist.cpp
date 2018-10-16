#include "ggo_kame_animation_artist.h"
#include <kernel/memory/ggo_array.h>
#include <2d/fill/ggo_fill.h>
#include <2d/paint/ggo_paint.h>
#include <2d/ggo_color.h>
#include <2d/blur/ggo_gaussian_blur.h>
#include <2d/ggo_blit.h>

namespace
{
  ggo::rgb_8u sub_blend(ggo::rgb_8u bkgd_color, ggo::rgb_8u brush_color)
  {
    int r = static_cast<int>(bkgd_color._r) - static_cast<int>(brush_color._r);
    int g = static_cast<int>(bkgd_color._g) - static_cast<int>(brush_color._g);
    int b = static_cast<int>(bkgd_color._b) - static_cast<int>(brush_color._b);

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
std::unique_ptr<ggo::kame_animation_artist::kame> ggo::kame_animation_artist::create_kame()
{
  auto new_kame = std::make_unique<kame>();

  // Vertices.
  const float phi = (1.f + std::sqrt(5.f)) / 2.f;

  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+phi, +1.f, 0.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-phi, +1.f, 0.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+phi, -1.f, 0.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-phi, -1.f, 0.f)));

  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+1.f, 0.f, +phi)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(+1.f, 0.f, -phi)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-1.f, 0.f, +phi)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(-1.f, 0.f, -phi)));

  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, +phi, +1.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, -phi, +1.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, +phi, -1.f)));
  new_kame->_vertices.push_back(std::make_unique<timed_vertex>(ggo::pos3f(0.f, -phi, -1.f)));

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
    ggo::vec3f main_dir{ ggo::rand<float>(-1.f, 1.f), ggo::rand<float>(-1.f, 1.f), ggo::rand<float>(-1.f, 1.f) };
    main_dir.normalize();

    const float scale_max = ggo::rand(0.3f, 0.6f);
    const float angle_max = ggo::rand(0.6f, 1.2f);

    for (auto & vertex : new_kame->_vertices)
    {
      const float angle = std::acos(ggo::dot(vertex->_cur.get_normalized(), main_dir));
      const float scale = angle > angle_max ? 1.f : 1.f + scale_max * ggo::ease_inout(ggo::map(angle, 0.f, angle_max, 1.f, 0.f));
      const float length = scale * vertex->_cur.get_length();

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
ggo::pos2f ggo::kame_animation_artist::kame::proj(const ggo::pos2f & p) const
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
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::kame::paint(void * buffer, const animation_artist_abc & artist) const
{
  std::vector<ggo::link<const pos3f *>> edges;

  std::vector<ggo::static_paint_shape<ggo::triangle2d_float, uint8_t>> triangles;

  for (const auto & triangle : _triangles)
  {
    if (triangle->get_normal().z() > 0.0f)
    {
      ggo::push_once(edges, ggo::link<const pos3f *>(&triangle->_v1->_prv, &triangle->_v2->_prv));
      ggo::push_once(edges, ggo::link<const pos3f *>(&triangle->_v2->_prv, &triangle->_v3->_prv));
      ggo::push_once(edges, ggo::link<const pos3f *>(&triangle->_v3->_prv, &triangle->_v1->_prv));

      auto proj1 = proj({ triangle->_v1->_prv.x(), triangle->_v1->_prv.y() });
      auto proj2 = proj({ triangle->_v2->_prv.x(), triangle->_v2->_prv.y() });
      auto proj3 = proj({ triangle->_v3->_prv.x(), triangle->_v3->_prv.y() });

      auto normal = ggo::cross(triangle->_v1->_prv - triangle->_v3->_prv, triangle->_v2->_prv - triangle->_v3->_prv);
      normal.normalize();
      uint8_t color = ggo::round_to<uint8_t>(std::pow(std::abs(normal.z()), 0.5f) * 0xff);

      triangles.emplace_back(ggo::triangle2d_float(proj1, proj2, proj3), color);
    }
  }

  ggo::paint_shapes<ggo::y_8u_yu, ggo::sampling_1>(buffer, artist.width(), artist.height(), artist.width(), triangles);

  using paint_shape_t = ggo::static_paint_shape<ggo::capsule_float, uint8_t>;

  std::vector<paint_shape_t> shapes;
  for (const auto & edge : edges)
  {
    auto proj1 = proj({ edge._v1->x(), edge._v1->y() });
    auto proj2 = proj({ edge._v2->x(), edge._v2->y() });

    shapes.push_back(paint_shape_t({ proj1, proj2, _thickness }, 0x00));
  }

  ggo::paint_shapes<ggo::y_8u_yu, ggo::sampling_16x16>(buffer, artist.width(), artist.height(), artist.width(), shapes);
}

//////////////////////////////////////////////////////////////
void ggo::kame_animation_artist::render_frame(void * buffer, int frame_index)
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

      ggo::gaussian_blur2d<ggo::y_8u_yu>(buffer_gray.data(), width(), size(), stddev);
    }

    ggo::blit<ggo::y_8u_yu, ggo::rgb_8u_yu>(buffer_gray.data(), width(), height(), width(),
      buffer, width(), height(), line_step(), 0, 0);
  }
}

