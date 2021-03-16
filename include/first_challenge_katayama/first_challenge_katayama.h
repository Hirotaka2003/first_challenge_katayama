#ifndef FIRST_CHALLENGE_KATAYAMA_H
#define FIRST_CHALLENGE_KATAYAMA_H

#include <ros/ros.h>
#include "roomba_500driver_meiji/RoombaCtrl.h"
#include "nav_msgs/Odometry.h"
//#include "sensor_msgs/LaserScan.h"


class RoombaController
{
    public:
        RoombaController();
        void process();
        void go_straight();
        void one_rotate();
        //void approach_wall();

    private:
        void pose_callback(const nav_msgs::Odometry::ConstPtr &);
        //void pose_callback_lider(const sensor_msgs::LaserScan::ConstPtr &);

        int hz;
        //int laser_rate;
        float start_position_x = 0.0;
        float start_position_y = 0.0;
        float distance = 0.0;
        float current_angular = 0.0;
        float sigma_angular = 0.0;

        ros::Publisher pub_cmd_vel;
        ros::Subscriber sub_scan;
        ros::Subscriber sub_pose;
        ros::NodeHandle nh;
        ros::NodeHandle private_nh;
        roomba_500driver_meiji::RoombaCtrl cmd_vel;
        nav_msgs::Odometry current_pose;
        nav_msgs::Odometry quaternion;
        //sensor_msgs::LaserScan laserscan;

};

#endif

