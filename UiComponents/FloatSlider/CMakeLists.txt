project(FloatSlider)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB srcs *.cpp)
file(GLOB hdrs *.h)

add_library(
    ${PROJECT_NAME} SHARED
    ${srcs}
    ${hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}
    UiComponent
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/DSPatchables/Components
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib/dspatch/components)
