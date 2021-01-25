# IKFast tutorial
This tutorial shows how to create a custom kinematics solver plugin for MoveIt. The reason this is useful is because the recommended MoveIt solver does not work with robots with less than 6 DoF, so creating a custom one is needed.

## Prerequisites
* Docker
* Having your robot URDF ready

## Setting up the environment
First, you'll need to build the docker image containing the ros-openrave installation. It is easier to use the docker image than to build openrave from source. To build the docker image, go into the `ros-openrave` folder included in this repo and use this command: 
```bash
sudo docker build -t personalrobotics/ros-openrave .
```

## Converting urdf to collada
OpenRAVE needs the robot description in collada format in order to work.

If your robot description is in `.xacro` format, you'll need to convert it to plain `urdf` with this command: 
```bash
rosrun xacro xacro <your_robot_description>.urdf.xacro > <your_robot_description>.urdf
```

Then, you can convert the urdf to collada with 
```bash
rosrun collada_urdf urdf_to_collada <your_robot_description>.urdf <your_robot_description>.dae
```

Often floating point issues arrise in converting a URDF file to Collada file, so a script has been created to round all the numbers down to x decimal places in your .dae file. Its probably best if you skip this step initially and see if IKFast can generate a solution with your default values, but if the generator takes longer than, say, an hour, try the following:

```bash
rosrun moveit_ikfast round_collada_numbers.py <input_dae> <output_dae> <decimal places>
```

You can start with five decimal places at first and lower it if it still takes too long.

## Making wrapper for OpenRAVE
OpenRAVE needs a wrapper for the robot. You can name the wrapper `wrapper.xml` and place it in the same folder where you have your `.urdf` and your `.dae` files for simplicity.

`wrapper.xml`:
```xml
<robot file="$NAME_OF_YOUR_COLLADA_FILE">
  <Manipulator name="NAME_OF_THE_ROBOT_IN_URDF">
    <base>NAME_OF_THE_ROBOT_BASE_LINK</base>
    <effector>NAME_OF_THE_ROBOT_EFFECTOR_LINK</effector>
  </Manipulator>
</robot>
```

## Generating the solver
Before you generate the solver, you need to chose which IK type is needed for you robot. This page has more information: http://openrave.org/docs/latest_stable/openravepy/ikfast/#ik-types


Go to the directory where you have your `.urdf`, `.dae` and your `wrapper.xml` files.
Run the following command by remembering to choose the right IK type. In this example, translation3d was used.
```bash
sudo docker run -it --rm -v `pwd`:/ikfast -v `pwd`/output:/root/.openrave personalrobotics/ros-openrave openrave0.9.py --database inversekinematics --robot=/ikfast/wrapper.xml --iktype=translation3d --iktests=1000
```
Once it has finished running you should see an `output` folder containing the generated solver in your current working directory.

## Creating the plugin for MoveIt
Now that the IK solver has been generated, we need to create the plugin that will be used by MoveIt. To do so, we need to create a package containing the files generated earlier. It is recommended to call it `<myrobot_name>_ikfast_<planning_group_name>_plugin`.

First, create the package in the `src` folder of your catkin_ws and build it.
```bash
cd ~/catkin_ws/src
catkin_create_pkg <myrobot_name>_ikfast_<planning_group_name>_plugin
cd ..
catkin_make
```
Create the plugin source code:
```bash
rosrun moveit_kinematics create_ikfast_moveit_plugin.py <myrobot_name> <planning_group_name> <myrobot_name>_ikfast_<planning_group_name>_plugin <base_link_name> <end_effector_link_name> <ikfast_output_cpp_path>
```
Finally, use the MoveIt setup assistant to edit the robot's configuration package to make sure the IKFast solver is used.
