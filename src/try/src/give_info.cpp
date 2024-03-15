#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <iostream>
#include <tf/tf.h>
#include "car_status.h"
#include <sensor_msgs/NavSatFix.h>
using namespace ros;

using namespace std;
float tt=0.0;
float qq=0.0;
float ttt=0.0;
int main(int argc, char** argv)
{
    // 初始化ROS节点
    ros::init(argc, argv, "mavlink");

    // 创建节点句柄
    ros::NodeHandle nh;

      ros::Time last_time1 = ros::Time::now();
      ros::Time last_time2 = ros::Time::now();
      ros::Time last_time3 = ros::Time::now();
    // 创建一个Publisher对象，发布名为"topic_name"的话题，消息类型为std_msgs::String
    ros::Publisher pub = nh.advertise<sensor_msgs::NavSatFix>("mavros_global_position_raw_fix", 10);
    ros::Publisher pub_tf = nh.advertise<geometry_msgs::TransformStamped>("tf", 10);
    ros::Publisher pub_carTx = nh.advertise<car::car_status>("carTx", 30);
    ros::Publisher pub_local = nh.advertise<nav_msgs::Odometry>("mavros/local", 20);
    // 设置发布频率
    ros::Rate rate(40);
    // 循环发布消息

    cout << "jiedian2"<<endl;
    while (ros::ok()) {
        ros::Duration elapsed_time1 = ros::Time::now() - last_time1;
        ros::Duration elapsed_time2 = ros::Time::now() - last_time2;
        ros::Duration elapsed_time3 = ros::Time::now() - last_time3;
      if (elapsed_time1.toSec() > 0.025) {
        // 创建一个nav_msgs::Odometry类型的消息对象，并填充随机数据
        car::car_status msg;
        ros::Time current_time1 = ros::Time::now();
        msg.time = current_time1.toNSec();
        //msg.header.frame_id = "carTx";
        //msg.child_frame_id = "base_link";
        qq+=0.025;
        
            if (fabs(qq - round(qq)) < 0.0001){
            //msg.pose.pose.position.x = rand() / double(RAND_MAX);
            //msg.pose.pose.position.y = rand() / double(RAND_MAX);
            //msg.pose.pose.position.z = rand() / double(RAND_MAX);
            //msg.pose.pose.orientation = tf::createQuaternionMsgFromYaw(rand() / double(RAND_MAX));
            //msg.twist.twist.linear.x = rand() / double(RAND_MAX);
           // msg.twist.twist.linear.y = rand() / double(RAND_MAX);
            //msg.twist.twist.linear.z = rand() / double(RAND_MAX);
            //msg.twist.twist.angular.x = rand() / double(RAND_MAX);
            //msg.twist.twist.angular.y = rand() / double(RAND_MAX);
            //msg.twist.twist.angular.z = rand() / double(RAND_MAX);
              cout <<qq<<endl;
              msg.status = 11;
              msg.px = 30.754814749;
              msg.py = 103.92316411;
              msg.pz = 0;
            }else {
         
              msg.status = 1;
              msg.px = qq;
              msg.py = -qq;
              msg.pz = 0;
            } 
          msg.roll = 0.0;
          msg.pitch = 0.0;
          msg.yaw = 0.785398;
          
       
        pub_carTx.publish(msg);                                                //发布小车
        last_time1 = current_time1;
      }
      if (elapsed_time2.toSec() > 0.05) {
        nav_msgs::Odometry msg2;
        msg2.header.stamp = ros::Time::now();
        ros::Time current_time2 = ros::Time::now();
        msg2.header.frame_id = "mavros/local";
        msg2.child_frame_id = "base_link";
        tt+=0.1;
            
        //msg2.pose.pose.position.x = rand() / double(RAND_MAX);
        //msg2.pose.pose.position.y = rand() / double(RAND_MAX);
        //msg2.pose.pose.position.z = rand() / double(RAND_MAX);
        //msg2.pose.pose.orientation = tf::createQuaternionMsgFromYaw(rand() / double(RAND_MAX));
        //msg2.twist.twist.linear.x = rand() / double(RAND_MAX);
       // msg2.twist.twist.linear.y = rand() / double(RAND_MAX);
        //msg2.twist.twist.linear.z = rand() / double(RAND_MAX);
        //msg2.twist.twist.angular.x = rand() / double(RAND_MAX);
       // msg2.twist.twist.angular.y = rand() / double(RAND_MAX);
       // msg2.twist.twist.angular.z = rand() / double(RAND_MAX);
        
          msg2.pose.pose.position.x = -tt;
          msg2.pose.pose.position.y =  tt;
          msg2.pose.pose.position.z = 0;
          msg2.pose.pose.orientation.w = 0.92388;
          msg2.pose.pose.orientation.x = 0;
          msg2.pose.pose.orientation.y = 0;
          msg2.pose.pose.orientation.z = 0.382683;
        
        pub_local.publish(msg2);                                              //发布飞机
        last_time2 = current_time2;
       }
     if (elapsed_time3.toSec() > 0.1){
        // geometry_msgs::TransformStamped msg1;
        sensor_msgs::NavSatFix msg1;
        ros::Time current_time3 = ros::Time::now();
        msg1.header.stamp = current_time3;
            
        //msg1.header.frame_id = "tf";
        //msg1.child_frame_id = "robot";
        //msg1.transform.translation.x = rand() / double(RAND_MAX);
        //msg1.transform.translation.y = rand() / double(RAND_MAX);
        //msg1.transform.translation.z = rand() / double(RAND_MAX);
        //tf2::Quaternion q;
        //q.setRPY(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX));
        //msg1.transform.rotation.x = q.getX();
        //msg1.transform.rotation.y = q.getY();
        //msg1.transform.rotation.z = q.getZ();
       // msg1.transform.rotation.w = q.getW();
       // msg1.latitude = -ttt*10+30.754814793;
       // msg1.longitude =  ttt*10+103.92316455;
        msg1.latitude = 30.754814793;
        msg1.longitude = 103.92316455;
        ttt+=0.2;
        msg1.altitude = 0;
        pub.publish(msg1);                                                    //飞机经纬度
     

        last_time3 = current_time3;
       }
      rate.sleep();
    }

    return 0;
}
