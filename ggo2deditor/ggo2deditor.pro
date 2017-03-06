TEMPLATE = app
LANGUAGE = C++
CONFIG -= app_bundle
QT += widgets

CONFIG( debug, debug|release ) {
  # debug
  QMAKE_LIBDIR += $$PWD/../x64/Debug
} else {
  # release
  QMAKE_LIBDIR += $$PWD/../x64/Release
}
LIBS += ggokernel.lib ggo2d.lib

INCLUDEPATH = ../ggokernel/src \
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
              ../ggo2d/src/rle_image

HEADERS = src/MainWindow.h src \
          src/RenderWidget.h \
          src/ShapeFactory.h \
          src/DiscFactory.h \
          src/PolygonFactory.h \
          src/ShapeHandler.h \
          src/DiscHandler.h \
          src/PolygonHandler.h
SOURCES = src/main.cpp \
          src/MainWindow.cpp \
          src/RenderWidget.cpp \
          src/DiscFactory.cpp \
          src/PolygonFactory.cpp \
          src/ShapeHandler.cpp \
          src/DiscHandler.cpp \
          src/PolygonHandler.cpp
