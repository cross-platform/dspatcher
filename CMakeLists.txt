cmake_minimum_required(VERSION 3.1)

project(DSPatcher)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4")
elseif(MINGW)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic -Wall -Wextra -Wnon-virtual-dtor -Wno-unknown-pragmas")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -pedantic -Wall -Wextra -Wnon-virtual-dtor")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-gnu-zero-variadic-macro-arguments -Wno-vla -Wno-vla-extension")
endif()

if(NOT CMAKE_PREFIX_PATH)

  if(APPLE)
    find_program(BREW_EXECUTABLE brew)
    if(BREW_EXECUTABLE)
      execute_process(COMMAND ${BREW_EXECUTABLE} --prefix qt5 OUTPUT_VARIABLE QT5_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
      if(QT5_PREFIX)
        set(CMAKE_PREFIX_PATH "${QT5_PREFIX}")
      else()
        execute_process(COMMAND ${BREW_EXECUTABLE} --prefix qt OUTPUT_VARIABLE QT_PREFIX OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(QT_PREFIX)
          set(CMAKE_PREFIX_PATH "${QT_PREFIX}")
        else()
          file(GLOB QT_VERSIONS "$ENV{HOME}/Qt/5.*")
        endif()
      endif()
    else()
      file(GLOB QT_VERSIONS "$ENV{HOME}/Qt/5.*")
    endif()

  elseif(WIN32)

    file(GLOB QT_VERSIONS "C:/Qt/5.*")

  endif()

  if(QT_VERSIONS)

    list(GET QT_VERSIONS 0 QT_VERSION_PATH)

    file(GLOB QT_COMPILERS RELATIVE ${QT_VERSION_PATH} ${QT_VERSION_PATH}/*)

    if(MSVC)

      foreach(QT_COMPILER ${QT_COMPILERS})
        if(${QT_COMPILER} MATCHES "msvc")
          set(CMAKE_PREFIX_PATH "${QT_VERSION_PATH}/${QT_COMPILER}")
          break()
        endif()
      endforeach()

    elseif(MINGW)

      foreach(QT_COMPILER ${QT_COMPILERS})
        if(${QT_COMPILER} MATCHES "mingw")
          set(CMAKE_PREFIX_PATH "${QT_VERSION_PATH}/${QT_COMPILER}")
          break()
        endif()
      endforeach()

    else()

      list(GET QT_COMPILERS 0 QT_COMPILER)
      set(CMAKE_PREFIX_PATH "${QT_VERSION_PATH}/${QT_COMPILER}")

    endif()

  endif(QT_VERSIONS)

endif(NOT CMAKE_PREFIX_PATH)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(DSPatchables)
add_subdirectory(DSPatcher)
add_subdirectory(QtPatcher)
add_subdirectory(UiComponents)
