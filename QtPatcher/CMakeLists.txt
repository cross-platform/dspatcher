project(QtPatcher)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)
file(GLOB qrcs data/*.qrc)

qt5_add_resources(rcs ${qrcs})

add_library(
  ${PROJECT_NAME}
  ${srcs}
  ${hdrs}
  ${rcs}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${Qt5Widgets_LIBRARIES}
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
