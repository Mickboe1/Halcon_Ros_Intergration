cmake_minimum_required(VERSION 2.8.3)
project(ros_halcon_bridge)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  asr_halcon_bridge
  message_generation
)
add_message_files(
   FILES
   Lego.msg
)
generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
)

catkin_package(
    LIBRARIES ${PROJECT_NAME}
    CATKIN_DEPENDS message_runtime
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

add_executable(background_extraction src/background_extraction.cpp)
add_dependencies(background_extraction ${PROJECT_NAME})
target_link_libraries(background_extraction ${catkin_LIBRARIES})
target_link_libraries(background_extraction libhalconcpp.so libhalcon.so libhdevenginecpp.so libhalconcppxl.so libhalconxl.so libhdevenginecppxl.so libhalconcpp10.so libhalconc.so libhdevenginecpp10.so libuastack.so libhalconcpp10xl.so libhalconcxl.so libhdevenginecpp10xl.so)

add_executable(legos src/legos.cpp)
add_dependencies(legos ${PROJECT_NAME})
target_link_libraries(legos ${catkin_LIBRARIES})
target_link_libraries(legos libhalconcpp.so libhalcon.so libhdevenginecpp.so libhalconcppxl.so libhalconxl.so libhdevenginecppxl.so libhalconcpp10.so libhalconc.so libhdevenginecpp10.so libuastack.so libhalconcpp10xl.so libhalconcxl.so libhdevenginecpp10xl.so)
