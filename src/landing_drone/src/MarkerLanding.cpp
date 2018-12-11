#include "ros/ros.h"
#include <iostream>
#include <cstdlib>
// TF
#include "tf/transform_listener.h"
// Ardrone control
#include "std_msgs/Empty.h"
#include "boost/thread.hpp"
#include "sensor_msgs/Range.h"
#include <ardrone_autonomy/Navdata.h>
#include <Eigen/Dense>
#include "KalmanFilter.h"

using namespace boost;
using namespace boost::this_thread;
using namespace Eigen;

int state = 0;
double altd = 0.0;
double r = 0.0;
double initTime = 0;
bool land = false;
double limit = 1.0;
double sum_error_x = 0.0;
double sum_error_y = 0.0;
double last_error_x = 0.0;
double last_error_y = 0.0;
double diff_x = 0.0;
double diff_y = 0.0;
double vel_x = 0.0;
double vel_y = 0.0;

void checkCin() {
    char follow_marker;
    std::cin >> follow_marker;
    if (follow_marker == 'l') {
        state = 2;
    }
    char land;
    std::cin >> land;
    if (land == 'l') {
        state = 3;
    }
}

void navdataCallback(const ardrone_autonomy::NavdataConstPtr nav) {
    vel_x = nav->vx * 0.001;
    vel_y = nav->vy * 0.001;
    altd = nav->altd * 0.001;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "MarkerLanding");
    ROS_INFO("MarkerLanding : Started");
    // Define node handler for subs
    tf::TransformListener listener;
    ros::NodeHandle n;
    initTime = (double)ros::Time::now().toSec();
    // Loop rate
    ros::Rate rate(100);

    // Thread that waits user input
    boost::thread t(&checkCin);

    // Twist msgs init
    geometry_msgs::Twist twist_old;
    geometry_msgs::Twist twist;

    twist.linear.x = 0.0;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;
    twist.angular.x = 0.0001;
    twist.angular.y = 0.0001;
    twist.angular.z = 0.0;
    
    int size_n = 4;
    int size_m = 2;
    double dt = 0.01;
    double state_x = 0.0, state_y = 0.0, current_x = 0.0, current_y = 0.0;
    
    MatrixXf A(size_n, size_n); // System dynamics matrix
    MatrixXf H(size_m, size_n); // Output matrix
    MatrixXf Q(size_n, size_n); // Process noise covariance
    MatrixXf R(size_m, size_m); // Measurement noise covariance
    MatrixXf P(size_n, size_n); // Estimate error covariance
    MatrixXf X(size_n, 1);
    
    A << 1, 0, dt, 0, 0, 1, 0, dt, 0, 0, 1, 0, 0, 0, 0, 1;
    H << 1, 0, 0, 0, 0, 1, 0, 0;
    R << 15, 0, 0, 15;
    P << 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1;
    Q << 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5, 0, 0, 0, 0, 5;
    X << 0, 0, 0, 0;
    
    KalmanFilter kf(size_n, size_m);
    kf.setFixed(A, H, Q, R);
    kf.setInitial(X, P);

    VectorXf measurement(size_m);

    ros::Subscriber navdata = n.subscribe("ardrone/navdata", 10,navdataCallback);

    while(ros::ok()) {
        tf::StampedTransform transform;
        transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
        try {
            listener.lookupTransform("/ardrone_base_bottomcam", "ar_marker_0", ros::Time(0), transform);
            printf("------- Frame Start -------\n");
            
            current_x = transform.getOrigin().x();
            current_y = transform.getOrigin().y();
            
            measurement << current_x, current_y;
            kf.predict();
            kf.correct(measurement);
            state_x = kf.X(0);
            state_y = kf.X(1);
            

            // Integral Part of PID
            sum_error_x -= state_x;
            sum_error_y -= state_y;
      
            // PID
            twist.linear.x = current_x * 1.0 + sum_error_x * 0.1 + 1.0 *(current_x - last_error_x);
            twist.linear.y = current_y * 1.0 + sum_error_y * 0.1 + 1.0 *(current_y - last_error_y);
            
            printf("Measured vals x: %f, y: %f\n", transform.getOrigin().x(), transform.getOrigin().y());
            printf("Corrected vals x: %f, y: %f\n", state_x, state_y);
            printf("Twist Corrected x: %f, y: %f\n", twist.linear.x, twist.linear.y);
            printf("------- Frame End ---------\n");
            last_error_x = state_x;
            last_error_y = state_y;
        }
        catch(tf::TransformException &ex) { // If no tf are received: break operations until next loop
            ROS_ERROR("%s", ex.what());
            ros::Duration(1.0).sleep();
        }

        // Lost marker control
        if(twist.linear.x == twist_old.linear.x && twist.linear.y == twist_old.linear.y && twist.linear.z == twist_old.linear.z) {
            twist.linear.x = 0.0;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.z = 0.0;
        }
        else twist_old = twist;

        // States
        printf("Altitude: %f m\n", altd);
        if(state == 0) {
            printf("taking off\n");
            ros::Publisher pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff",1);
            double time_start = (double)ros::Time::now().toSec();
            while((double)ros::Time::now().toSec() < time_start + 6.0) { // send conmmand for 3 sec
                pub.publish(std_msgs::Empty());
                ros::spinOnce();
            }
            state = 1;
        }
        // State - Following
        if(state == 1) {
            printf("Stable\n");
            ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
            double time_start = (double)ros::Time::now().toSec();
            while((double)ros::Time::now().toSec() < time_start + 0.25) {
                pub.publish(twist);
                ros::spinOnce();
                rate.sleep();
            }
            if ((double)ros::Time::now().toSec() >= time_fly+10) state = 2;
        }
        // State - Land the drone
        if(state == 2) {
            if (altd > 0.4) {
                printf("Landing!!!\n");
                ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
                twist.linear.z = -1.0;
                double time_start = (double)ros::Time::now().toSec();
                while((double)ros::Time::now().toSec() < time_start + 0.1) {
                    pub.publish(twist);
                    ros::spinOnce();
                    rate.sleep();
                }
            }
            else {
                printf("Landed\n");
                ros::Publisher pub = n.advertise<std_msgs::Empty>("/ardrone/land", 1);
                double time_start = (double)ros::Time::now().toSec();
                while((double)ros::Time::now().toSec() < time_start + 1.0) {
                    pub.publish(std_msgs::Empty()); // Land the drone
                    ros::spinOnce();
                    rate.sleep();
                }
                return 0;
            }
        }

        // Emergency landing
        if(state == 3) {
            ros::Publisher pub = n.advertise<std_msgs::Empty>("/ardrone/land", 1);
            double time_start = (double)ros::Time::now().toSec();
            while((double)ros::Time::now().toSec() < time_start + 3.0) {
                pub.publish(std_msgs::Empty()); // Land the drone
                ros::spinOnce();
                rate.sleep();
            }
            return 0;
        }
        rate.sleep();
    }
    return 0;
}

