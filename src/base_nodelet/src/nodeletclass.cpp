#include "nodeletclass.h"
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseStamped.h>
#include <ros/ros.h>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <pluginlib/class_list_macros.h>
#include <std_msgs/Int32.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <pluginlib/class_list_macros.h>
#include <tf/transform_datatypes.h>
#include "car_status.h"
#include <sensor_msgs/NavSatFix.h>

using namespace std;
using namespace Eigen;
using namespace tf;
using namespace base_nodelet;
namespace base_nodelet{
    
    void TopicNodelet::onInit() {
        ros::NodeHandle& nh = getNodeHandle();
        sub = nh.subscribe("tf", 10, &TopicNodelet::inputCallback, this);//视觉
        sub1 = nh.subscribe("cartx_data", 10, &TopicNodelet::inputCallback1, this);//地面RTK
        sub2 = nh.subscribe("/mavros/local_position/odom", 10, &TopicNodelet::inputCallback2, this);//RTK 
      //  sub3 = nh.subscribe("tag_detections", 10, &TopicNodelet::inputCallback3, this);//其他
       
        sub3 = nh.subscribe("mavros/global_position/raw/fix",10, &TopicNodelet::inputCallback22, this);              //hh
        pub = nh.advertise<nav_msgs::Odometry>("output_topic", 10);
        odom_timer = nh.createTimer(ros::Duration(0.01), &TopicNodelet::publishMessage, this);
        ROS_INFO("Nodelet is OK for test"); 
        
    }

   void TopicNodelet::inputCallback(const geometry_msgs::TransformStamped &transform) { //视觉数据
       odom_.header = transform.header;
       vision_position.x() = transform.transform.translation.x;
       vision_position.y() = transform.transform.translation.y;
       vision_position.z() = transform.transform.translation.z;
       vision_ori.w() = transform.transform.rotation.w;
       vision_ori.x() = transform.transform.rotation.x;
       vision_ori.y() = transform.transform.rotation.y;
       vision_ori.z() = transform.transform.rotation.z;
       received_tf_ = true;
       
    }
        
    void TopicNodelet::inputCallback1(const car::car_status& mavlink_car_status) {    //小车的东北天数据

        received_odom1_ = true;
        // victory_car = Eigen::Quaterniond(0,mavlink_car_status.vx,mavlink_car_status.vy,mavlink_car_status.vz); //小车的东北天速度
        odom1_.header.stamp = ros::Time().fromNSec(mavlink_car_status.time);
        
        quaternion1 = AngleAxisd(mavlink_car_status.roll, Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(mavlink_car_status.pitch, Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(mavlink_car_status.yaw,     Eigen::Vector3d::UnitZ());
        
        if (mavlink_car_status.status == 1  ){                                            //获取local_car数据
            position_x_car = mavlink_car_status.px;
            position_y_car = mavlink_car_status.py;
            position_z_car = mavlink_car_status.pz;
            // posit_car = true;
           }

        if (mavlink_car_status.status == 11){                                            //获取小车经纬度信息
            if (!pre2){
                latitude_car = mavlink_car_status.px;
                longitude_car = mavlink_car_status.py;
                altitude_car = mavlink_car_status.pz;
                pre2 = true;  
                
                                                                  //信号2 是否有小车的经纬度
            }
        
        // odom1_.pose.pose.orientation.x = quaternion1.x();
        // odom1_.pose.pose.orientation.y = quaternion1.y();
        // odom1_.pose.pose.orientation.z = quaternion1.z();
        // odom1_.pose.pose.orientation.w = quaternion1.w();
      //  car_linear_ENU_x = quaternion1.inverse() * mavlink_car_status.vx;
      //  car_linear_ENU_y = quaternion1.inverse() * mavlink_car_status.vy;
      //  car_linear_ENU_z = quaternion1.inverse() * mavlink_car_status.vz;

        
        }
    } 
        
     void TopicNodelet::inputCallback22(const sensor_msgs::NavSatFix &odom) {      //飞机的经纬度
           
           // if (STATUS_FIX)                                                      //调用固定解和卫星类型数据
            if (!pre3){
                latitude_uav = odom.latitude;
                longitude_uav = odom.longitude;
                altitude_uav  = odom.altitude;
                pre3 = true;
            }

          
            if  (pre2 && pre1 && !bias_ok){
                geodeticToENU(latitude_car, longitude_car, altitude_car, latitude_uav , longitude_uav , altitude_uav , bias_x, bias_y, bias_z); 
                   //小车的位置信息，东北天下的delta_p
                bias_ok = true;
                cout << "bias_y"<<bias_y <<endl;
                
            }
        }
        
  //   geometry_msgs::Quaternion ENU_local1( Eigen::Quaterniond& pre_odom1_orientation,  Eigen::Quaterniond& quaternion1){  //计算东北天到飞机local坐标系的转角
  //      geometry_msgs::Quaternion result;
  //      Eigen::Quaterniond ori = pre_odom1_orientation.inverse() * quaternion1;
  //      result.w = ori.w();
  //      result.x = ori.x();
  //      result.y = ori.y();
  //      result.z = ori.z();
  //      cout << "转角" <<endl;
  //      std::cout << result.w << std::endl;
   //     return result;
       
  //   }
// 对数据做初始化，消息名称不统一，没有跳到回调里，看一下方针名称和消息格式，自定义消息格式。
 //飞机的东北天定位数据
    void TopicNodelet::inputCallback2(const nav_msgs::Odometry &odom) {       
        odom2_.header = odom.header;
        // nav_msgs::Odometry  odom2_;
        received_odom2_ = true;
        if (!pro){
            first_position_x = odom.pose.pose.position.x;
            first_position_y = odom.pose.pose.position.y;
            first_position_z = odom.pose.pose.position.z;
            pro = true;
        }      
        // odom2_.header = odom.header;
        // odom2_.pose.pose.position.x = odom.pose.pose.position.x;
        // odom2_.pose.pose.position.y = odom.pose.pose.position.y;
        // odom2_.pose.pose.position.z = odom.pose.pose.position.z;
        // odom2_.pose.pose.orientation = odom.pose.pose.orientation; 

        // quaternion2 = Eigen::Quaterniond(odom2_.pose.pose.orientation.w,odom2_.pose.pose.orientation.x,odom2_.pose.pose.orientation.y,odom2_.pose.pose.orientation.z);
        quaternion2.w() = odom.pose.pose.orientation.w;
        quaternion2.x() = odom.pose.pose.orientation.x;
        quaternion2.y() = odom.pose.pose.orientation.y;
        quaternion2.z() = odom.pose.pose.orientation.z;
        
        // odom2_.twist.twist.linear.x = odom.twist.twist.linear.x;
        // odom2_.twist.twist.linear.y = odom.twist.twist.linear.y;
        // odom2_.twist.twist.linear.z = odom.twist.twist.linear.z;
        position_x_uav = odom.pose.pose.position.x - first_position_x;
        position_y_uav = odom.pose.pose.position.y - first_position_y;
        position_z_uav = odom.pose.pose.position.z - first_position_z;


       // uav_linear_ENU_x = quaternion2.inverse() * odom.twist.twist.linear.x;
       // uav_linear_ENU_y = quaternion2.inverse() * odom.twist.twist.linear.y;
       // uav_linear_ENU_z = quaternion2.inverse() * odom.twist.twist.linear.z;


        // victory2 = Eigen::Quaterniond(0,odom.twist.twist.linear.x,odom.twist.twist.linear.y,odom.twist.twist.linear.z);
        // odom2_.twist.twist.angular.x = odom.twist.twist.angular.x;
        // odom2_.twist.twist.angular.y = odom.twist.twist.angular.y;
        // odom2_.twist.twist.angular.z = odom.twist.twist.angular.z;

        if (!pre1){
            pre_odom1_orientation = quaternion2;                                     //简易版的姿态角归零程序
            pre1 = true;  
                                                                    //信号1 是否有飞机初始的姿态信息     
        }
         
    }       

//经纬度计算部分




     void TopicNodelet::publishMessage(const ros::TimerEvent& time_event){                //发布话题函数
            if (bias_ok && received_odom1_){
              
            //   trust_position_x = position_x + bias_x; 
            //   trust_position_y = position_y + bias_y; 
            //   trust_position_z = position_z + bias_z; 
              vec = Eigen::Vector3d(bias_x,bias_y,bias_z);
            //   trust_position = Eigen::Quaterniond(0,trust_position_x,trust_position_y,trust_position_z);
              
              cout << bias_x<< "bias_x" <<endl;
              }
            if (pre1){
                
                Eigen::Quaterniond ori = pre_odom1_orientation.inverse() * quaternion1;
              //  geometry_msgs::Quaternion odom_orientation = ENU_local1( pre_odom1_orientation,quaternion1);
                odom1_.pose.pose.orientation.w = ori.w();  
                odom1_.pose.pose.orientation.x = ori.x();
                odom1_.pose.pose.orientation.y = ori.y();
                odom1_.pose.pose.orientation.z = ori.z();      //小车的姿态转到飞机local
               // odom2_.pose.pose.orientation = ENU_local1(quaternion2, pre_odom1_orientation);        //飞机转到飞机的local
                //odom2_.twist.twist.linear.x = ENU_local1(victory2, pre_odom1_orientation).x;          //飞机速度
               // odom2_.twist.twist.linear.y = ENU_local1(victory2, pre_odom1_orientation).y; 
                //odom2_.twist.twist.linear.z = ENU_local1(victory2, pre_odom1_orientation).z; 
               // odom1_.twist.twist.linear.x = ENU_local1(victory_car, pre_odom1_orientation).x;          //小车速度
              //  odom1_.twist.twist.linear.y = ENU_local1(victory_car, pre_odom1_orientation).y; 
               // odom1_.twist.twist.linear.z = ENU_local1(victory_car, pre_odom1_orientation).z;
               // geometry_msgs::Quaternion odom_position = ENU_local1(pre_odom1_orientation,trust_position);
              //  odom1_.pose.pose.position.x =  odom_position.x;
              //  odom1_.pose.pose.position.y =  odom_position.y;
               // odom1_.pose.pose.position.z =  odom_position.z;
                Eigen::Vector3d car_z(position_x_car ,position_y_car,position_z_car);
                Eigen::Vector3d linear(car_linear_ENU_x,car_linear_ENU_y,car_linear_ENU_z);
                vec_bias = pre_odom1_orientation.inverse() * vec;
                position_local = pre_odom1_orientation.inverse() *quaternion1.inverse() * car_z;
                // linear_local = pre_odom1_orientation * linear; // ？
                odom1_.pose.pose.position.x = position_local.x() + vec_bias.x();
                odom1_.pose.pose.position.y = position_local.y() + vec_bias.y();
                odom1_.pose.pose.position.z = position_local.z() + vec_bias.z();
                odom1_.twist.twist.linear.x = linear_local.x();
                odom1_.twist.twist.linear.y = linear_local.y();
                odom1_.twist.twist.linear.z = linear_local.z();
                
                if ((odom_.header.stamp-odom2_.header.stamp).toNSec<0.01){                                                   //
                    vision_pose_mid = vision_ori_cal.inverse() * vision_position + vision_pose_cal;               //
                    vision_ori_mid = vision_ori_cal.inverse() * vision_ori;                                                                    //
                    vision_pose = quaternion2.inverse() *vision_pose_mid + pose_uav;                                       //
                    vision_ori = ori_uav_ori.inverse() * vision_or_midi;                                                                         //
                }else{
                    delta_proportion = (odom2_.header.stamp - last_time).toNSec/(odom_.header.stamp -last_time  ).toNSec;
                    vision_position_L = (vision_position - last_vision_position) * delta_proportion + last_vision_position;
                    vision_delta_ori = vision_ori * last_vision_ori.inverse();
                    vision_ori_L = vision_delta_ori.log() * delta_proportion * last_vision_ori ;

                    vision_pose_mid = vision_ori_cal.inverse() * vision_position_L + vision_pose_cal;               //
                    vision_ori_mid = vision_ori_cal.inverse() * vision_ori_L;                                                                    //
                    vision_pose = quaternion2.inverse() *vision_pose_mid + pose_uav;                                       //
                    vision_ori = ori_uav_ori.inverse() * vision_or_midi;                                                                         //
                }
                last_time = odom_.header.stamp;
                last_vision_position = vision_position;
                last_vision_ori = vision_ori; 
                odom_.pose.pose.position.z = vision_pose.z.();
                odom_.pose.pose.orientation.w = vision_ori.w();
                odom_.pose.pose.orientation.x = vision_ori.x();
                odom_.pose.pose.orientation.y = vision_ori.y();
                odom_.pose.pose.orientation.z = vision_ori.z();
                pub.publish(odom_);                                                                                                                                           //
                pub.publish(odom1_);
            }

         if (!received_tf_){
             cout<<"没有视觉数据"<<endl;
          //   refusion(启动预测函数)
         }
         if (!received_odom1_){
             cout<<"没有地面数据"<<endl;
          //   refusion(启动通讯函数)
         }
         if (!received_odom2_){
             cout<<"没有飞机数据"<<endl;
          //   refusion(启动避障函数)
         }
    }



inline double deg2rad(double degrees) {
    return degrees * M_PI / 180.0;
}
// 弧度转度
inline double rad2deg(double radians) {
    return radians * 180.0 / M_PI;
}

void geodeticToECEF(double lat, double lon, double alt, 
                    double &x, double &y, double &z) {
    double latRad = deg2rad(lat);
    double lonRad = deg2rad(lon);

    double sinLat = sin(latRad);
    double cosLat = cos(latRad);
    double sinLon = sin(lonRad);
    double cosLon = cos(lonRad);

    double N = A / sqrt(1 - F * (2 - F) * sinLat * sinLat);

    x = (N + alt) * cosLat * cosLon;
    y = (N + alt) * cosLat * sinLon;
    z = (N * (1 - F) * (1 - F) + alt) * sinLat;
}

void ecefToNED(double x, double y, double z, 
               double refLat, double refLon, double refAlt,
               double &north, double &east, double &down) {
    double refX, refY, refZ;
    geodeticToECEF(refLat, refLon, refAlt, refX, refY, refZ);

    double latRad = deg2rad(refLat);
    double lonRad = deg2rad(refLon);

    double sinLat = sin(latRad);
    double cosLat = cos(latRad);
    double sinLon = sin(lonRad);
    double cosLon = cos(lonRad);

    double dx = x - refX;
    double dy = y - refY;
    double dz = z - refZ;

    north = -sinLat * cosLon * dx - sinLat * sinLon * dy + cosLat * dz;
    east  = -sinLon * dx + cosLon * dy;
    down  = -cosLat * cosLon * dx - cosLat * sinLon * dy - sinLat * dz;
}

void convertGeodeticToNED(double lat, double lon, double alt, 
                          double refLat, double refLon, double refAlt,
                          double &north, double &east, double &down) {
    double x, y, z;
    geodeticToECEF(lat, lon, alt, x, y, z);
    ecefToNED(x, y, z, refLat, refLon, refAlt, north, east, down);
}

void convertGeodeticToNED(double lat, double lon, double alt, 
                          double refLat, double refLon, double refAlt,
                          double &north, double &east, double &down) {
    double x, y, z;
    geodeticToECEF(lat, lon, alt, x, y, z);
    ecefToNED(x, y, z, refLat, refLon, refAlt, north, east, down);

}
}
//nodelet的本质是把节点作为插件来调用，因此需要PLUGINLIB的宏定义、
//第一个参数是类名，第二个参数是父类
PLUGINLIB_EXPORT_CLASS(base_nodelet::TopicNodelet, nodelet::Nodelet);

