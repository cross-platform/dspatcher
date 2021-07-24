project(UiComponent)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/DSPatchables/DSPatch/include
)

file(GLOB srcs *.cpp)

add_library(
    ${PROJECT_NAME} SHARED
    ${srcs}
)

target_link_libraries(
    ${PROJECT_NAME}
    DSPatch
    ${Qt5Widgets_LIBRARIES}
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/DSPatcher
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/DSPatcher
)

add_subdirectory(FloatSlider)
add_subdirectory(TextInput)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
