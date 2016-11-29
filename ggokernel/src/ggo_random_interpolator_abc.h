#ifndef __GGO_RANDOM_INTERPOLATOR_ABC__
#define __GGO_RANDOM_INTERPOLATOR_ABC__

#include <ggo_interpolation1d.h>

namespace ggo
{
  //////////////////////////////////////////////////////////////
  template <typename data_t, typename time_t>
  class random_interpolator_abc
  {
  public:

                    random_interpolator_abc();
    virtual	       ~random_interpolator_abc();

            data_t  update(time_t dt);

    virtual	void    get_random_data(data_t & data, time_t & dt) = 0;

  private:

    time_t	_time;
    struct interpolation_data
    {
      data_t  _interp;
      time_t  _time;
    };
    interpolation_data * _interp;
  };
}

namespace ggo
{
  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename time_t>
  random_interpolator_abc<data_t, time_t>::random_interpolator_abc()
  {
    _interp = nullptr;
  }

  /////////////////////////////////////////////////////////////////////
  template <typename data_t, typename time_t>
  random_interpolator_abc<data_t, time_t>::~random_interpolator_abc()
  {
    if (_interp != nullptr)
    {
      delete[] _interp;
    }
  }

  //////////////////////////////////////////////////////////////
  template <typename data_t, typename time_t>
  data_t random_interpolator_abc<data_t, time_t>::update(time_t dt)
  {
    if (_interp == nullptr)
    {
      _interp = new interpolation_data[4];

      time_t dt;

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

    data_t result = ggo::cubic_interpolation<time_t, data_t>(_interp[0]._time, _interp[0]._interp,
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

      time_t dt;
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
