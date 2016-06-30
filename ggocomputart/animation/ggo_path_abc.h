#ifndef __GGO_PATH_ABC__
#define __GGO_PATH_ABC__

#include <ggo_vec.h>

class ggo_path_abc
{
public:	

	virtual 			     ~ggo_path_abc() {}
	
	virtual	ggo::pos2f  get_pos(int counter) = 0;
};

#endif