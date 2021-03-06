#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/robot_trajectory/robot_trajectory.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "trajectory_control", ros::init_options::AnonymousName);
    ros::AsyncSpinner spinner(1);
    spinner.start();
    static const std::string PLANNING_GROUP = "manipulator";
    moveit::planning_interface::MoveGroupInterface move_group_interface(PLANNING_GROUP);
    const moveit::core::JointModelGroup* joint_model_group = 
	    move_group_interface.getCurrentState()->getJointModelGroup(PLANNING_GROUP);
    move_group_interface.setPoseReferenceFrame("arm_base_link");
    move_group_interface.setGoalPositionTolerance(0.1);
    move_group_interface.setGoalOrientationTolerance(0.2);
/*
    geometry_msgs::Pose target_pose;
    target_pose.orientation.w = 1;
    target_pose.orientation.x = 0;
    target_pose.orientation.y = 0;
    target_pose.orientation.z = 0;

    target_pose.position.x = 0.009;
    target_pose.position.y = -0.03;
    target_pose.position.z = 0.646;

    std::vector<geometry_msgs::Pose> waypoints;
    move_group.setPoseTarget(target_pose);
*/
    std::vector<double> joint_group_positions;
    moveit::core::RobotStatePtr current_state = move_group_interface.getCurrentState();
    current_state->copyJointGroupPositions(joint_model_group, joint_group_positions);
    joint_group_positions[0] = -1.5707;
    move_group_interface.setJointValueTarget(joint_group_positions);
    moveit::planning_interface::MoveGroupInterface::Plan my_plan;
    moveit::planning_interface::MoveItErrorCode success = move_group_interface.plan(my_plan);
      
    ROS_INFO("Visualizing plan 3 (pose goal) %s",success?"":"FAILED");

    if(success)
        move_group_interface.execute(my_plan);

    ros::shutdown();

    return 0;

}
