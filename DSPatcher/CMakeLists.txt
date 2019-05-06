include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/DSPatchables/DSPatch/include
    ${CMAKE_SOURCE_DIR}/QtPatcher/include
    ${CMAKE_SOURCE_DIR}/UiComponents
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)

add_definitions(-DDEFAULT_PLUGIN_DIR="${CMAKE_BINARY_DIR}/DSPatchables/Components")

add_executable(
  ${PROJECT_NAME}
  ${srcs}
  ${hdrs}
)

target_link_libraries(
  ${PROJECT_NAME}
  DSPatch
  QtPatcher
  UiComponent
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(WIN32)

  if(MSVC)

    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/../DSPatchables/DSPatch/$<CONFIGURATION>/DSPatch.dll
      ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>
    )

    set(
      QT5_DEBUG_DLLS
      libEGLd.dll libGLESv2d.dll Qt5Cored.dll Qt5Guid.dll Qt5Widgetsd.dll
    )

    foreach(QT5_DLL ${QT5_DEBUG_DLLS})
      add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_PREFIX_PATH}/bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Debug/${QT5_DLL}
      )
    endforeach(QT5_DLL ${QT5_DEBUG_DLLS})

    set(
      QT5_RELEASE_DLLS
      libEGL.dll libGLESv2.dll Qt5Core.dll Qt5Gui.dll Qt5Widgets.dll
    )

    foreach(QT5_DLL ${QT5_RELEASE_DLLS})
      add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_PREFIX_PATH}/bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/MinSizeRel/${QT5_DLL}
      )
      add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_PREFIX_PATH}/bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/Release/${QT5_DLL}
      )
      add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_PREFIX_PATH}/bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/${QT5_DLL}
      )
    endforeach(QT5_DLL ${QT5_RELEASE_DLLS})

  endif(MSVC)

  if(MINGW)

    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_BINARY_DIR}/../DSPatchables/DSPatch/libDSPatch.dll
      ${CMAKE_CURRENT_BINARY_DIR}
    )

    string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type)

    if (cmake_build_type MATCHES debug)
      set(
        QT5_DLLS
        libEGLd.dll libGLESv2d.dll Qt5Cored.dll Qt5Guid.dll Qt5Widgetsd.dll
       )
    else()
      set(
        QT5_DLLS
        libEGL.dll libGLESv2.dll Qt5Core.dll Qt5Gui.dll Qt5Widgets.dll
       )
    endif()

    foreach(QT5_DLL ${QT5_DLLS})
      add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_PREFIX_PATH}/bin/${QT5_DLL} ${CMAKE_CURRENT_BINARY_DIR}/${QT5_DLL}
      )
    endforeach(QT5_DLL ${QT5_DLLS})

    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_PREFIX_PATH}/bin/libgcc_s_dw2-1.dll
      ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_PREFIX_PATH}/bin/libstdc++-6.dll
      ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_PREFIX_PATH}/bin/libwinpthread-1.dll
      ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/../app.exe.manifest
      ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.exe.manifest
    )

  endif(MINGW)

endif(WIN32)
