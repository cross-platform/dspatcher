cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

project(DSPatcher)

if(WIN32)
  # N.B. Edit this path if your Windows Qt path differs
  set(CMAKE_PREFIX_PATH C:/Qt/Qt5.2.1/5.2.1/msvc2010)
endif(WIN32)

if(APPLE)
  # N.B. Edit this path if your Mac Qt path differs
  set(CMAKE_PREFIX_PATH ~/Qt/5.9.6/clang_64)
endif(APPLE)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(DSPatchables)
add_subdirectory(DSPatcher)
add_subdirectory(QtPatcher)
