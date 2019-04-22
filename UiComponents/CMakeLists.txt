project(UiComponent)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/DSPatchables/DSPatch/include
)

file(GLOB srcs *.cpp)
file(GLOB hdrs *.h)

add_library(
    ${PROJECT_NAME} SHARED
    ${srcs}
    ${hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}
    DSPatch
)

qt5_use_modules(
  ${PROJECT_NAME}
  Widgets
)

add_subdirectory(UiGain)
