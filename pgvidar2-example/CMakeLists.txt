
cmake_minimum_required(VERSION 3.5)
#项目名称
project(vidar2_example)

set(CMAKE_C_STANDARD 11)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/include/opencv4
)
link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib
)

add_executable(vidar2_test
  src/vidar2_test.cpp
)

target_link_libraries(vidar2_test pgvidar2 glog vision_type opencv_world pgtools_helper)
