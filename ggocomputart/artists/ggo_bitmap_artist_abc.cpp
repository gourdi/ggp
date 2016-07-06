#include "ggo_bitmap_artist_abc.h"
#include "ggo_animation_artist_abc.h"
#include "ggo_artist_ids.h"
#include "artists/duffing/ggo_duffing_bitmap_artist.h"
#include "artists/trees/ggo_trees_artist.h"
#include "artists/worms/ggo_worms_artist.h"
#include "artists/flower/ggo_flower_artist.h"
#include "artists/cells/ggo_cells_artist.h"
#include "artists/buddhabrot/ggo_buddhabrot_artist.h"
#include "artists/kanji/ggo_kanji_bitmap_artist.h"
#include "artists/crystal/ggo_crystal_bitmap_artist.h"
#include "artists/filling_squares/ggo_filling_squares_bitmap_artist.h"
#include "artists/voronoi/ggo_voronoi_artist.h"
#include "artists/vortex/ggo_vortex_artist.h"
#include "artists/vortex2/ggo_vortex2_bitmap_artist.h"
#include "artists/plastic/ggo_plastic_bitmap_artist.h"
#include "artists/mandelbrot/ggo_mandelbrot_artist.h"
#include "artists/mondrian/ggo_mondrian_artist.h"
#include "artists/ifs/ggo_ifs_bitmap_artist.h"
#include "artists/mosaic/ggo_mosaic_artist.h"
#include "artists/metaballs/ggo_metaballs_bitmap_artist.h"
#include "artists/marbles/ggo_marbles_artist.h"
#include "artists/julia/ggo_julia_bitmap_artist.h"
#include "artists/alpha/ggo_alpha_bitmap_artist.h"
#include "artists/dupecheck/ggo_dupecheck_bitmap_artist.h"
#include "artists/cumbia/ggo_cumbia_bitmap_artist.h"
#include "artists/topodoko/ggo_topodoko_artist.h"
#include "artists/bozons/ggo_bozons_animation_artist.h"
#include "artists/rex/ggo_rex_artist.h"
#include "artists/circles/ggo_circles_bitmap_artist.h"
#include "artists/aggregation/ggo_aggregation_bitmap_artist.h"
#include "artists/bubbles/ggo_bubbles_artist.h"
#include "artists/distorsion/ggo_distorsion_bitmap_artist.h"
#include "artists/toutouyoutou/ggo_toutouyoutou_bitmap_artist.h"
#include "artists/hexa/ggo_hexa_bitmap_artist.h"
#include "artists/chryzode/ggo_chryzode_bitmap_artist.h"
#include "artists/stoa/ggo_stoa_bitmap_artist.h"
#include "artists/rediff/ggo_rediff_animation_artist.h"
#include "artists/entabeni/ggo_entabeni_bitmap_artist.h"
#include "artists/cabrel/ggo_cabrel_animation_artist.h"

//////////////////////////////////////////////////////////////
// ANIMATION ARTISTS WRAPPER

//////////////////////////////////////////////////////////////
class ggo_bitmap_artist_animation_wrapper : public ggo_bitmap_artist_abc
{
public:
	
  // A 'zero' frames count means: process all frames.
	ggo_bitmap_artist_animation_wrapper(ggo_animation_artist_id artist_id,
                                      int render_width,
                                      int render_height,
                                      int frames_count = 0,
                                      bool render_last_frame_only = false);
	
	void	render_bitmap(uint8_t * buffer) override;
	
private:
	
	ggo_animation_artist_id	_artist_id;
	int						          _frames_count;
  bool                    _render_last_frame_only;
};

//////////////////////////////////////////////////////////////
ggo_bitmap_artist_animation_wrapper::ggo_bitmap_artist_animation_wrapper(ggo_animation_artist_id artist_id,
                                                                         int render_width,
                                                                         int render_height,
                                                                         int frames_count,
                                                                         bool render_last_frame_only)
:
ggo_bitmap_artist_abc(render_width, render_height)
{
	_artist_id = artist_id;
	_frames_count = frames_count;
  _render_last_frame_only = render_last_frame_only;
    
  GGO_ASSERT(frames_count > 0 || render_last_frame_only == true);
}

//////////////////////////////////////////////////////////////
void ggo_bitmap_artist_animation_wrapper::render_bitmap(uint8_t * buffer)
{
	ggo_animation_artist_abc * artist = ggo_animation_artist_abc::create(_artist_id, get_render_width(), get_render_height());
	GGO_ASSERT(artist);
	
	if (artist != nullptr)
  {
    while (true)
    {
      bool done = false;

      if (_frames_count > 0 && artist->get_frame_index() >= _frames_count)
      {
        done = true;
      }

      uint8_t * frame_buffer = buffer;
      if (_render_last_frame_only == true && artist->get_frame_index() != _frames_count)
      {
        frame_buffer = nullptr;
      }
      if (artist->render_next_frame(frame_buffer) == false)
      {
        done = true;
      }

      if (done == true)
      {
        break;
      }
    }

    delete artist;
  }
}

//////////////////////////////////////////////////////////////
// BITMAP ARTISTS

//////////////////////////////////////////////////////////////
ggo_bitmap_artist_abc::ggo_bitmap_artist_abc(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{
	
}

//////////////////////////////////////////////////////////////
ggo_bitmap_artist_abc * ggo_bitmap_artist_abc::create(ggo_bitmap_artist_id artist_id, int render_width, int render_height)
{
	switch (artist_id)
	{
	case GGO_BITMAP_ARTIST_DUFFING:
		return new ggo_duffing_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_TREES:
		return new ggo_trees_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_WORMS:
		return new ggo_worms_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_FLOWER:
		return new ggo_flower_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_CELLS:
		return new ggo_cells_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_BUDDHABROT:
		return new ggo_buddhabrot_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_KANJI:
		return new ggo_kanji_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_CRYSTAL:
		return new ggo_crystal_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_FILLING_SQUARES:
		return new ggo_filling_squares_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_VORONOI:
		return new ggo_voronoi_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_VORTEX:
		return new ggo_vortex_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_VORTEX2:
		return new ggo_vortex2_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_PLASTIC:
		return new ggo_plastic_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_MANDELBROT:
		return new ggo_mandelbrot_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_MONDRIAN:
		return new ggo_mondrian_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_IFS:
		return new ggo_ifs_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_MOSAIC:
		return new ggo_mosaic_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_METABALLS:
		return new ggo_metaballs_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_MARBLES:
		return new ggo_marbles_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_JULIA:
		return new ggo_julia_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_ALPHA:
		return new ggo_alpha_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_DUPECHECK:
		return new ggo_dupecheck_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_CIRCLES:
    return new ggo_circles_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_AGGREGATION:
    return new ggo_aggregation_bitmap_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_TOPODOKO:
    return new ggo_topodoko_artist(render_width, render_height);
	case GGO_BITMAP_ARTIST_REX:
		return new ggo_rex_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_BUBBLE:
    return new ggo_bubbles_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_DISTORSION:
    return new ggo_distorsion_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_TOUTOUYOUTOU:
    return new ggo_toutouyoutou_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_CUMBIA:
		return new ggo_cumbia_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_HEXA:
   	return new ggo_hexa_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_CHRYZODE:
    return new ggo_chryzode_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_STOA:
    return new ggo_stoa_bitmap_artist(render_width, render_height);
  case GGO_BITMAP_ARTIST_ENTABENI:
    return new ggo_entabeni_bitmap_artist(render_width, render_height);

    // Animation artists.
	case GGO_BITMAP_ARTIST_SMOKE:
		return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_SMOKE, render_width, render_height, 400);
	case GGO_BITMAP_ARTIST_IKEDA:
		return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_IKEDA, render_width, render_height, 1);
	case GGO_BITMAP_ARTIST_LAGAUDE:
		return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_LAGAUDE, render_width, render_height, 25);
  case GGO_BITMAP_ARTIST_AMOROSI:
    return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_AMOROSI, render_width, render_height, 100, true);
  case GGO_BITMAP_ARTIST_BOZONS:
    return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_BOZONS, render_width, render_height);
  case GGO_BITMAP_ARTIST_RAH:
    return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_RAH, render_width, render_height, 800, true);
  case GGO_BITMAP_ARTIST_SONSON:
    return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_SONSON, render_width, render_height, 200, true);
  case GGO_BITMAP_ARTIST_REDIFF:
    return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_REDIFF, render_width, render_height, 350, true);
  case GGO_BITMAP_ARTIST_CABREL:
    return new ggo_bitmap_artist_animation_wrapper(GGO_ANIMATION_ARTIST_CABREL, render_width, render_height);

	default:
		GGO_FAIL();
		return nullptr;
    }
}


