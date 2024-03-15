#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <std_msgs/Int32.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <Eigen/Dense>
#include <iostream>
int main(int argc, char** argv)
{
    ros::init(argc, argv, "quaternion_example");
    ros::NodeHandle nh;
    Eigen::Vector3d pose1(1,2,-3);
    Eigen::Vector3d pose2;
    Eigen::Quaterniond ori1;
    Eigen::Quaterniond ori2;
    Eigen::Quaterniond ori3;
    Eigen::Quaterniond ori4;
    
    ori1.w() =1;
    ori1.x() =0;
    ori1.y() =0;
    ori1.z() =0;
    ori2.x() = -ori1.x();
    ori2.y() = -ori1.y();
    ori2.z() = ori1.z();
    ori2.w() = -ori1.w();
    ori3.w() = 0.707;
    ori3.x() = 0;
    ori3.y() = 0;
    ori3.z() = 0.707;
    ori4 = ori3 * ori2;
    pose2 = ori3*pose1;
    std::cout <<"pose2" << pose2.x() <<pose2.y() << pose2.z() << std::endl;
    std::cout << "ori4" << ori4.w() << ori4.x() << ori4.y() << ori4.z() << std::endl; 
}

