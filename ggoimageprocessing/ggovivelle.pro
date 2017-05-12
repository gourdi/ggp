TEMPLATE = app
LANGUAGE = C++
CONFIG -= app_bundle
QT += widgets

CONFIG( debug, debug|release ) {
  # debug
  QMAKE_LIBDIR += $$PWD/../x64/Debug $$PWD/../externals/LibRaw-0.18-stable/x64/Debug
} else {
  # release
  QMAKE_LIBDIR += $$PWD/../x64/Release $$PWD/../externals/LibRaw-0.18-stable/x64/Release
}
LIBS += ggokernel.lib ggo2d.lib ggoimageprocessing.lib libraw.lib

INCLUDEPATH = ../ggokernel/src \
              ../ggokernel/src/memory \
              ../ggokernel/src/rle \
              ../ggokernel/src/math \
              ../ggokernel/src/math/shapes_2d \
              ../ggokernel/src/math/shapes_3d \
              ../ggokernel/src/math/marching_algorithms \
              ../ggokernel/src/math/delaunay_triangulation \
              ../ggokernel/src/math/signal_proc \
              ../ggokernel/src/math/linear_algebra \
              ../ggokernel/src/math/sampling/low_discrepancy_sequences \
              ../ggokernel/src/math/sampling/shape_sampling \
              ../ggokernel/src/math/sampling/pixel_sampling \
              ../ggokernel/src/math/interpolation \
              ../ggokernel/src/math/scalar_fields_2d \
              ../ggo2d/src/ \
              ../ggo2d/src/fill \
              ../ggo2d/src/blur \
              ../ggo2d/src/processing \
              ../ggo2d/src/space_partitionning \
              ../ggo2d/src/paint \
              ../ggo2d/src/rle_image \
              ../ggo2d/src/canvas \
              ../ggoimageprocessing/src

HEADERS = src/MainWindow.h \
          src/ImageWidget.h
SOURCES = src/main.cpp \
          src/MainWindow.cpp \
          src/ImageWidget.cpp 
