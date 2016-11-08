#include "ggo_animation_artist_abc.h"
#include "artists/duffing/ggo_duffing_animation_artist.h"
#include "artists/julia/ggo_julia_animation_artist.h"
#include "artists/kanji/ggo_kanji_animation_artist.h"
#include "artists/bozons/ggo_bozons_animation_artist.h"
#include "artists/filling_squares/ggo_filling_squares_animation_artist.h"
#include "artists/plastic/ggo_plastic_animation_artist.h"
#include "artists/aggregation/ggo_aggregation_animation_artist.h"
#include "artists/ifs/ggo_ifs_animation_artist.h"
#include "artists/metaballs/ggo_metaballs_animation_artist.h"
#include "artists/smoke/ggo_smoke_animation_artist.h"
#include "artists/flies/ggo_flies_artist.h"
#include "artists/newton/ggo_newton_artist.h"
#include "artists/alpha/ggo_alpha_anim_artist.h"
#include "artists/dupecheck/ggo_dupecheck_animation_artist.h"
#include "artists/lagaude/ggo_lagaude_animation_artist.h"
#include "artists/crystal/ggo_crystal_animation_artist.h"
#include "artists/ikeda/ggo_ikeda_artist.h"
#include "artists/cumbia/ggo_cumbia_animation_artist.h"
#include "artists/vortex2/ggo_vortex2_animation_artist.h"
#include "artists/circles/ggo_circles_animation_artist.h"
#include "artists/distorsion/ggo_distorsion_animation_artist.h"
#include "artists/toutouyoutou/ggo_toutouyoutou_animation_artist.h"
#include "artists/amorosi/ggo_amorosi_animation_artist.h"
#include "artists/rah/ggo_rah_animation_artist.h"
#include "artists/hexa/ggo_hexa_animation_artist.h"
#include "artists/chryzode/ggo_chryzode_animation_artist.h"
#include "artists/stoa/ggo_stoa_animation_artist.h"
#include "artists/sonson/ggo_sonson_animation_artist.h"
#include "artists/rediff/ggo_rediff_animation_artist.h"
#include "artists/entabeni/ggo_entabeni_animation_artist.h"

//////////////////////////////////////////////////////////////
// ANIMATION ARTISTS

//////////////////////////////////////////////////////////////
ggo_animation_artist_abc::ggo_animation_artist_abc(int render_width, int render_height)
:
ggo_artist_abc(render_width, render_height)
{
	_counter = 0;
}

//////////////////////////////////////////////////////////////
ggo_animation_artist_abc * ggo_animation_artist_abc::create(ggo_animation_artist_id artist_id, int render_width, int render_height)
{
	ggo_animation_artist_abc * artist = NULL;
	
	switch (artist_id)
	{
	case GGO_ANIMATION_ARTIST_DUFFING:
		artist = new ggo_duffing_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_JULIA:
		artist = new ggo_julia_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_KANJI:
		artist = new ggo_kanji_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_BOZONS:
		artist = new ggo_bozons_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_FILLING_SQUARES:
		artist = new ggo_filling_squares_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_PLASTIC:
		artist = new ggo_plastic_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_AGGREGATION:
		artist = new ggo_aggregation_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_IFS:
		artist = new ggo_ifs_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_METABALLS:
		artist = new ggo_metaballs_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_SMOKE:
		artist = new ggo_smoke_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_SONSON:
    artist = new ggo_sonson_animation_artist(render_width, render_height);
    break;
	case GGO_ANIMATION_ARTIST_NEWTON:
		artist = new ggo_newton_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_ALPHA:
		artist = new ggo_alpha_anim_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_DUPECHECK:
		artist = new ggo_dupecheck_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_LAGAUDE:
		artist = new ggo_lagaude_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_CRYSTAL:
		artist = new ggo_crystal_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_IKEDA:
		artist = new ggo_ikeda_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_CUMBIA:
		artist = new ggo_cumbia_animation_artist(render_width, render_height);
		break;
	case GGO_ANIMATION_ARTIST_VORTEX2:
		artist = new ggo_vortex2_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_CIRCLES:
		artist = new ggo_circles_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_DISTORSION:
    artist = new ggo_distorsion_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_TOUTOUYOUTOU:
    artist = new ggo_toutouyoutou_anim_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_AMOROSI:
    artist = new ggo_amorosi_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_RAH:
    artist = new ggo_rah_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_HEXA:
    artist = new ggo_hexa_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_CHRYZODE:
    artist = new ggo_chryzode_animation_artist(render_width, render_height);
		break;
  case GGO_ANIMATION_ARTIST_STOA:
    artist = new ggo_stoa_animation_artist(render_width, render_height);
    break;
  case GGO_ANIMATION_ARTIST_FLIES:
    artist = new ggo_flies_artist(render_width, render_height);
    break;
  case GGO_ANIMATION_ARTIST_REDIFF:
    artist = new ggo_rediff_animation_artist(render_width, render_height);
    break;
  case GGO_ANIMATION_ARTIST_ENTABENI:
    artist = new ggo_entabeni_animation_artist(render_width, render_height);
    break;
	default:
		GGO_FAIL();
		break;
	}
    
  if (artist != nullptr)
  {
    artist->init();
  }

	return artist;
}

//////////////////////////////////////////////////////////////
void ggo_animation_artist_abc::init()
{
	_counter = 0;
	
	init_sub();
}

//////////////////////////////////////////////////////////////
bool ggo_animation_artist_abc::render_next_frame(uint8_t * buffer)
{
	if (render_next_frame_sub(buffer, _counter) == false)
	{
		return false;
	}
	
	++_counter;
	
	return true;
}

//////////////////////////////////////////////////////////////
// ACCUMULATION ARTIST

//////////////////////////////////////////////////////////////
ggo_accumulation_animation_artist_abc::ggo_accumulation_animation_artist_abc(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height)
{
}

//////////////////////////////////////////////////////////////
bool ggo_accumulation_animation_artist_abc::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (frame_index == 0)
  {
    init_output_buffer(buffer);
  }

  return render_next_frame_acc(buffer, frame_index);
};

//////////////////////////////////////////////////////////////
// STATIC BACKGROUND ARTIST

//////////////////////////////////////////////////////////////
ggo_static_background_animation_artist_abc::ggo_static_background_animation_artist_abc(int render_width, int render_height)
:
ggo_animation_artist_abc(render_width, render_height),
_bkgd_buffer(3 * render_width * render_height)
{
}

//////////////////////////////////////////////////////////////
bool ggo_static_background_animation_artist_abc::render_next_frame_sub(uint8_t * buffer, int frame_index)
{
  if (frame_index == 0)
  {
    init_bkgd_buffer(_bkgd_buffer.data());
  }

  if (buffer != nullptr)
  {
    memcpy(buffer, _bkgd_buffer.data(), _bkgd_buffer.get_count());
  }

  return render_next_frame_bkgd(buffer, frame_index);
};

