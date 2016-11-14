#include "ggo_toutouyoutou_bitmap_artist.h"

//////////////////////////////////////////////////////////////
ggo::toutouyoutou_bitmap_artist::toutouyoutou_bitmap_artist(int render_width, int render_height)
:
bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo::toutouyoutou_bitmap_artist::render_bitmap(void * buffer) const
{
    int frames_count = ggo::rand<int>(100, 400);

    std::unique_ptr<ggo::bitmap_artist_abc> anim_artist(ggo::animation_artist_abc::create(GGO_ANIMATION_ARTIST_TOUTOUYOUTOU, get_render_width(), get_render_height()));
        
    anim_artist->render_next_frame(buffer);

    for (int i = 1; i < frames_count; ++i)
    {
        anim_artist->render_next_frame(nullptr);
    }
    
    anim_artist->render_next_frame(buffer);
}
