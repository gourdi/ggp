#ifndef __GGO_RANDOM_INTERPOLATOR_ABC__
#define __GGO_RANDOM_INTERPOLATOR_ABC__

#include <ggo_interpolation1d.h>

//////////////////////////////////////////////////////////////
template <typename T, typename TIME_T>
class ggo_random_interpolator_abc
{
public:
	
                ggo_random_interpolator_abc();
	virtual	     ~ggo_random_interpolator_abc();

			T		      update(TIME_T dt);
	
	virtual	void	get_random_data(T & data, TIME_T & dt) = 0;

private:
	
	TIME_T	_time;
	struct ggo_interpolation_data
	{
		T	      _interp;
		TIME_T	_time;
	};
	ggo_interpolation_data * _interp;
};

/////////////////////////////////////////////////////////////////////
template <typename T, typename TIME_T>
ggo_random_interpolator_abc<T, TIME_T>::ggo_random_interpolator_abc()
{
	_interp = nullptr;
}

/////////////////////////////////////////////////////////////////////
template <typename T, typename TIME_T>
ggo_random_interpolator_abc<T, TIME_T>::~ggo_random_interpolator_abc()
{
	if (_interp != nullptr)
	{
		delete[] _interp;
	}
}

//////////////////////////////////////////////////////////////
template <typename T, typename TIME_T>
T ggo_random_interpolator_abc<T, TIME_T>::update(TIME_T dt)
{
	if (_interp == nullptr)
	{
		_interp = new ggo_interpolation_data[4];
		
		TIME_T dt;
		
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
	
	T result = ggo::cubic_interpolation<TIME_T, T>(_interp[0]._time, _interp[0]._interp,
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
		
		TIME_T dt;
		get_random_data(_interp[3]._interp, dt);
		_interp[3]._time = _interp[2]._time + dt;
        
        // Make sure the time does not grow (otherwise numerical imprecision may arise).
        _time -= _interp[0]._time;
        _interp[3]._time -= _interp[0]._time;
        _interp[2]._time -= _interp[0]._time;
        _interp[1]._time -= _interp[0]._time;
        _interp[0]._time  = 0;
	}

	return result;
}

#endif
