cmake_minimum_required(VERSION 3.0.2)
project(rover_arm)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  message_generation
  rover_arm_msgs
  rover_control_msgs
)

catkin_package(
  CATKIN_DEPENDS
    roscpp
    rospy
    std_msgs
    rover_arm_msgs
    rover_control_msgs
  )

include_directories(
  ${catkin_INCLUDE_DIRS}
)

install(FILES
  img/keybinding.png
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

add_executable(master src/master.cpp)
add_dependencies(master ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(master ${catkin_LIBRARIES})


add_executable(joint_watchdog src/joint_watchdog.cpp)
add_dependencies(joint_watchdog ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(joint_watchdog ${catkin_LIBRARIES})
