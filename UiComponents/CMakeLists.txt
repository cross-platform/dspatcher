project(UiComponents)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/DSPatchables/DSPatch/include
    ${CMAKE_SOURCE_DIR}/DSPatchables/Components
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)

add_library(
    ${PROJECT_NAME} SHARED
    ${srcs}
    ${hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}
    DSPatch
    InOut
)

add_subdirectory(FloatSlider)
