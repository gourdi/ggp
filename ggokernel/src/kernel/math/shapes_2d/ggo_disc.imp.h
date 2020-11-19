namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename scalar_t>
  disc<scalar_t>::disc(const ggo::pos2<scalar_t> & p1, const ggo::pos2<scalar_t> & p2) :
    _center((p1 + p2) / scalar_t(2)),
    _radius(ggo::distance(p1, p2) / scalar_t(2))
  {
  }

  /////////////////////////////////////////////////////////////////////
  template <typename scalar_t>
  scalar_t disc<scalar_t>::dist_to_point(const ggo::pos2<scalar_t> & p) const
  {
    scalar_t sq_dist = hypot(_center - p);

    if (sq_dist > _radius * _radius)
    {
      return std::sqrt(sq_dist) - _radius;
    }
    else
    {
      return 0;
    }
  }

  /////////////////////////////////////////////////////////////////////
  template <typename scalar_t>
  rect_data<scalar_t> disc<scalar_t>::get_bounding_rect() const
  {
    scalar_t diameter = 2 * _radius;

    return { { _center.x() - _radius, _center.y() - _radius }, diameter, diameter };
  }

  /////////////////////////////////////////////////////////////////////
  template <typename scalar_t>
  bool disc<scalar_t>::is_point_inside(const ggo::pos2<scalar_t> & p) const
  {
    return hypot(_center - p) <= _radius * _radius;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename scalar_t>
  rect_intersection disc<scalar_t>::get_rect_intersection(const rect_data<scalar_t> & rect_data) const
  {
    scalar_t left    = rect_data._pos.x();
    scalar_t bottom  = rect_data._pos.y();
    scalar_t right   = left + rect_data._width;
    scalar_t top     = bottom + rect_data._height;

    // Rectangle in circle?
    ggo::pos2<scalar_t> p1(left,  bottom);
    ggo::pos2<scalar_t> p2(left,  top);
    ggo::pos2<scalar_t> p3(right, bottom);
    ggo::pos2<scalar_t> p4(right, top);

    scalar_t hypot = _radius * _radius;
    scalar_t hypot1 = ggo::hypot(_center, p1);
    scalar_t hypot2 = ggo::hypot(_center, p2);
    scalar_t hypot3 = ggo::hypot(_center, p3);
    scalar_t hypot4 = ggo::hypot(_center, p4);

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
    ggo::circle<scalar_t> circle(_center, _radius);
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
  template <typename scalar_t, bool orthonormal, bool cross_product_up>
  ggo::disc<scalar_t> from_local_to_world(const ggo::disc<scalar_t> & local_disc, const orthogonal_basis2d<scalar_t, orthonormal, cross_product_up> & basis)
  {
    auto center = basis.point_from_local_to_world(local_disc.center());
    auto radius = length(basis.x()) * local_disc.radius();
    return ggo::disc<scalar_t>(center, radius);
  }
}

