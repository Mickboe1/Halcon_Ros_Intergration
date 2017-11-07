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


#target_link_libraries(main libhalconc)

#include_directories(
#   ${catkin_INCLUDE_DIRS}
#  /opt/halcon/lib/x64-linux
#)

#target_link_libraries(main ${PROJECT_NAME})


target_link_libraries(treshhold libhalconcpp.so libhalcon.so libhdevenginecpp.so libhalconcppxl.so libhalconxl.so libhdevenginecppxl.so libhalconcpp10.so libhalconc.so libhdevenginecpp10.so libuastack.so libhalconcpp10xl.so libhalconcxl.so libhdevenginecpp10xl.so)

#target_link_libraries(main hAcqArgos3D-P1xx.so hAcqGigEVision.so hAcqMatrixVisionAcquire.so hAcquEye.so hioOPC_UA.so libhalconcpp.so libhalcon.so libhdevenginecpp.so hAcqArgos3D-P1xxxl.so hAcqGigEVisionxl.so hAcqMatrixVisionAcquirexl.so hAcquEyexl.so hioOPC_UAxl.so libhalconcppxl.so libhalconxl.so libhdevenginecppxl.so hAcqFile.so hAcqLPS36.so hAcqO3D3xx.so hAcqVideo4Linux2.so libhalconcpp10.so libhalconc.so libhdevenginecpp10.so libuastack.so hAcqFilexl.so hAcqLPS36xl.so hAcqO3D3xxxl.so hAcqVideo4Linux2xl.so libhalconcpp10xl.so libhalconcxl.so libhdevenginecpp10xl.so libusb-1.0-usan.so.0)


#target_link_libraries(main )
#add_dependencies(main ${PROJECT_NAME})
#target_link_libraries(main ${catkin_LIBRARIES})
