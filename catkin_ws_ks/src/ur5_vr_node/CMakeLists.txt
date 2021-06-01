cmake_minimum_required(VERSION 3.0.2)
project(ur5_vr_node)

add_compile_options(-std=c++14)


find_package(catkin REQUIRED COMPONENTS
	roscpp
	moveit_core
	moveit_ros_planning_interface
	rosparam_shortcuts
    interactive_markers
    moveit_core
    moveit_visual_tools
    moveit_ros_planning
    moveit_ros_planning_interface
    pluginlib
    geometric_shapes
    pcl_ros
    pcl_conversions
    rosbag
    tf2_ros
    tf2_eigen
    tf2_geometry_msgs
)


catkin_package()

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

add_executable(ur5_vr_node src/vr2ur5.cpp)
target_link_libraries(ur5_vr_node ${catkin_LIBRARIES})








