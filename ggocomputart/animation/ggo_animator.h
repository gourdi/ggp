#ifndef __GGO_ANIMATOR__
#define __GGO_ANIMATOR__

#include <vector>
#include <stdint.h>
#include <memory>

class ggo_animate_abc;

class ggo_animator final
{
public:
	
  ~ggo_animator() = default;

	void					          add_animate(ggo_animate_abc * animate); // The animator takes ownership of the pointer.
	void					          insert_animate(ggo_animate_abc * animate, int pos); // The animator takes ownership of the pointer.
	                        
	int						          get_animates_count() const { return static_cast<int>(_animates.size()); }
	bool					          is_empty() const { return _animates.empty(); }
	const ggo_animate_abc *	get_animate(int index) const { return _animates[index].get(); }
	
	void					          clear();

	void					          update(void * buffer, int width, int height);
		
private:

  std::vector<std::unique_ptr<ggo_animate_abc>> _animates;
};

#endif
