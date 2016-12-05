//////////////////////////////////////////////////////////////
template <ggo::pixel_buffer_format pbf>
void ggo::plastic_artist::render(void * buffer, int line_step, const std::vector<ggo::plastic_artist::params> & params, const ggo::color_32f & color, float altitude_factor) const
{
  const float width_f = static_cast<float>(get_width());
  const float height_f = static_cast<float>(get_height());
  const float range_x = width_f > height_f ? width_f / height_f : 1.f;
  const float range_y = width_f > height_f ? 1.f : height_f / width_f;

  for (int y = 0; y < get_height(); ++y)
  {
    const float y1 = ggo::map(y - 3 / 8.f, 0.f, height_f, -range_y, range_y);
    const float y2 = ggo::map(y + 3 / 8.f, 0.f, height_f, -range_y, range_y);

    void * ptr = ggo::get_line_ptr<pbf>(buffer, y, get_height(), line_step);

    for (int x = 0; x < get_width(); ++x)
    {
      // Compute altitude.
      const float x1 = ggo::map(x - 3 / 8.f, 0.f, width_f, -range_x, range_x);
      const float x2 = ggo::map(x + 3 / 8.f, 0.f, width_f, -range_x, range_x);

      const float z11 = evaluate(x1, y1, params);
      const float z12 = evaluate(x1, y2, params);
      const float z21 = evaluate(x2, y1, params);
      const float z22 = evaluate(x2, y2, params);

      // Render the slope.
      const float dx = x2 - x1;
      const float dy = y2 - y1;
      const ggo::vec3f v1(dx, dy, z22 - z11);
      const ggo::vec3f v2(dx, -dy, z21 - z12);
      ggo::vec3f normal = ggo::cross(v1, v2);
      normal.normalize();

      const ggo::color_32f pixel_color = color * (altitude_factor + std::abs(normal.get<2>()) / altitude_factor);

      ggo::write_pixel<pbf>(buffer, ggo::convert_color_to<ggo::color_8u>(pixel_color));

      buffer = ggo::ptr_offset<ggo::pixel_buffer_format_info<pbf>::pixel_byte_size>(buffer);
    }
  }
}
