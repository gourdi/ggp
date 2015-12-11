#include "ggo_animator.h"
#include "ggo_animate_abc.h"

//////////////////////////////////////////////////////////////
ggo_animator::~ggo_animator()
{
	clear();
}

//////////////////////////////////////////////////////////////
void ggo_animator::clear()
{
    for (ggo_animate_abc * animate : _animates)
	{
		delete animate;
	}
}

//////////////////////////////////////////////////////////////
void ggo_animator::add_animate(ggo_animate_abc * animate)
{
	_animates.push_back(animate);
}

//////////////////////////////////////////////////////////////
void ggo_animator::insert_animate(ggo_animate_abc * animate, int pos)
{
	_animates.insert(_animates.begin() + pos, animate);
}

//////////////////////////////////////////////////////////////
void ggo_animator::update(uint8_t * output_buffer, uint8_t * bkgd_buffer, int width, int height)
{
    int i = 0;
    while (i < _animates.size())
    {
        ggo_animate_abc * animate = _animates[i];
        
        if (animate->update(output_buffer, bkgd_buffer, width, height) == false)
        {
            delete animate;
            _animates.erase(_animates.begin() + i);
        }
        else
        {
            ++i;
        }
    }
}
