#include "ggo_toutouyoutou_bitmap_artist.h"
#include "ggo_animation_artist_abc.h"

//////////////////////////////////////////////////////////////
ggo_toutouyoutou_bitmap_artist::ggo_toutouyoutou_bitmap_artist(int render_width, int render_height)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
void ggo_toutouyoutou_bitmap_artist::render_bitmap(uint8_t * buffer)
{
    int frames_count = ggo::rand_int(100, 400);

    ggo_animation_artist_abc * anim_artist = ggo_animation_artist_abc::create(GGO_ANIMATION_ARTIST_TOUTOUYOUTOU, get_render_width(), get_render_height());
        
    anim_artist->render_next_frame(buffer);

    for (int i = 1; i < frames_count; ++i)
    {
        anim_artist->render_next_frame(nullptr);
    }
    
    anim_artist->render_next_frame(buffer);
    
    delete anim_artist;
}
