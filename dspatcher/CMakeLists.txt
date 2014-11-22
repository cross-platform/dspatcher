include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/qtpatcher/include
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)

add_executable(
  ${PROJECT_NAME}
  ${srcs}
  ${hdrs}
)

target_link_libraries(
  ${PROJECT_NAME}
  QtPatcher
)

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
  endforeach(QT5_DLL ${QT5_DEBUG_DLLS})

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
  endforeach(QT5_DLL ${QT5_RELEASE_DLLS})
endif(WIN32)
