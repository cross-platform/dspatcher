project(DSPatcher)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../qtpatcher/include
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

qt5_use_modules(
  ${PROJECT_NAME}
  Widgets
)
