#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Imu.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Quaternion.h>
#include <Eigen/Dense>
#include <iostream>

using namespace Eigen;

class KalmanFilter
{
public:
    KalmanFilter();
    void filter(const ros::Time& stamp, const Vector3d& pos, const Quaterniond& q, const Vector3d& imu);

private:
    ros::Time last_stamp_;
    Vector3d last_pos_;
    Quaterniond last_q_;
    Vector3d last_imu_;
    Matrix<double, 9, 9> P_;
    VectorXd X_;
    Matrix<double, 9, 9> Q_;
    Matrix<double, 3, 3> R_;
    Matrix<double, 9, 3> G_;

    void predict(const double dt);
    void update(const Vector3d& z, const double dt);
};
 
KalmanFilter::KalmanFilter()
{
    // 初始化 P 矩阵
    P_ = Matrix<double, 9, 9>::Identity() * 0.1;

    // 初始化 X 向量
    X_.setZero();

    // 初始化 Q 矩阵，表示过程噪声的方差
    Q_ << Matrix3d::Identity() * 0.01, Matrix3d::Zero(), Matrix3d::Zero(),
          Matrix3d::Zero(), Matrix3d::Identity() * 0.01, Matrix3d::Zero(),
          Matrix3d::Zero(), Matrix3d::Zero(), Matrix3d::Identity() * 0.01;

    // 初始化 R 矩阵，表示观测噪声的方差
    R_ << Matrix3d::Identity() * 0.1;

    // 初始化 G 矩阵，表示输入向量对状态向量的影响
    G_ << Matrix3d::Identity(), Matrix3d::Zero(), Matrix3d::Zero(),
          Matrix3d::Zero(), Matrix3d::Identity(), Matrix3d::Zero(),
          Matrix3d::Zero(), Matrix3d::Zero(), Matrix3d::Identity();
}

void KalmanFilter::predict(const double dt)
{
    // 预测状态向量 X
    X_ = X_ + (G_ * last_imu_ + VectorXd::Zero(9)) * dt;

    // 更新 P 矩阵
    P_ = P_ + Q_ * dt;
}

void KalmanFilter::update(const Vector3d& z, const double dt)
{
    // 计算观测残差
    Vector3d y = z - last_pos_;

    // 计算卡尔曼增益
    Matrix<double, 9, 3> K = P_ * G_.transpose() * (G_ * P_ * G_.transpose() + R_).inverse();

    // 更新状态向量 X
    X_ = X_ + K * y;

    // 更新 P 矩阵
    P_ = (Matrix<double, 9, 9>::Identity() - K * G_) * P_;

    // 更新时间戳、位置和姿态信息
    last_stamp_ = last_stamp_ + ros::Duration(dt);
    last_pos_ = X_.block<3,1>(0,0);
    last_q_ = Quaterniond(X_.block<3,3>(3,0));
}

void KalmanFilter::filter(const ros::Time& stamp, const Vector3d& pos, const Quaterniond& q, const Vector3d& imu)
{
    if (last_stamp_.isZero())
    {
        // 如果是第一次接收到位置信息，不进行预测和更新
        last_stamp_ = stamp;
        last_pos_ = pos;
        last_q_ = q;
        last_imu_ = imu;
        return;
    }

    // 计算时间差
    double dt = (stamp - last_stamp_).toSec();

    // 运动模型预测
    predict(dt);

    // 观测模型更新
    update(pos, dt);

    // 保存最新的状态量
    last_stamp_ = stamp;
    last_pos_ = pos;
    last_q_ = q;
    last_imu_ = imu;
}

class KalmanFilterNode
{
public:
    KalmanFilterNode();
    void odomCallback(const nav_msgs::Odometry::ConstPtr& msg);
    void imuCallback(const sensor_msgs::Imu::ConstPtr& msg);

private:
    ros::NodeHandle nh_;
    ros::Subscriber odom_sub_;
    ros::Subscriber imu_sub_;
    ros::Publisher filtered_pub_;

    KalmanFilter filter_;

    double last_imu_time_;
};

KalmanFilterNode::KalmanFilterNode()
{
    odom_sub_ = nh_.subscribe("odom", 10, &KalmanFilterNode::odomCallback, this);
    imu_sub_ = nh_.subscribe("imu", 10, &KalmanFilterNode::imuCallback, this);
    filtered_pub_ = nh_.advertise<nav_msgs::Odometry>("filtered_odom", 10);

    last_imu_time_ = 0;
}

void KalmanFilterNode::odomCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    // 获取位置和姿态
    Vector3d pos(msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z);
    Quaterniond q(msg->pose.pose.orientation.w, msg->pose.pose.orientation.x, msg->pose.pose.orientation.y, msg->pose.pose.orientation.z);

    // 获取时间戳
    ros::Time stamp = msg->header.stamp;

    // 获取 IMU 信息
    Vector3d imu(filter_.X_.block<3,1>(6,0));

    // 对齐时间戳
    double dt = (last_imu_time_ > 0) ? (stamp.toSec() - last_imu_time_) : 0;
    if (dt > 0)
    {
        filter_.filter(stamp, pos, q, imu);
    }

    // 构造滤波后的 odometry 消息
    nav_msgs::Odometry filtered_msg;
    filtered_msg.header.stamp = stamp;
    filtered_msg.header.frame_id = "world";
    filtered_msg.pose.pose.position.x = filter_.X_(0);
    filtered_msg.pose.pose.position.y = filter_.X_(1);
    filtered_msg.pose.pose.position.z = filter_.X_(2);
    filtered_msg.pose.pose.orientation.w = filter_.X_(3);
    filtered_msg.pose.pose.orientation.x = filter_.X_(4);
    filtered_msg.pose.pose.orientation.y = filter_.X_(5);
    filtered_msg.pose.pose.orientation.z = filter_.X_(6);
    filtered_msg.twist.twist.linear.x = filter_.X_(7);
    filtered_msg.twist.twist.linear.y = filter_.X_(8);
    filtered_msg.twist.twist.linear.z = filter_.X_(9);

    // 发布滤波后的 odometry 消息
    filtered_pub_.publish(filtered_msg);
}

void KalmanFilterNode::imuCallback(const sensor_msgs::Imu::ConstPtr& msg)
{
    // 获取时间戳
    double timestamp = msg->header.stamp.toSec();
    if (last_imu_time_ == 0)
    {
        last_imu_time_ = timestamp;
        return;
    }

    // 计算时间差
    double dt = timestamp - last_imu_time_;
    last_imu_time_ = timestamp;

    // 获取 IMU 信息
    Vector3d imu(msg->linear_acceleration.x, msg->linear_acceleration.y, msg->linear_acceleration.z);

    // 进行状态预测和更新
    filter_.predict(dt);
    filter_.update(filter_.last_pos_, dt);
    filter_.filter(ros::Time::now(), filter_.last_pos_, filter_.last_q_, imu);
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "kalman_filter_node");

    KalmanFilterNode node;

    ros::spin();

    return 0;
}
