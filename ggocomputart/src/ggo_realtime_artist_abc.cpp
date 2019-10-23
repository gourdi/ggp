#include "ggo_realtime_artist_abc.h"
#include "kanji/ggo_kanji_realtime_artist.h"
#include "bozons/ggo_bozons_realtime_artist.h"
#include "neon/ggo_neon_realtime_artist.h"
#include "storni/ggo_storni_realtime_artist.h"
#include "duffing/ggo_duffing_realtime_artist.h"
#include "lagaude/ggo_lagaude_realtime_artist.h"
#include "demeco/ggo_demeco_realtime_artist.h"
#include "wakenda/ggo_wakenda_realtime_artist.h"
#include "poupette/ggo_poupette_realtime_artist.h"
#include "sonson/ggo_sonson_realtime_artist.h"
#include "badaboum/ggo_badaboum_realtime_artist.h"

namespace ggo
{
  //////////////////////////////////////////////////////////////
  realtime_artist_abc::realtime_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
  :
  paint_artist(width, height, line_byte_step, pixel_type, memory_lines_order)
  {

  }

  //////////////////////////////////////////////////////////////
  void realtime_artist_abc::preprocess_frame(uint32_t cursor_events, ggo::pos2_i cursor_pos, float time_step)
  {
    preprocess_frame(_frame_index, cursor_events, cursor_pos, time_step);
  }

  //////////////////////////////////////////////////////////////
  void realtime_artist_abc::render_tile(void * buffer, const rect_int & clipping)
  {
    render_tile(buffer, _frame_index, clipping);
  }

  //////////////////////////////////////////////////////////////
  bool realtime_artist_abc::finished()
  {
    if (finished(_frame_index) == true)
    {
      return true;
    }

    ++_frame_index;

    return false;
  }

  //////////////////////////////////////////////////////////////
  realtime_artist_abc * realtime_artist_abc::create(realtime_artist_id artist_id, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
  {
    switch (artist_id)
    {
    case realtime_artist_id::kanji:
      return new kanji_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::bozons:
      return new bozons_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::neon:
      return new neon_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::storni:
      return new storni_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::duffing:
      return new duffing_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::lagaude:
      return new lagaude_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::demeco:
      return new demeco_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::wakenda:
      return new wakenda_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::poupette:
      return new poupette_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::sonson:
      return new sonson_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::badaboum:
      return new badaboum_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);

    default:
      GGO_FAIL();
      return nullptr;
    };

    return nullptr;
  }
}

namespace ggo
{
  //////////////////////////////////////////////////////////////
  fixed_frames_count_realtime_artist_abc::fixed_frames_count_realtime_artist_abc(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
  :
  realtime_artist_abc(width, height, line_byte_step, pixel_type, memory_lines_order)
  {

  }

  //////////////////////////////////////////////////////////////
  bool fixed_frames_count_realtime_artist_abc::finished(int frame_index) const
  {
    return frame_index >= frames_count();
  }
}
