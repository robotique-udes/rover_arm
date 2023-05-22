cmake_minimum_required(VERSION 3.0.2)
project(rover_arm)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  message_generation
  rover_arm_msg
)

#The following block needs to go
################################################################################
## Generate messages in the 'msg' folder
add_message_files(
  DIRECTORY 
  msg 
  FILES
  feedback.msg
  vitesse_moteur_msg.msg
  angle.msg
  arm_gui_cmd.msg
)

# Generate services in the 'srv' folder
add_service_files(
  DIRECTORY 
  srv
  FILES
  diffKinematicsCalc.srv
)

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
)
################################################################################

catkin_package(
   CATKIN_DEPENDS roscpp rospy std_msgs rover_arm_msg
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  ${catkin_INCLUDE_DIRS}
)

# catkin_install_python(PROGRAMS
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
install(FILES
  img/keybinding.png
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

add_executable(master_cpp src/master.cpp)
target_link_libraries(master_cpp ${catkin_LIBRARIES})
add_dependencies(master_cpp rover_arm_generate_messages_cpp)

add_executable(joint_watchdog src/joint_watchdog.cpp)
add_dependencies(joint_watchdog ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(joint_watchdog ${catkin_LIBRARIES})
