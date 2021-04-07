#include "first_challenge_katayama/first_challenge_katayama.h"
#include <tf/transform_datatypes.h>
#include <stdio.h>

RoombaController::RoombaController():private_nh("~")
{
    private_nh.param("hz",hz,{10});
    private_nh.param("stage",stage,{0});
//    private_nh.param("data_output",data_output,{1080});

    sub_pose = nh.subscribe("/roomba/odometry",10,&RoombaController::pose_callback,this);
//    sub_scan = nh.subscribe("scan",10,$RoombaController::pose_callback_lider,this);

    pub_cmd_vel = nh.advertise<roomba_500driver_meiji::RoombaCtrl>("roomba/control",1);
}

void RoombaController::pose_callback(const nav_msgs::Odometry::ConstPtr &msg){
    current_pose = *msg;
}
/*
void RoombaController::pose_callback_lider(const sensor_msgs::LaserScan::ConstPtr &msg){
    laserscan = *msg;
}
*/
void RoombaController::go_straight()
{
    cmd_vel.mode = 11;
    if(stage <= 1){
        if(stage == 0){
            start_position_x = current_pose.pose.pose.position.x;
            start_position_y = current_pose.pose.pose.position.y;
            stage = 1;
        }
        cmd_vel.cntl.linear.x=0.5;
        distance = sqrt((current_pose.pose.pose.position.x - start_position_x)*(current_pose.pose.pose.position.x - start_position_x) + (current_pose.pose.pose.position.y - start_position_y)*(current_pose.pose.pose.position.y - start_position_y));
        if(distance >= 1){
        cmd_vel.cntl.linear.x = 0.0;
        stage = 2;
        }
    pub_cmd_vel.publish(cmd_vel);
    }
}

void RoombaController::one_rotate(){
    tf::Quaternion q(current_pose.pose.pose.orientation.x, current_pose.pose.pose.orientation.y, current_pose.pose.pose.orientation.z, current_pose.pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    if(stage == 2){
        cmd_vel.cntl.angular.z = 0.5;
        angle_integral = angle_integral + yaw;
        if(angle_integral <= 0.0){
            cmd_vel.cntl.angular.z = 0.0;
            stage = 3;
        }
    pub_cmd_vel.publish(cmd_vel);
    }
}
/*
void RoombaController::approach_wall(){
    if(stage = 3){
        cmd_vel.cntl.linear.x = 0.5;

        ROS_INFO_STREAM(laserscan.ranges[data_output]);
        if(laserscan.ranges[data_output] <= 0.5){
            cmd_vel.cntl.linear.x = 0;
        }
    }
}
*/
void RoombaController::process(){
    ros::Rate loop_rate(hz);

    while(ros::ok())
    {
    go_straight();
    one_rotate();
//   approach_wall();
    ros::spinOnce();
    loop_rate.sleep();
    }
}

int main(int argc,char** argv){
    ros::init(argc,argv, "first_challenge_katayama");
    RoombaController roomba_controller;
    roomba_controller.process();
    return 0;
}
