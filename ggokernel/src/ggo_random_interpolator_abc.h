#ifndef __GGO_RANDOM_INTERPOLATOR_ABC__
#define __GGO_RANDOM_INTERPOLATOR_ABC__

#include <ggo_interpolation1d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_type, typename time_type>
  class random_interpolator_abc
  {
  public:

                  random_interpolator_abc();
    virtual	     ~random_interpolator_abc();

    data_type		  update(time_type dt);

    virtual	void  get_random_data(data_type & data, time_type & dt) = 0;

  private:

    time_type	_time;
    struct ggo_interpolation_data
    {
      data_type _interp;
      time_type	_time;
    };
    ggo_interpolation_data * _interp;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_type, typename time_type>
  random_interpolator_abc<data_type, time_type>::random_interpolator_abc()
  {
    _interp = nullptr;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_type, typename time_type>
  random_interpolator_abc<data_type, time_type>::~random_interpolator_abc()
  {
    if (_interp != nullptr)
    {
      delete[] _interp;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_type, typename time_type>
  data_type random_interpolator_abc<data_type, time_type>::update(time_type dt)
  {
    if (_interp == nullptr)
    {
      _interp = new ggo_interpolation_data[4];

      time_type dt;

      get_random_data(_interp[0]._interp, dt);
      _interp[0]._time = 0;

      get_random_data(_interp[1]._interp, dt);
      _interp[1]._time = _interp[0]._time + dt;

      get_random_data(_interp[2]._interp, dt);
      _interp[2]._time = _interp[1]._time + dt;

      get_random_data(_interp[3]._interp, dt);
      _interp[3]._time = _interp[2]._time + dt;

      _time = _interp[1]._time;
    }

    data_type result = ggo::cubic_interpolation<time_type, data_type>(_interp[0]._time, _interp[0]._interp,
                                                                      _interp[1]._time, _interp[1]._interp,
                                                                      _interp[2]._time, _interp[2]._interp,
                                                                      _interp[3]._time, _interp[3]._interp, _time);

    // Update spline points if needed.
    _time += dt;
    if (_time > _interp[2]._time)
    {
      _interp[0] = _interp[1];
      _interp[1] = _interp[2];
      _interp[2] = _interp[3];

      time_type dt;
      get_random_data(_interp[3]._interp, dt);
      _interp[3]._time = _interp[2]._time + dt;

      // Make sure the time does not grow (otherwise numerical imprecision may arise).
      _time -= _interp[0]._time;
      _interp[3]._time -= _interp[0]._time;
      _interp[2]._time -= _interp[0]._time;
      _interp[1]._time -= _interp[0]._time;
      _interp[0]._time = 0;
    }

    return result;
  }
}

#endif
