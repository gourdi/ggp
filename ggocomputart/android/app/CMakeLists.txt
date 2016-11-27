# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds it for you.
# Gradle automatically packages shared libraries with your APK.

add_definitions(-D__ANDROID__ -DGL_GLEXT_PROTOTYPES)

set(SDL2_SOURCE_DIR "../../../externals/SDL2-2.0.4/")

include_directories(
  ${SDL2_SOURCE_DIR}/include
  ${SDL2_SOURCE_DIR}/src/render/opengles
  ${SDL2_SOURCE_DIR}/src/render/opengles2)

file(GLOB SDL2_SOURCE_FILES
  ${SDL2_SOURCE_DIR}/src/*.c
  ${SDL2_SOURCE_DIR}/src/atomic/*.c
  ${SDL2_SOURCE_DIR}/src/audio/*.c
  ${SDL2_SOURCE_DIR}/src/audio/android/*.c
  ${SDL2_SOURCE_DIR}/src/audio/dummy/*.c
  ${SDL2_SOURCE_DIR}/src/core/android/*.c
  ${SDL2_SOURCE_DIR}/src/cpuinfo/*.c
  ${SDL2_SOURCE_DIR}/src/dynapi/*.c
  ${SDL2_SOURCE_DIR}/src/events/*.c
  ${SDL2_SOURCE_DIR}/src/file/*.c
  ${SDL2_SOURCE_DIR}/src/filesystem/android/*.c
  ${SDL2_SOURCE_DIR}/src/haptic/*.c
  ${SDL2_SOURCE_DIR}/src/haptic/dummy/*.c
  ${SDL2_SOURCE_DIR}/src/libm/*.c
  ${SDL2_SOURCE_DIR}/src/loadso/dlopen/*.c
  ${SDL2_SOURCE_DIR}/src/joystick/*.c
  ${SDL2_SOURCE_DIR}/src/joystick/android/*.c
  ${SDL2_SOURCE_DIR}/src/power/*.c
  ${SDL2_SOURCE_DIR}/src/power/android/*.c
  ${SDL2_SOURCE_DIR}/src/stdlib/*.c
  ${SDL2_SOURCE_DIR}/src/render/*.c
  ${SDL2_SOURCE_DIR}/src/render/opengles/*.c
  ${SDL2_SOURCE_DIR}/src/render/opengles2/*.c
  ${SDL2_SOURCE_DIR}/src/render/software/*.c
  ${SDL2_SOURCE_DIR}/src/thread/*.c
  ${SDL2_SOURCE_DIR}/src/thread/pthread/*.c
  ${SDL2_SOURCE_DIR}/src/timer/*.c
  ${SDL2_SOURCE_DIR}/src/timer/unix/*.c
  ${SDL2_SOURCE_DIR}/src/video/*.c
  ${SDL2_SOURCE_DIR}/src/video/android/*.c
  )

add_library(SDL2 SHARED ${SDL2_SOURCE_FILES})
add_library(main SHARED src/main/cpp/ggocomputart/main.cpp src/main/cpp/ggocomputart/SDL_android_main.c)


# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library(log-lib log)
find_library(android-lib android)
find_library(EGL-lib EGL)
find_library(GLESv1_CM-lib GLESv1_CM)
find_library(GLESv2-lib GLESv2)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.


target_link_libraries( SDL2

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} ${android-lib} ${EGL-lib} ${GLESv1_CM-lib} ${GLESv2-lib})

target_link_libraries( # Specifies the target library.
                       main SDL2)
