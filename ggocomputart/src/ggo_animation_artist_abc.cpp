#include "ggo_animation_artist_abc.h"
#include "ggo_realtime_artist_abc.h"
#include "duffing/ggo_duffing_animation_artist.h"
#include "julia/ggo_julia_animation_artist.h"
#include "bozons/ggo_bozons_realtime_artist.h"
#include "filling_squares/ggo_filling_squares_animation_artist.h"
#include "plastic/ggo_plastic_animation_artist.h"
#include "aggregation/ggo_aggregation_animation_artist.h"
#include "ifs/ggo_ifs_animation_artist.h"
#include "metaballs/ggo_metaballs_animation_artist.h"
#include "smoke/ggo_smoke_animation_artist.h"
#include "alpha/ggo_alpha_animation_artist.h"
#include "dupecheck/ggo_dupecheck_animation_artist.h"
#include "crystal/ggo_crystal_animation_artist.h"
#include "ikeda/ggo_ikeda_artist.h"
#include "cumbia/ggo_cumbia_animation_artist.h"
#include "vortex/ggo_vortex_animation_artist.h"
#include "distorsion/ggo_distorsion_animation_artist.h"
#include "toutouyoutou/ggo_toutouyoutou_animation_artist.h"
#include "amorosi/ggo_amorosi_animation_artist.h"
#include "rah/ggo_rah_animation_artist.h"
#include "hexa/ggo_hexa_animation_artist.h"
#include "chryzode/ggo_chryzode_animation_artist.h"
#include "stoa/ggo_stoa_animation_artist.h"
#include "rediff/ggo_rediff_animation_artist.h"
#include "entabeni/ggo_entabeni_animation_artist.h"
#include "neon/ggo_neon_artist.h"
#include "storni/ggo_storni_realtime_artist.h"
#include "kame/ggo_kame_animation_artist.h"
#include "demeco/ggo_demeco_animation_artist.h"
#include "wakenda/ggo_wakenda_animation_artist.h"
#include "poupette/ggo_poupette_animation_artist.h"

//////////////////////////////////////////////////////////////
// REALTIME ARTISTS WRAPPER

//////////////////////////////////////////////////////////////
namespace ggo
{
  class animation_artist_realtime_wrapper : public animation_artist_abc
  {
  public:

    animation_artist_realtime_wrapper(ggo::realtime_artist_id artist_id, int width, int height, int line_step, ggo::image_format format)
    :
    animation_artist_abc(width, height, line_step, format),
    _artist(realtime_artist_abc::create(artist_id, width, height, line_step, format))
    {
    }

    void  render_frame(void * buffer, int frame_index, bool & finished) override
    {
      _artist->preprocess_frame(0, { 0, 0 });
      _artist->render_tile(buffer, ggo::rect_int::from_size(size()));
      finished = _artist->finished();
    }

  private:

    std::unique_ptr<realtime_artist_abc> _artist;
  };
}

//////////////////////////////////////////////////////////////
// ANIMATION ARTIST

namespace ggo
{
  //////////////////////////////////////////////////////////////
  ggo::animation_artist_abc::animation_artist_abc(int width, int height, int line_step, ggo::image_format format)
    :
    ggo::artist(width, height, line_step, format)
  {
  }

  //////////////////////////////////////////////////////////////
  bool ggo::animation_artist_abc::render_frame(void * buffer)
  {
    bool finished = true;
    render_frame(buffer, _frame_index, finished);
    if (finished == true)
    {
      return false;
    }

    ++_frame_index;

    return true;
  }

  //////////////////////////////////////////////////////////////
  ggo::animation_artist_abc * ggo::animation_artist_abc::create(ggo::animation_artist_id artist_id, int width, int height, int line_step, ggo::image_format format)
  {
    switch (artist_id)
    {
    case ggo::animation_artist_id::duffing:
      return new ggo::duffing_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::julia:
      return new ggo::julia_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::filling_squares:
      return new ggo::filling_squares_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::plastic:
      return new ggo::plastic_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::aggregation:
      return new ggo::aggregation_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::ifs:
      return new ggo::ifs_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::metaballs:
      return new ggo::metaballs_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::smoke:
      return new ggo::smoke_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::alpha:
      return new ggo::alpha_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::dupecheck:
      return new ggo::dupecheck_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::crystal:
      return new ggo::crystal_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::ikeda:
      return new ggo::ikeda_artist(width, height, line_step, format);
    case ggo::animation_artist_id::cumbia:
      return new ggo::cumbia_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::vortex:
      return new ggo::vortex_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::distorsion:
      return new ggo::distorsion_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::toutouyoutou:
      return new ggo::toutouyoutou_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::amorosi:
      return new ggo::amorosi_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::rah:
      return new ggo::rah_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::hexa:
      return new ggo::hexa_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::chryzode:
      return new ggo::chryzode_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::stoa:
      return new ggo::stoa_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::rediff:
      return new ggo::rediff_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::entabeni:
      return new ggo::entabeni_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::kame:
      return new ggo::kame_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::demeco:
      return new ggo::demeco_animation_artist(width, height, line_step, format);
    case ggo::animation_artist_id::poupette:
      return new ggo::poupette_animation_artist(width, height, line_step, format);

    // Real-time artists.
    case ggo::animation_artist_id::kanji:
      return new ggo::animation_artist_realtime_wrapper(ggo::realtime_artist_id::kanji, width, height, line_step, format);
    case ggo::animation_artist_id::bozons:
      return new ggo::animation_artist_realtime_wrapper(ggo::realtime_artist_id::bozons, width, height, line_step, format);
    case ggo::animation_artist_id::neon:
      return new ggo::animation_artist_realtime_wrapper(ggo::realtime_artist_id::neon, width, height, line_step, format);
    case ggo::animation_artist_id::storni:
      return new ggo::animation_artist_realtime_wrapper(ggo::realtime_artist_id::storni, width, height, line_step, format);
    case ggo::animation_artist_id::lagaude:
      return new ggo::animation_artist_realtime_wrapper(ggo::realtime_artist_id::lagaude, width, height, line_step, format);
    case ggo::animation_artist_id::wakenda:
      return new ggo::animation_artist_realtime_wrapper(ggo::realtime_artist_id::wakenda, width, height, line_step, format);

    default:
      GGO_FAIL();
      return nullptr;
    }

    return nullptr;
  }
}

//////////////////////////////////////////////////////////////
// FIXED FRAMES COUNT ANIMATION ARTIST

//////////////////////////////////////////////////////////////
ggo::fixed_frames_count_animation_artist_abc::fixed_frames_count_animation_artist_abc(int width, int height, int line_step, ggo::image_format format, int frames_count)
:
ggo::animation_artist_abc(width, height, line_step, format),
_frames_count(frames_count)
{
}

//////////////////////////////////////////////////////////////
void ggo::fixed_frames_count_animation_artist_abc::render_frame(void * buffer, int frame_index, bool & finished)
{
  if (frame_index >= _frames_count)
  {
    finished = true;
  }
  else
  {
    render_frame(buffer, frame_index);

    finished = false;
  }
}
