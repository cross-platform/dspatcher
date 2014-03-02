cmake_minimum_required(VERSION 2.8)

if(WIN32)
  # N.B. Edit this path if your Windows Qt path differs
  set(CMAKE_PREFIX_PATH C:/Qt/Qt5.2.1/5.2.1/msvc2010/)
endif(WIN32)

if(APPLE)
  # N.B. Edit this path if your Mac Qt path differs
  set(CMAKE_PREFIX_PATH ~/Qt5.2.1/5.2.1/clang_64)
endif(APPLE)

add_definitions(
    -fno-permissive
    -Wall
    -Wextra
)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(dspatcher)
add_subdirectory(qtpatcher)

if(WIN32)
  set(
    QT5_DEBUG_DLLS
    icudt51.dll icuin51.dll icuuc51.dll
    libEGLd.dll libGLESv2d.dll Qt5Cored.dll Qt5Guid.dll Qt5Widgetsd.dll
  )

  foreach(QT5_DLL ${QT5_DEBUG_DLLS})
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_PREFIX_PATH}bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug/${QT5_DLL}
    )
  endforeach(QT5_DLL ${QT5_DLLS})

  set(
    QT5_RELEASE_DLLS
    icudt51.dll icuin51.dll icuuc51.dll
    libEGL.dll libGLESv2.dll Qt5Core.dll Qt5Gui.dll Qt5Widgets.dll
  )

  foreach(QT5_DLL ${QT5_RELEASE_DLLS})
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_PREFIX_PATH}bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/MinSizeRel/${QT5_DLL}
    )
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_PREFIX_PATH}bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Release/${QT5_DLL}
    )
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_PREFIX_PATH}bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/${QT5_DLL}
    )
  endforeach(QT5_DLL ${QT5_DLLS})
endif(WIN32)
