include ../Makefile.config

CFLAGS+=-I. -I./artists -I./animation -I./animation/paths -I./animation/animates -I../ggokernel/include -I../ggo2d/include -I../ggoraytracer/include
LIBDIR=lib
LIBS=../ggoraytracer/$(LIBDIR)/libggoraytracer.a ../ggo2d/$(LIBDIR)/libggo2d.a ../ggokernel/$(LIBDIR)/libggokernel.a
TARGETS=ggocomputartbitmaps ggocomputartanim

OBJS=artists/ggo_artist_abc.o \
     artists/ggo_bitmap_artist_abc.o \
     artists/ggo_animation_artist_abc.o \
     animation/ggo_animator.o \
     animation/ggo_animate_abc.o \
     animation/paths/ggo_spiral_path.o \
     animation/paths/ggo_linear_path.o \
     artists/duffing/ggo_duffing_bitmap_artist.o \
     artists/duffing/ggo_duffing_animation_artist.o \
     artists/worms/ggo_worms_artist.o \
     artists/trees/ggo_trees_artist.o \
     artists/flower/ggo_flower_artist.o \
     artists/julia/ggo_julia_artist.o \
     artists/julia/ggo_julia_bitmap_artist.o \
     artists/julia/ggo_julia_animation_artist.o \
     artists/cells/ggo_cells_artist.o \
     artists/buddhabrot/ggo_buddhabrot_artist.o \
     artists/circles/ggo_circles_artist.o \
     artists/circles/ggo_circles_bitmap_artist.o \
     artists/circles/ggo_circles_animation_artist.o \
     artists/neon/ggo_neon_artist.o \
     artists/kanji/ggo_kanji_artist.o \
     artists/kanji/ggo_kanji_bitmap_artist.o \
     artists/kanji/ggo_kanji_animation_artist.o \
     artists/bozons/ggo_bozons_animation_artist.o \
     artists/crystal/ggo_crystal_artist.o \
     artists/crystal/ggo_crystal_bitmap_artist.o \
     artists/crystal/ggo_crystal_animation_artist.o \
     artists/filling_squares/ggo_filling_squares_artist.o \
     artists/filling_squares/ggo_filling_squares_bitmap_artist.o \
     artists/filling_squares/ggo_filling_squares_animation_artist.o \
     artists/voronoi/ggo_voronoi_artist.o \
     artists/vortex/ggo_vortex_artist.o \
     artists/vortex2/ggo_vortex2_artist.o \
     artists/vortex2/ggo_vortex2_bitmap_artist.o \
     artists/vortex2/ggo_vortex2_animation_artist.o \
     artists/plastic/ggo_plastic_artist.o \
     artists/plastic/ggo_plastic_bitmap_artist.o \
     artists/plastic/ggo_plastic_animation_artist.o \
     artists/mandelbrot/ggo_mandelbrot_artist.o \
     artists/aggregation/ggo_aggregation_artist.o \
     artists/aggregation/ggo_aggregation_bitmap_artist.o \
     artists/aggregation/ggo_aggregation_animation_artist.o \
     artists/mondrian/ggo_mondrian_artist.o \
     artists/ifs/ggo_ifs_artist.o \
     artists/ifs/ggo_ifs_bitmap_artist.o \
     artists/ifs/ggo_ifs_animation_artist.o \
     artists/mosaic/ggo_mosaic_artist.o \
     artists/metaballs/ggo_metaballs_artist.o \
     artists/metaballs/ggo_metaballs_bitmap_artist.o \
     artists/metaballs/ggo_metaballs_animation_artist.o \
     artists/marbles/ggo_marbles_artist.o \
     artists/smoke/ggo_smoke_animation_artist.o \
     artists/flies/ggo_flies_artist.o \
     artists/newton/ggo_newton_artist.o \
     artists/alpha/ggo_alpha_bitmap_artist.o \
     artists/alpha/ggo_alpha_anim_artist.o \
     artists/topodoko/ggo_topodoko_artist.o \
     artists/dupecheck/ggo_dupecheck_animation_artist.o \
     artists/dupecheck/ggo_dupecheck_bitmap_artist.o \
     artists/rex/ggo_rex_artist.o \
     artists/lagaude/ggo_lagaude_animation_artist.o \
     artists/ikeda/ggo_ikeda_artist.o \
     artists/cumbia/ggo_cumbia_artist.o \
     artists/cumbia/ggo_cumbia_bitmap_artist.o \
     artists/cumbia/ggo_cumbia_animation_artist.o \
     artists/bubbles/ggo_bubbles_artist.o \
     artists/distorsion/ggo_distorsion_bitmap_artist.o \
     artists/distorsion/ggo_distorsion_animation_artist.o \
     artists/toutouyoutou/ggo_toutouyoutou_animation_artist.o \
     artists/toutouyoutou/ggo_toutouyoutou_bitmap_artist.o \
     artists/amorosi/ggo_amorosi_animation_artist.o \
     artists/rah/ggo_rah_animation_artist.o \
     artists/hexa/ggo_hexa_artist.o \
     artists/hexa/ggo_hexa_bitmap_artist.o \
     artists/hexa/ggo_hexa_animation_artist.o \
     artists/chryzode/ggo_chryzode_artist.o \
     artists/chryzode/ggo_chryzode_bitmap_artist.o \
     artists/chryzode/ggo_chryzode_animation_artist.o \
     artists/stoa/ggo_stoa_artist.o \
     artists/stoa/ggo_stoa_bitmap_artist.o \
     artists/stoa/ggo_stoa_animation_artist.o

all: $(TARGETS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

ggocomputartbitmaps: $(OBJS) ggo_computart_bitmaps.o $(LIBS)
	$(CC) $^ $(LDFLAGS) -o $@

ggocomputartanim: $(OBJS) ggo_computart_anim.o $(LIBS)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -fr $(TARGETS) $(OBJS) ggo_computart_bitmaps.o ggo_computart_anim.o

.PHONY: clean all

