search_mode=OPTIMIZE_MAX_JOINT
srdf_filename=open_manipulator.srdf
robot_name_in_srdf=open_manipulator
moveit_config_pkg=open_manipulator_moveit_config
robot_name=open_manipulator
planning_group_name=arm
ikfast_plugin_pkg=open_manipulator_ikfast_arm_plugin
base_link_name=link0
eef_link_name=end_effector_link
ikfast_output_path=/home/jeremie/catkin_ws/src/rover_arm/open_manipulator_ikfast_arm_plugin/src/open_manipulator_arm_ikfast_solver.cpp

rosrun moveit_kinematics create_ikfast_moveit_plugin.py\
  --search_mode=$search_mode\
  --srdf_filename=$srdf_filename\
  --robot_name_in_srdf=$robot_name_in_srdf\
  --moveit_config_pkg=$moveit_config_pkg\
  $robot_name\
  $planning_group_name\
  $ikfast_plugin_pkg\
  $base_link_name\
  $eef_link_name\
  $ikfast_output_path
