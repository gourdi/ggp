namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  disc<data_t>::disc(const ggo::pos2<data_t> & p1, const ggo::pos2<data_t> & p2) :
    _center((p1 + p2) / data_t(2)),
    _radius(ggo::distance(p1, p2) / data_t(2))
  {
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  data_t disc<data_t>::dist_to_point(const ggo::pos2<data_t> & p) const
  {
    ggo::vec2<data_t> diff  = _center - p;
    data_t hypot	          = diff.get_hypot();
    data_t sqrd_radius      = _radius * _radius;

    if (hypot > sqrd_radius)
    {
      return std::sqrt(hypot) - _radius;
    }
    else
    {
      return 0;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_data<data_t> disc<data_t>::get_bounding_rect() const
  {
    data_t diameter = 2 * _radius;

    return { { _center.x() - _radius, _center.y() - _radius }, diameter, diameter };
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  bool disc<data_t>::is_point_inside(const ggo::pos2<data_t> & p) const
  {
    vec2<data_t> diff = _center - p;
    data_t hypot	    = diff.get_hypot();

    return hypot <= _radius * _radius;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t>
  rect_intersection disc<data_t>::get_rect_intersection(const rect_data<data_t> & rect_data) const
  {
    data_t left    = rect_data._pos.x();
    data_t bottom  = rect_data._pos.y();
    data_t right   = left + rect_data._width;
    data_t top     = bottom + rect_data._height;

    // Rectangle in circle?
    ggo::pos2<data_t> p1(left,  bottom);
    ggo::pos2<data_t> p2(left,  top);
    ggo::pos2<data_t> p3(right, bottom);
    ggo::pos2<data_t> p4(right, top);

    data_t hypot = _radius * _radius;
    data_t hypot1 = ggo::hypot(_center, p1);
    data_t hypot2 = ggo::hypot(_center, p2);
    data_t hypot3 = ggo::hypot(_center, p3);
    data_t hypot4 = ggo::hypot(_center, p4);

    if (hypot1 <= hypot && hypot2 <= hypot && hypot3 <= hypot && hypot4 <= hypot)
    {
      return rect_intersection::rect_in_shape;
    }

    // Circle in rectangle?
    if (_center.x() - left   >= _radius &&
        _center.y() - bottom >= _radius &&
        right - _center.x()  >= _radius &&
        top - _center.y()    >= _radius)
    {
      return rect_intersection::shape_in_rect;
    }

    // Partial overlap?
    ggo::circle<data_t> circle(_center, _radius);
    if (circle.test_segment_intersection({ left, bottom }, { left, top }) == true ||
        circle.test_segment_intersection({ right, bottom }, { right, top }) == true ||
        circle.test_segment_intersection({ left, bottom }, { right, bottom }) == true ||
        circle.test_segment_intersection({ left, top }, { right, top }) == true)
    {
      return rect_intersection::partial_overlap;
    }

    return rect_intersection::disjoints;
  }
}

namespace ggo
{
  template <typename data_t, bool orthonormal, bool cross_product_up>
  ggo::disc<data_t> from_local_to_world(const ggo::disc<data_t> & local_disc, const orthogonal_basis2d<data_t, orthonormal, cross_product_up> & basis)
  {
    auto center = basis.point_from_local_to_world(local_disc.center());
    auto radius = basis.x().get_length() * local_disc.radius();
    return ggo::disc<data_t>(center, radius);
  }
}

