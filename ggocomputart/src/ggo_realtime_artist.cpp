#include "ggo_realtime_artist.h"
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
  realtime_artist::realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order)
  :
  paint_artist(width, height, line_byte_step, pixel_type, memory_lines_order)
  {

  }

  //////////////////////////////////////////////////////////////
  realtime_artist * realtime_artist::create(realtime_artist_id artist_id, int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio fps)
  {
    switch (artist_id)
    {
    case realtime_artist_id::kanji:
      return new kanji_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order, fps);
    case realtime_artist_id::bozons:
      return new bozons_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order, fps);
    case realtime_artist_id::neon:
      return new neon_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order, fps);
    case realtime_artist_id::storni:
      return new storni_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order, fps);
    case realtime_artist_id::duffing:
      return new duffing_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order, fps);
    //case realtime_artist_id::lagaude:
    //  return new lagaude_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    //case realtime_artist_id::demeco:
    //  return new demeco_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    //case realtime_artist_id::wakenda:
    //  return new wakenda_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    //case realtime_artist_id::poupette:
    //  return new poupette_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    case realtime_artist_id::sonson:
      return new sonson_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);
    //case realtime_artist_id::badaboum:
    //  return new badaboum_realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order);

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
  progress_realtime_artist::progress_realtime_artist(int width, int height, int line_byte_step, ggo::pixel_type pixel_type, ggo::lines_order memory_lines_order, ggo::ratio duration)
    :
    realtime_artist(width, height, line_byte_step, pixel_type, memory_lines_order),
    _duration(duration)
  {

  }

  //////////////////////////////////////////////////////////////
  float progress_realtime_artist::progress() const
  {
    auto now = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start_time);

    return 0.001f * to<float>(elapsed_time.count() / _duration);
  }

  //////////////////////////////////////////////////////////////
  void progress_realtime_artist::preprocess_frame(void* buffer, uint32_t cursor_events, ggo::pos2_i cursor_pos)
  {
    preprocess_frame(buffer, cursor_events, cursor_pos, progress());
  }

  //////////////////////////////////////////////////////////////
  bool progress_realtime_artist::finished()
  {
    return progress() >= 1.f;
  }
}
