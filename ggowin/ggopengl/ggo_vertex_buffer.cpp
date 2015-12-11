#include "ggo_vertex_buffer.h"

//////////////////////////////////////////////////////////////
void ggo_vertex_buffer::add_vertex(const ggo::point3d_float & pos, const ggo::color & color, const ggo::vector3d_float & normal)
{
  add_vertex(pos.x(), pos.y(), pos.z(), color.r(), color.g(), color.b(), normal.x(), normal.y(), normal.z());
}

//////////////////////////////////////////////////////////////
void ggo_vertex_buffer::add_vertex(float px, float py, float pz, float r, float g, float b, float nx, float ny, float nz)
{
  _buffer.push_back(px);
  _buffer.push_back(py);
  _buffer.push_back(pz);
  _buffer.push_back(1);

  _buffer.push_back(r);
  _buffer.push_back(g);
  _buffer.push_back(b);
  _buffer.push_back(1);

  _buffer.push_back(nx);
  _buffer.push_back(ny);
  _buffer.push_back(nz);
  _buffer.push_back(1);
}

//////////////////////////////////////////////////////////////
std::array<float, 4> ggo_vertex_buffer::get_pos(int i) const
{
  std::array<float, 4> pos;
  pos[0] = _buffer[12 * i + 0];
  pos[1] = _buffer[12 * i + 1];
  pos[2] = _buffer[12 * i + 2];
  pos[3] = _buffer[12 * i + 3];
  return pos;
}

//////////////////////////////////////////////////////////////
void ggo_vertex_buffer::clear()
{
  _buffer.clear();
}

//////////////////////////////////////////////////////////////
const float * ggo_vertex_buffer::get_pointer() const
{
  return &_buffer[0];
}

//////////////////////////////////////////////////////////////
int ggo_vertex_buffer::get_vertex_count() const
{
  return static_cast<int>(_buffer.size()) / 12;
}

//////////////////////////////////////////////////////////////
int ggo_vertex_buffer::get_byte_size() const
{
  return static_cast<int>(_buffer.size()) * sizeof(float);
}

//////////////////////////////////////////////////////////////
int ggo_vertex_buffer::get_stride_byte_size()
{
  return 12 * sizeof(float);
}

//////////////////////////////////////////////////////////////
int ggo_vertex_buffer::get_point_byte_offset()
{
  return 0;
}

//////////////////////////////////////////////////////////////
int ggo_vertex_buffer::get_color_byte_offset()
{
  return 4 * sizeof(float);
}

//////////////////////////////////////////////////////////////
int ggo_vertex_buffer::get_normal_byte_offset()
{
  return 8 * sizeof(float);
}
