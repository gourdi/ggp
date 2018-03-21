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
#include "artists/newton/ggo_newton_artist.h"
#include "artists/alpha/ggo_alpha_animation_artist.h"
#include "artists/dupecheck/ggo_dupecheck_animation_artist.h"
#include "artists/lagaude/ggo_lagaude_animation_artist.h"
#include "artists/crystal/ggo_crystal_animation_artist.h"
#include "artists/ikeda/ggo_ikeda_artist.h"
#include "artists/cumbia/ggo_cumbia_animation_artist.h"
#include "artists/vortex/ggo_vortex_animation_artist.h"
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
#include "artists/neon/ggo_neon_artist.h"
#include "artists/storni/ggo_storni_animation_artist.h"
#include "artists/kame/ggo_kame_animation_artist.h"

//////////////////////////////////////////////////////////////
// ANIMATION ARTISTS

//////////////////////////////////////////////////////////////
ggo::animation_artist_abc::animation_artist_abc(int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
:
ggo::artist(width, height),
_line_step(line_step),
_pbf(pbf),
_rendering_type(rt)
{
}

//////////////////////////////////////////////////////////////
ggo::animation_artist_abc * ggo::animation_artist_abc::create(ggo::animation_artist_id artist_id, int width, int height, int line_step, ggo::pixel_buffer_format pbf, rendering_type rt)
{
	ggo::animation_artist_abc * artist = nullptr;
	
	switch (artist_id)
	{
	case ggo::animation_artist_id::duffing:
		artist = new ggo::duffing_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::julia:
		artist = new ggo::julia_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::kanji:
		artist = new ggo::kanji_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::bozons:
		artist = new ggo::bozons_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::filling_squares:
		artist = new ggo::filling_squares_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::plastic:
		artist = new ggo::plastic_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::aggregation:
		artist = new ggo::aggregation_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::ifs:
		artist = new ggo::ifs_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::metaballs:
		artist = new ggo::metaballs_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::smoke:
		artist = new ggo::smoke_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::sonson:
    artist = new ggo::sonson_animation_artist(width, height, line_step, pbf, rt);
    break;
	case ggo::animation_artist_id::newton:
		artist = new ggo::newton_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::alpha:
		artist = new ggo::alpha_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::dupecheck:
		artist = new ggo::dupecheck_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::lagaude:
		artist = new ggo::lagaude_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::crystal:
		artist = new ggo::crystal_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::ikeda:
		artist = new ggo::ikeda_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::cumbia:
		artist = new ggo::cumbia_animation_artist(width, height, line_step, pbf, rt);
		break;
	case ggo::animation_artist_id::vortex:
		artist = new ggo::vortex_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::circles:
		artist = new ggo::circles_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::distorsion:
    artist = new ggo::distorsion_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::toutouyoutou:
    artist = new ggo::toutouyoutou_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::amorosi:
    artist = new ggo::amorosi_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::rah:
    artist = new ggo::rah_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::hexa:
    artist = new ggo::hexa_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::chryzode:
    artist = new ggo::chryzode_animation_artist(width, height, line_step, pbf, rt);
		break;
  case ggo::animation_artist_id::stoa:
    artist = new ggo::stoa_animation_artist(width, height, line_step, pbf, rt);
    break;
  case ggo::animation_artist_id::rediff:
    artist = new ggo::rediff_animation_artist(width, height, line_step, pbf, rt);
    break;
  case ggo::animation_artist_id::entabeni:
    artist = new ggo::entabeni_animation_artist(width, height, line_step, pbf, rt);
    break;
  case ggo::animation_artist_id::neon:
    artist = new ggo::neon_artist(width, height, line_step, pbf, rt);
    break;
  case ggo::animation_artist_id::storni:
    artist = new ggo::storni_animation_artist(width, height, line_step, pbf, rt);
    break;
  case ggo::animation_artist_id::kame:
    artist = new ggo::kame_animation_artist(width, height, line_step, pbf, rt);
    break;
	default:
		GGO_FAIL();
		break;
	}

	return artist;
}

