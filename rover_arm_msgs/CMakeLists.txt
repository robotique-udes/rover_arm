cmake_minimum_required(VERSION 3.0.2)
project(rover_arm_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
)

add_message_files(
  FILES
  encoder_status.msg
  desired_joint_state.msg
  joint_state.msg
  watchdog.msg
  feedback.msg
  arm_gui_cmd.msg
)

add_service_files(
  FILES
  joint_calib.srv
  diff_kinematics_calc.srv
)

generate_messages(
  DEPENDENCIES
)

catkin_package(
  CATKIN_DEPENDS message_generation
)
