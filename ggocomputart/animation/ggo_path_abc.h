#ifndef __GGO_PATH_ABC__
#define __GGO_PATH_ABC__

#include <ggo_set2.h>

class ggo_path_abc
{
public:	

	virtual 			        	   ~ggo_path_abc() {}
	
	virtual	ggo::point2d_float	get_pos(int counter) = 0;
};

#endif