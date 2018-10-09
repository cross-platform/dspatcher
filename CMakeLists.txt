cmake_minimum_required(VERSION 3.0)

project(DSPatcher)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4")
elseif(MINGW)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic -Wall -Wextra -Wnon-virtual-dtor -Wno-unknown-pragmas")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -pedantic -Wall -Wextra -Wnon-virtual-dtor")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-gnu-zero-variadic-macro-arguments -Wno-vla-extension")
endif()

if(WIN32)
  # N.B. Edit this if your Windows Qt path differs
  if(MSVC)
    set(CMAKE_PREFIX_PATH C:/Qt/5.11.2/msvc2015)
  else()
    set(CMAKE_PREFIX_PATH C:/Qt/5.9.6/mingw53_32)
  endif()
endif(WIN32)

if(APPLE)
  # N.B. Edit this if your Mac Qt path differs
  set(CMAKE_PREFIX_PATH ~/Qt/5.9.6/clang_64)
endif(APPLE)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(DSPatchables)
add_subdirectory(DSPatcher)
add_subdirectory(QtPatcher)
