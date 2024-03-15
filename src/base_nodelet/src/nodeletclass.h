#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Int32.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <pluginlib/class_list_macros.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <pluginlib/class_list_macros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <pluginlib/class_list_macros.h>
#include <tf/transform_datatypes.h>
#include "car_status.h"
#include <sensor_msgs/NavSatFix.h>
using namespace std;
using namespace Eigen;
using namespace tf;
namespace base_nodelet {
    class TopicNodelet : public nodelet::Nodelet {
    public:

        double dLat ;
        double dLon ;
        double dAlt ;
        
    // 计算参考点处的子午线曲率半径
        double N ;
        virtual void onInit();
        Eigen::Quaterniond vision_ori_cal =( 0.013247752305396116 ,-0.04044915173437392, -0.998541730296118 ,-0.033208071908428685);
        Eigen::Vector3d vision_pose_mid = ( -0.0683589 , 0.01509123 ,-0.14474158);
        void inputCallback(const geometry_msgs::TransformStamped &transform);
        void inputCallback1(const car::car_status& mavlink_car_status );
        void inputCallback2(const nav_msgs::Odometry& odom);
  
        void publishMessage(const ros::TimerEvent& time_event);
        void inputCallback22(const sensor_msgs::NavSatFix &odom);
        void geodeticToENU(double lat, double lon, double alt, double refLat, double refLon, double refAlt,
                       double& east, double& north, double& up);
        geometry_msgs::Quaternion ENU_local1( Eigen::Quaterniond& pre_odom1_orientation, Eigen::Quaterniond& quaternion1);
        double toRad(double degree);
       // void inputCallback3(const std_msgs::Int32::ConstPtr& msg);
    private:
        bool received_tf_ = false;
        bool received_odom1_ = false;
        bool received_odom2_ = false;
        bool posit_car = false;
        bool pro = false;
        double vision_x;
        double vision_y;
        double vision_z;
        double vision_qx;
        double vision_qw;
        double vision_qy;
        double vision_qz;
        double position_x;
        double position_y;
        double position_z;
        double position_x_car;
        double position_y_car;
        double position_z_car;
        double position_x_uav;
        double position_y_uav;
        double position_z_uav;
        double car_linear_ENU_x;
        double car_linear_ENU_y;
        double car_linear_ENU_z;
        double uav_linear_ENU_x;
        double uav_linear_ENU_y;
        double uav_linear_ENU_z;
        double first_position_x;
        double first_position_y;
        double  first_position_z;


        long latitude_uav  ;
        long longitude_uav;
        long altitude_uav;
        long latitude_car ;
        long longitude_car;
        long altitude_car;
        bool pre1 = false;
        bool pre2 = false;
        bool pre3 = false;
        bool bias_ok = false;
        ros::Timer odom_timer;
        double bias_x, bias_y, bias_z;
        nav_msgs::Odometry odom_;
        nav_msgs::Odometry odom1_;
        nav_msgs::Odometry odom2_;
        nav_msgs::Odometry odom_diff;
        ros::Subscriber sub_odom1_;
        ros::Subscriber sub_odom2_;
        ros::Publisher pub_diff_;


        Eigen::Vector3d vision_position;

        Eigen::Vector3d car_z;
        Eigen::Vector3d local_car_z;
        Eigen::Quaterniond vision_ori;
        Eigen::Quaterniond quaternion1;
        Eigen::Quaterniond quaternion2;
        Eigen::Quaterniond victory_car;
        Eigen::Quaterniond victory2;
        Eigen::Quaterniond pre_odom1_orientation;
        Eigen::Quaterniond trust_position;
        Eigen::Vector3d vec;
        Eigen::Vector3d vec_bias;
        Eigen::Vector3d linear_local;
        Eigen::Vector3d position_local;
        Eigen::Vector3d linear;
        geometry_msgs::TransformStamped tf_msg_;
        ros::NodeHandle nh;
        ros::Subscriber sub;
        ros::Subscriber sub1;
        ros::Subscriber sub2;
        ros::Subscriber sub3;
        ros::Publisher pub;
    };
}
