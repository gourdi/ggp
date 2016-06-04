namespace ggo
{
  template <typename data_t>
  bool find_closest_lines_point(const ggo::set3<data_t> & orig1, const ggo::set3<data_t> & dir1,
                                const ggo::set3<data_t> & orig2, const ggo::set3<data_t> & dir2,
                                data_t & dist1, data_t & dist2);


  template <typename data_t>
  bool find_closest_lines_point(const ggo::set3<data_t> & orig1, const ggo::set3<data_t> & dir1,
                                const ggo::set3<data_t> & orig2, const ggo::set3<data_t> & dir2,
                                ggo::set3<data_t> & p1, ggo::set3<data_t> & p2);
}
