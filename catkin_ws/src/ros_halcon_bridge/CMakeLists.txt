cmake_minimum_required(VERSION 2.8.3)
project(ros_halcon_bridge)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  asr_halcon_bridge
)

catkin_package(
    LIBRARIES ${PROJECT_NAME}
)


include_directories(include ${catkin_INCLUDE_DIRS})
link_directories(/opt/halcon/lib/x64-linux)

add_executable(treshhold src/treshhold.cpp)
add_dependencies(treshhold ${PROJECT_NAME})
target_link_libraries(treshhold ${catkin_LIBRARIES})
target_link_libraries(treshhold libhalconcpp.so libhalcon.so libhdevenginecpp.so libhalconcppxl.so libhalconxl.so libhdevenginecppxl.so libhalconcpp10.so libhalconc.so libhdevenginecpp10.so libuastack.so libhalconcpp10xl.so libhalconcxl.so libhdevenginecpp10xl.so)

add_executable(erosion src/erosion.cpp)
add_dependencies(erosion ${PROJECT_NAME})
target_link_libraries(erosion ${catkin_LIBRARIES})
target_link_libraries(erosion libhalconcpp.so libhalcon.so libhdevenginecpp.so libhalconcppxl.so libhalconxl.so libhdevenginecppxl.so libhalconcpp10.so libhalconc.so libhdevenginecpp10.so libuastack.so libhalconcpp10xl.so libhalconcxl.so libhdevenginecpp10xl.so)

add_executable(dilation src/dilation.cpp)
add_dependencies(dilation ${PROJECT_NAME})
target_link_libraries(dilation ${catkin_LIBRARIES})
target_link_libraries(dilation libhalconcpp.so libhalcon.so libhdevenginecpp.so libhalconcppxl.so libhalconxl.so libhdevenginecppxl.so libhalconcpp10.so libhalconc.so libhdevenginecpp10.so libuastack.so libhalconcpp10xl.so libhalconcxl.so libhdevenginecpp10xl.so)
