#ifndef __GGO_VERTEX_BUFFER__
#define __GGO_VERTEX_BUFFER__

#include <vector>
#include <ggo_set3.h>
#include <ggo_color.h>
#include <array>

class ggo_vertex_buffer
{
public:

  void                  add_vertex(const ggo::point3d_float & pos, const ggo::color & color, const ggo::vector3d_float & normal);
  void                  add_vertex(float px, float py, float pz, float r, float g, float b, float nx, float ny, float nz);

  void                  clear();

  std::array<float, 4>  get_pos(int i) const;

  const float *         get_pointer() const;
  int                   get_vertex_count() const;
  int                   get_byte_size() const;

  static int            get_stride_byte_size();
  static int            get_point_byte_offset();
  static int            get_color_byte_offset();
  static int            get_normal_byte_offset();

private:

  std::vector<float>  _buffer;
};

#endif