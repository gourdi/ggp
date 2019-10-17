#ifndef __GGO_ANIMATOR__
#define __GGO_ANIMATOR__

#include <vector>
#include <stdint.h>
#include <memory>
#include <kernel/ggo_rect_int.h>
#include <2d/ggo_image.h>

namespace ggo
{
  class animate_abc;

  class animator final
  {
  public:

                             ~animator() = default;

    void					            add_animate(ggo::animate_abc * animate); // The animator takes ownership of the pointer.
    void					            insert_animate(ggo::animate_abc * animate, int pos); // The animator takes ownership of the pointer.

    int						            get_animates_count() const { return static_cast<int>(_animates.size()); }
    bool					            is_empty() const { return _animates.empty(); }
    const ggo::animate_abc *	get_animate(int index) const { return _animates[index].get(); }

    void					            clear();

    void					            update();
    void					            render(ggo::image & img) const;
    void					            render(ggo::image & img, const ggo::rect_int & clipping) const;

  private:

    std::vector<std::unique_ptr<ggo::animate_abc>> _animates;
  };
}

#endif
