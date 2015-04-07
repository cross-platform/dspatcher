cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)

project(DSPatcher)

if(WIN32)
  # N.B. Edit this path if your Windows Qt path differs
  set(CMAKE_PREFIX_PATH C:/Qt/Qt5.2.1/5.2.1/msvc2010)
endif(WIN32)

if(APPLE)
  # N.B. Edit this path if your Mac Qt path differs
  set(CMAKE_PREFIX_PATH ~/Qt5.2.1/5.2.1/clang_64)
endif(APPLE)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(dspatcher)
add_subdirectory(qtpatcher)
