#include "ggo_bitmap_artist_abc.h"
#include "ggo_animation_artist_abc.h"
#include "ggo_artist_ids.h"
#include "duffing/ggo_duffing_bitmap_artist.h"
#include "trees/ggo_trees_artist.h"
#include "worms/ggo_worms_artist.h"
#include "flower/ggo_flower_artist.h"
#include "cells/ggo_cells_artist.h"
#include "buddhabrot/ggo_buddhabrot_artist.h"
#include "crystal/ggo_crystal_bitmap_artist.h"
#include "filling_squares/ggo_filling_squares_bitmap_artist.h"
#include "voronoi/ggo_voronoi_artist.h"
#include "vortex/ggo_vortex_bitmap_artist.h"
#include "plastic/ggo_plastic_bitmap_artist.h"
#include "mandelbrot/ggo_mandelbrot_artist.h"
#include "ifs/ggo_ifs_bitmap_artist.h"
#include "mosaic/ggo_mosaic_artist.h"
#include "metaballs/ggo_metaballs_bitmap_artist.h"
#include "marbles/ggo_marbles_artist.h"
#include "julia/ggo_julia_bitmap_artist.h"
#include "dupecheck/ggo_dupecheck_animation_artist.h"
#include "cumbia/ggo_cumbia_bitmap_artist.h"
#include "topodoko/ggo_topodoko_artist.h"
#include "bozons/ggo_bozons_realtime_artist.h"
#include "rex/ggo_rex_artist.h"
#include "bubbles/ggo_bubbles_artist.h"
#include "hexa/ggo_hexa_bitmap_artist.h"
#include "chryzode/ggo_chryzode_bitmap_artist.h"
#include "stoa/ggo_stoa_bitmap_artist.h"
#include "rediff/ggo_rediff_animation_artist.h"
#include "entabeni/ggo_entabeni_bitmap_artist.h"
#include "cabrel/ggo_cabrel_bitmap_artist.h"

//////////////////////////////////////////////////////////////
// ANIMATION ARTISTS WRAPPER

//////////////////////////////////////////////////////////////
namespace ggo
{
  class bitmap_artist_animation_wrapper : public bitmap_artist_abc
  {
  public:

    // A 'zero' frames count means: process all frames.
    bitmap_artist_animation_wrapper(ggo::animation_artist_id artist_id,
      int width, int height, int line_step,
      ggo::image_format format,
      int frames_count = 0,
      bool render_last_frame_only = true);

    void	render_bitmap(void * buffer) const override;

  private:

    ggo::animation_artist_id	_artist_id;
    int						            _frames_count;
    bool                      _render_last_frame_only;
  };
}

//////////////////////////////////////////////////////////////
ggo::bitmap_artist_animation_wrapper::bitmap_artist_animation_wrapper(ggo::animation_artist_id artist_id,
  int width, int height, int line_step,
  ggo::image_format format,
  int frames_count,
  bool render_last_frame_only)
  :
  bitmap_artist_abc(width, height, line_step, format)
{
  _artist_id = artist_id;
  _frames_count = frames_count;
  _render_last_frame_only = render_last_frame_only;

  GGO_ASSERT(frames_count > 0 || render_last_frame_only == true);
}

//////////////////////////////////////////////////////////////
void ggo::bitmap_artist_animation_wrapper::render_bitmap(void * buffer) const
{
  std::unique_ptr<ggo::animation_artist_abc> artist(ggo::animation_artist_abc::create(_artist_id, width(), height(), line_step(), format()));

  if (!artist)
  {
    throw std::runtime_error("failed creating aniamtion artist");
  }

  int frame_index = 0;

  while (true)
  {
    if (_frames_count > 0 && frame_index >= _frames_count)
    {
      break;
    }

    void * frame_buffer = buffer;
    if (_render_last_frame_only == true && frame_index != _frames_count - 1)
    {
      frame_buffer = nullptr;
    }

    if (artist->render_frame(frame_buffer) == false)
    {
      break;
    }

    ++frame_index;
  }
}

//////////////////////////////////////////////////////////////
// BITMAP ARTISTS

//////////////////////////////////////////////////////////////
ggo::bitmap_artist_abc::bitmap_artist_abc(int width, int height, int line_step, ggo::image_format format)
:
artist(width, height, line_step, format)
{

}

//////////////////////////////////////////////////////////////
ggo::bitmap_artist_abc * ggo::bitmap_artist_abc::create(bitmap_artist_id artist_id, int width, int height, int line_step, ggo::image_format format)
{
  switch (artist_id)
  {
  case ggo::bitmap_artist_id::duffing:
    return new ggo::duffing_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::trees:
    return new ggo::trees_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::worms:
    return new ggo::worms_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::flower:
    return new ggo::flower_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::cells:
    return new ggo::cells_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::buddhabrot:
    return new ggo::buddhabrot_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::crystal:
    return new ggo::crystal_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::filling_squares:
    return new ggo::filling_squares_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::voronoi:
    return new ggo::voronoi_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::vortex:
    return new ggo::vortex_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::plastic:
    return new ggo::plastic_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::mandelbrot:
    return new ggo::mandelbrot_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::ifs:
    return new ggo::ifs_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::mosaic:
    return new ggo::mosaic_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::metaballs:
    return new ggo::metaballs_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::marbles:
    return new ggo::marbles_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::julia:
    return new ggo::julia_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::topodoko:
    return new ggo::topodoko_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::rex:
    return new ggo::rex_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::bubble:
    return new ggo::bubbles_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::cumbia:
    return new ggo::cumbia_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::hexa:
    return new ggo::hexa_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::chryzode:
    return new ggo::chryzode_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::stoa:
    return new ggo::stoa_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::entabeni:
    return new ggo::entabeni_bitmap_artist(width, height, line_step, format);
  case ggo::bitmap_artist_id::cabrel:
    return new ggo::cabrel_bitmap_artist(width, height, line_step, format);

    // animation artists.
  case ggo::bitmap_artist_id::smoke:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::smoke, width, height, line_step, format, 400);
  case ggo::bitmap_artist_id::ikeda:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::ikeda, width, height, line_step, format, 1);
  case ggo::bitmap_artist_id::lagaude:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::lagaude, width, height, line_step, format, 25);
  case ggo::bitmap_artist_id::amorosi:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::amorosi, width, height, line_step, format, 100, true);
  case ggo::bitmap_artist_id::bozons:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::bozons, width, height, line_step, format);
  case ggo::bitmap_artist_id::rah:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::rah, width, height, line_step, format, 500, true);
  case ggo::bitmap_artist_id::rediff:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::rediff, width, height, line_step, format, 350, true);
  case ggo::bitmap_artist_id::aggregation:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::aggregation, width, height, line_step, format, 300, true);
  case ggo::bitmap_artist_id::kame:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::kame, width, height, line_step, format, 50, true);
  case ggo::bitmap_artist_id::dupecheck:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::dupecheck, width, height, line_step, format);
  case ggo::bitmap_artist_id::kanji:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::kanji, width, height, line_step, format);
  case ggo::bitmap_artist_id::toutouyoutou:
    return new ggo::bitmap_artist_animation_wrapper(ggo::animation_artist_id::toutouyoutou, width, height, line_step, format);

  default:
    GGO_FAIL();
    return nullptr;
  }
}


