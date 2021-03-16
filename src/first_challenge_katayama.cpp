#include "first_challenge_katayama/first_challenge_katayama.h"
#include <tf/transform_datatypes.h>
#include <stdio.h>

RoombaController::RoombaController():private_nh("~")
{
    private_nh.param("hz",hz,{10});
//    private_nh.param("lasar_rate",lasar_rate,{1080});
    sub_pose = nh.subscribe("/roomba/odometry",10,&RoombaController::pose_callback,this);

    pub_cmd_vel = nh.advertise<roomba_500driver_meiji::RoombaCtrl>("roomba/control",1);
}

void RoombaController::pose_callback(const nav_msgs::Odometry::ConstPtr &msg)
{
    current_pose = *msg;
}

void RoombaController::go_straight()
{
    if(start_position_x = 0){
        start_position_x = current_pose.pose.pose.position.x;
        start_position_y = current_pose.pose.pose.position.y;
    }
    cmd_vel.cntl.linear.x=0.5;
    while(distance <= 1){
        distance = sqrt((current_pose.pose.pose.position.x - start_position_x)*(current_pose.pose.pose.position.x - start_position_x) + (current_pose.pose.pose.position.y - start_position_y)*(current_pose.pose.pose.position.y - start_position_y));
    }
    cmd_vel.cntl.linear.x = 0.0;
    pub_cmd_vel.publish(cmd_vel);
}

void RoombaController::one_rotate(){
    tf::Quaternion q(current_pose.pose.pose.orientation.x, current_pose.pose.pose.orientation.y, current_pose.pose.pose.orientation.z, current_pose.pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    double roll, pitch, yaw;
    m.getRPY(roll, pitch, yaw);

    cmd_vel.cntl.angular.z = 0.5;
    while(current_angular<2*M_PI - 0.1){
        if(yaw<0){
            current_angular = 2*M_PI - yaw;
        }
        else{
            current_angular = yaw ;
    cmd_vel.cntl.angular.z = 0.0;
    }
//void RoombaController::approach_wall(){
//}
    }
}

void RoombaController::process(){
    cmd_vel.mode = 11;

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

