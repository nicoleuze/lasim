#include <ros/ros.h>
#include <moveit/trajectory_processing/iterative_time_parameterization.h>

#include <moveit/planning_scene/planning_scene.h>
#include <moveit/robot_state/robot_state.h>

#include <moveit/planning_scene_monitor/planning_scene_monitor.h>

#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <moveit_visual_tools/moveit_visual_tools.h>

class vr2ur5 {
    public:
        vr2ur5(ros::NodeHandle * nh, planning_scene_monitor::PlanningSceneMonitorPtr psm, std::string planningGroupName_= "manipulator");
        
        ~vr2ur5();
        
        bool canInterpolate();        
        void startLooping();

        goal_sub = nh.subscribe<sensor_msgs::JointState>("/socket_goalState", 1000, &goals_vr2real::goalState_Callback, this);
        goal_msg.position.resize(6);
        goal_msg.name.reserve(6);

        current_joints_sub = nh.subscribe<sensor_msgs::JointState>("/joint_states", 1000, &goals_vr2real::current_joints_Callback, this);
        current_joint.position.resize(6);
        current_joint.name.reserve(6);


      /*
      

      current_joint.name.push_back("elbow_joint");
      current_joint.name.push_back("shoulder_lift_joint");
      current_joint.name.push_back("shoulder_pan_joint");
      current_joint.name.push_back("wrist_1_joint");
      current_joint.name.push_back("wrist_2_joint");
      current_joint.name.push_back("wrist_3_joint");

      //Base Joint Commands
      base_joint_command.joint_names.push_back("shoulder_pan_joint");
      base_joint_command.joint_names.push_back("shoulder_lift_joint");
      base_joint_command.joint_names.push_back("elbow_joint");
      base_joint_command.joint_names.push_back("wrist_1_joint");
      base_joint_command.joint_names.push_back("wrist_2_joint");
      base_joint_command.joint_names.push_back("wrist_3_joint");

      for(int i = 0; i < 6; i++)  base_joint_command.velocities.push_back(1.0);   

      */
        //Callback Functions ----------------------------------------------------------------
        void goalState_Callback(const sensor_msgs::JointState::ConstPtr& msg)
        {
          for(int i = 0; i < 6; i++)  
            goal_msg.position[i] = msg->position[i];    
        }

        void current_joints_Callback(const sensor_msgs::JointState::ConstPtr& msg)
        {
            
          current_joint.position[0] = msg->position[2]; 
          current_joint.position[1] = msg->position[1]; 
          current_joint.position[2] = msg->position[0]; 
          current_joint.position[3] = msg->position[3]; 
          current_joint.position[4] = msg->position[4]; 
          current_joint.position[5] = msg->position[5]; 
        }
        //-----------------------------------------------------------------------------------
        std::string planningGroupName;

        moveit::planning_interface::MoveGroupInterface move_group;
        const robot_state::JointModelGroup* joint_model_group;
        moveit_visual_tools::MoveItVisualTools visual_tools;

        planning_scene_monitor::PlanningSceneMonitorPtr psm;
        planning_scene_monitor::LockedPlanningSceneRO lockedPlanningScene;
        
        //std::vector<double> currentGoalValues;
        sensor_msgs::JointState goal_msg;
        sensor_msgs::JointState current_joint;
        control_msgs::JointJog base_joint_command;


};

vr2ur5::vr2ur5(ros::NodeHandle * nh, planning_scene_monitor::PlanningSceneMonitorPtr psm, std::string planningGroupName_) :
    planningGroupName(planningGroupName_),
    move_group(planningGroupName),
    lockedPlanningScene(psm),
    visual_tools(move_group.getPlanningFrame())
    {
        joint_model_group = move_group.getCurrentState()->getJointModelGroup(planningGroupName);
        currentGoalValues.resize(6);
        visual_tools.deleteAllMarkers();
    }

void vr2ur5::startLooping(){
    
    while(ros::ok()){
        if(canInterpolate()){
            std::cout << "Hier sollte Ihr Aufruf des Controller-Servers stehen!" << std::endl;
        }
        else{
            std::cout << "Interpolation gescheitert! \n Verwende Planer: "<< move_group.getPlannerId() << std::endl;
            bool success = move_grou{
    joint_model_group = move_group.getCurrentState()->getJointModelGroup(planningGroupName);
    currentGoalValues.resize(6);
    visual_tools.deleteAllMarkers();
llMarkers();
                visual_tools.publishTrajectoryLine(my_plan.trajectory_, joint_model_group);
                visual_tools.trigger();
            }       
        }
        ros::Duration(0.1).sleep();
    }
}

bool vr2ur5::canInterpolate(){

    std::vector<double> currentJointValues = move_group.getCurrentJointValues();
    for(size_t i = 0; i < currentJointValues.size(); ++i){
        double diff = std::fabs(currentJointValues[i] - currentGoalValues[i]);
        if(diff > 0.2)
            return 0;
    }
    
    move_group.setJointValueTarget(currentGoalValues);    
    moveit::core::RobotState goalState = move_group.getJointValueTarget();
    if(lockedPlanningScene->isStateColliding(goalState, joint_model_group->getName()))
        return false;
    return true;
}

vr2ur5::~vr2ur5(){
    std::cout << "Einmal muss jeder gehn auch wenn dein Herz zerbricht; Dabei wird die Welt nicht untergehn, Mensch ärger dich nicht!"<< std::endl;
}


int main(int argc, char** argv) {
    
    ros::init(argc, argv, "kevin_test");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(1);
    spinner.start();
    
    planning_scene_monitor::PlanningSceneMonitorPtr psm = std::make_shared<planning_scene_monitor::PlanningSceneMonitor>("/robot_description");
    
    vr2ur5 kevinTest(&nh, psm);

    return 0;
}