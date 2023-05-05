cmake_minimum_required(VERSION 3.0.2)
project(rover_arm_msg)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  message_generation
)

add_message_files(
  FILES
  encoder_status.msg
  desired_joint_state.msg
  joint_state.msg
)

# add_service_files(
#   FILES
# )

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
   CATKIN_DEPENDS std_msgs
)
