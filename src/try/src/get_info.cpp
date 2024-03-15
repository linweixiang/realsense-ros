#include <ros/ros.h>
#include <iostream>
#include <nav_msgs/Odometry.h>
// 回调函数，用于接收并处理收到的消息
void topicCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    // 在此处进行消息处理逻辑
    // 可以访问 msg 中的数据，如 msg->data

    float x = msg->pose.pose.position.x;
    float y = msg->pose.pose.position.y;
    float z = msg->pose.pose.position.z;
    float qw = msg->pose.pose.orientation.w;
    float qx = msg->pose.pose.orientation.x;
    float qy = msg->pose.pose.orientation.y;
    float qz = msg->pose.pose.orientation.z;
    ROS_INFO("Received message: x=%f, y=%f, z=%f", x, y, z);

    nav_msgs::Odometry newMsg;
    newMsg.pose.pose.position.x = x;
    newMsg.pose.pose.position.y = y;
    newMsg.pose.pose.position.z = z;
    newMsg.pose.pose.orientation.w = qw;
    newMsg.pose.pose.orientation.x = qx;
    newMsg.pose.pose.orientation.y = qy;
    newMsg.pose.pose.orientation.z = qz;
    ros::NodeHandle nh;
    ros::Publisher processed_pub = nh.advertise<nav_msgs::Odometry>("mix_data", 10);
    processed_pub.publish(newMsg);

}



int main(int argc, char** argv)
{
    // 初始化 ROS 节点
    ros::init(argc, argv, "change");

    // 创建节点句柄
    ros::NodeHandle nh;

    // 创建一个 Subscriber 对象，订阅名为 "topic_name" 的话题，指定回调函数为 topicCallback
    ros::Subscriber sub = nh.subscribe("mavlink", 10, topicCallback);

    // 创建发布者
    // ros::Publisher chatter_pub = nh.advertise<std_msgs::String>("chatter", 10);
    ros::Publisher processed_pub = nh.advertise<nav_msgs::Odometry>("mix_data", 10);
    // 进入 ROS 主循环
    ros::spin();

    return 0;

}
