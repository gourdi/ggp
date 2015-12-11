#ifndef __GGO_IO__
#define __GGO_IO__

#include <iostream>
#include <vector>

namespace ggo
{
  class serializable
  {
  public:

    virtual void serialize(std::ostream & os) const = 0;
    virtual void deserialize(std::istream & is) = 0;
  };
}

inline std::ostream & operator<<(std::ostream & os, const ggo::serializable & s)
{
	s.serialize(os);
	return os;
}

inline std::istream & operator>>(std::istream & is, ggo::serializable & s)
{
	s.deserialize(is);
	return is;
}

//////////////////////////////////////////////////////////////
// Vector I/O.
template <typename T>
std::ostream & operator<<(std::ostream & os, const std::vector<T> & v)
{	
	os << v.size() << std::endl;

	typename std::vector<T>::const_iterator it;
	for (it = v.begin(); it != v.end(); ++it)
	{
		os << (*it) << std::endl;
	}
	
	return os;
}

template <typename T>
std::istream & operator>>(std::istream & is, std::vector<T> & v)
{
	int size;
	is >> size;
	v.clear();
	v.reserve(size);
	
	for (int i = 0; i < size; ++i)
	{
		T t;
		is >> t;
		v.push_back(t);	
	}

	return is;
}

#endif

