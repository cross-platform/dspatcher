cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -pedantic -Wall -Wextra -Wnon-virtual-dtor")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-gnu-zero-variadic-macro-arguments -Wno-vla-extension")

project(DSPatcher)

if(WIN32)
  # N.B. Edit this path if your Windows Qt path differs
  set(CMAKE_PREFIX_PATH C:/Qt/5.9.6/mingw53_32)
endif(WIN32)

if(APPLE)
  # N.B. Edit this path if your Mac Qt path differs
  set(CMAKE_PREFIX_PATH ~/Qt/5.9.6/clang_64)
endif(APPLE)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(DSPatchables)
add_subdirectory(DSPatcher)
add_subdirectory(QtPatcher)
