#ifndef __GGO_ARTISTS_IDS__
#define __GGO_ARTISTS_IDS__

namespace ggo
{
  enum class bitmap_artist_id
  {
    trees, // 0
    flower,
    duffing,
    worms,
    julia,
    cells, // 5
    buddhabrot,
    kanji,
    bozons,
    crystal,
    filling_squares, // 10
    voronoi,
    ikeda,
    demeco,
    vortex,
    plastic, //15
    mandelbrot,
    mondrian__REMOVED,
    ifs,
    mosaic,
    metaballs, // 20
    marbles,
    alpha,
    dupecheck,
    topodoko,
    rex, // 25
    smoke,
    lagaude,
    cumbia,
    circles__REMOVED,
    aggregation, // 30
    bubble,
    distorsion,
    toutouyoutou,
    amorosi,
    rah, //35
    hexa,
    chryzode,
    stoa,
    sonson__REMOVED,
    rediff, // 40
    entabeni,
    cabrel,
    kame
  };
}

namespace ggo
{
  enum class animation_artist_id
  {
    duffing, // 0
    julia,
    kanji,
    bozons,
    filling_squares,
    plastic, // 5
    aggregation,
    ifs,
    metaballs,
    rubiks_cube,
    smoke, // 10
    sonson__REMOVED,
    newton__REMOVED,
    alpha,
    dupecheck,
    lagaude, // 15
    crystal,
    ikeda,
    cumbia,
    vortex,
    circles__REMOVED, // 20
    distorsion,
    toutouyoutou,
    amorosi,
    rah,
    hexa, // 25
    chryzode,
    stoa,
    kame,
    rediff,
    entabeni, // 30
    neon,
    storni
  };
}

namespace ggo
{
  enum class realtime_artist_id
  {
    kanji, // 0
    bozons,
    neon,
    storni,
    lagaude,
    duffing // 5
  };
}

#endif

