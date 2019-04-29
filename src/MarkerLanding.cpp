#include "ros/ros.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
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
double limit = 1.0;
double sum_error_x = 0.0;
double sum_error_y = 0.0;
double sum_error_z = 0.0;
double last_error_x = 0.0;
double last_error_y = 0.0;
double last_error_z = 0.0;
double diff_x = 0.0;
double diff_y = 0.0;
double vel_x = 0.0;
double vel_y = 0.0;
double current_x = 0.0, current_y = 0.0;
double rotX = 0.0, rotY = 0.0;
geometry_msgs::Twist twist;

void checkCin() {
    char input;
    while(1) {
        std::cin >> input;
        if (input == 's') {
            state = 1;
        }
        if (input == 'f') {
            state = 2;
        }
        if (input == 'l') {
            state = 3;
        }
        if (input == 'e') {
            state = 4;
        }
    }
}

void posUGVCallback(const geometry_msgs::Point point) {
    current_x = -point.y;
    current_y = -point.x;
}

void navdataCallback(const ardrone_autonomy::NavdataConstPtr nav) {
    vel_x = nav->vx * 0.001;
    vel_y = nav->vy * 0.001;
    altd = nav->altd * 0.001;
    rotX = nav->rotX;
    rotY = nav->rotY;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "MarkerLanding");
    ROS_INFO("MarkerLanding : Started");
    // Define node handler for subs
    // tf::TransformListener listener;
    ros::NodeHandle n;

    initTime = (double)ros::Time::now().toSec();
    // Loop rate
    ros::Rate rate(100);

    // Thread that waits user input
    boost::thread t(&checkCin);

    twist.linear.x = 0.0;
    twist.linear.y = 0.0;
    twist.linear.z = 0.0;
    twist.angular.x = 0.0;
    twist.angular.y = 0.0;
    twist.angular.z = 0.0;
    
    int size_n = 4;
    int size_m = 2;
    double Kp=0.0001, Kd=0.0, Ki=0.0, Kpz = 0.005, Kdz = 0.0, Kp_tx = 0.0, Kp_ty = 0.0;
    double threshold_error = 200;
    double dt = 0.01;
    double state_x = 0.0, state_y = 0.0;
    double z_ref = 1.6;
    double error_z = 0.0;
    
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
    ros::Subscriber pos_ugv = n.subscribe("pos_ugv", 10, posUGVCallback);
    while(ros::ok()) {
        tf::StampedTransform transform;
        transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
        
        /*measurement << current_x, current_y;
        kf.predict();
        kf.correct(measurement);
        state_x = kf.X(0);
        state_y = kf.X(1);*/
        state_x = current_x;
        state_y = current_y;


        // Integral Part of PID
        if (sum_error_y > threshold_error or  sum_error_x > threshold_error ) {
            sum_error_x += 0;
            sum_error_y += 0;
        }
        else {
        sum_error_x += state_x;
        sum_error_y += state_y;
        }

        // PID
        if (abs(state_x)  > 100 or abs(state_y) > 100) {
            printf("True Following\n");
            twist.linear.x = Kp * state_x + Kd * (state_x - last_error_x) + Ki * sum_error_x;// - 0.01;
            twist.linear.y = Kp * state_y + Kd * (state_y - last_error_y) + Ki * sum_error_y;// + 0.005;
        }
        else {
            printf("Stable\n");
            twist.linear.x = 0.0;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.x = 0.0;
            twist.angular.y = 0.0;
            twist.angular.z = 0.0;
        }
        error_z = z_ref - altd;
        sum_error_z += error_z;

        // PID for altitude
        twist.linear.z = Kpz  * error_z  + Kdz * (error_z - last_error_z)  + Ki * sum_error_z ;

        //twist.angular.x = -Kp_tx  * rotX;
        //twist.angular.y = -Kp_ty  * rotY;

        last_error_x = state_x;
        last_error_y = state_y;
        last_error_z = altd;

        printf("Measured vals x: %f, y: %f\n", current_x, current_y);
        printf("Corrected vals x: %f, y: %f\n", state_x, state_y);
        printf("Vel x: %f, Vel y: %f, Vel z: %f\n", twist.linear.x, twist.linear.y, twist.linear.z);
        printf("Error z: %f\n", error_z);
        printf("------- Frame End ---------\n");



        // States
        // State - Taking off
        printf("Altitude: %f m\n", altd);
        if(state == 0) {
            printf("taking off\n");
            ros::Publisher pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);
            double time_start = (double) ros::Time::now().toSec();
            while ((double) ros::Time::now().toSec() < time_start + 6.0) {
                pub.publish(std_msgs::Empty());
                ros::spinOnce();
            }
            //state = 1;
        }
        if(state == 1) {
            printf("Stable\n");
            twist.linear.x = 0.0;
            twist.linear.y = 0.0;
            twist.linear.z = 0.0;
            twist.angular.x = 0.0;
            twist.angular.y = 0.0;
            twist.angular.z = 0.0;
            ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
            double time_start = (double) ros::Time::now().toSec();
            while ((double) ros::Time::now().toSec() < time_start + 0.5) {
                pub.publish(twist);
                ros::spinOnce();
                rate.sleep();
            }
        }
        if(state == 2) {
            printf("Following\n");
            ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
            double time_start = (double)ros::Time::now().toSec();
            while((double)ros::Time::now().toSec() < time_start + 0.5) {
                pub.publish(twist);
                ros::spinOnce();
                rate.sleep();
            }
        }
        // State - Land the drone
        if(state == 3) {
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
        if(state == 4) {
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

