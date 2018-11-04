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
using namespace boost;
using namespace boost::this_thread;

int state = 0; // Three states: 0 - ready to takeoff
// 1 - follow marker
// 2 - land
double r = 0;
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
	state = 1;
	char land;
	std::cin >> land;
	state = 2;
}

void sonarHeightCallback(const sensor_msgs::Range::ConstPtr& msg ) {
	ROS_INFO("Height is: %f", msg->range);
	r = msg->range;
}

void navdataSpeedCallback(const ardrone_autonomy::NavdataConstPtr nav) {
	vel_x = nav->vx * 0.001;
	vel_y = nav->vy * 0.001;
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
	twist.angular.x = 0.1;
	twist.angular.y = 0.1;
	twist.angular.z = 0.0;
	double X0 = 0.0, state_x = 0.0;
	double Y0 = 0.0, state_y = 0.0;
	double P0 = 0.0, covariance = 0.1;
	double Q = 2.0, K = 0.0, R = 15;
	double current_x = 0.0, current_y = 0.0;

	while(ros::ok()) {
// Lookup transform
		tf::StampedTransform transform;
		transform.setOrigin(tf::Vector3(0.0, 0.0, 0.0));
		ros::Subscriber navdata = n.subscribe("ardrone/navdata", 10,navdataSpeedCallback);
		//ros::Subscriber sub = n.subscribe("sonar_height", 1000,sonarHeightCallback);
		try {
			listener.lookupTransform("/ardrone_base_bottomcam", "ar_marker_0",
				ros::Time(0), transform);
			printf("------- Frame Start -------\n");
			printf("Received TF: x: %f, y: %f, z: %f. \n\n",
				transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z());
			printf("vx: %f, vy: %f \n", vel_x, vel_y);
			current_x = transform.getOrigin().x();
			current_y = transform.getOrigin().y();
	// Ugly Simple Form of Kalman Filter
	// Time Update (prediction)
			X0 = state_x + vel_x * 0.01;
			Y0 = state_y + vel_y * 0.01;
			P0 = covariance + Q;
	// Measurements update (correction)
			K = P0 / (P0 + R);
			state_x = X0 + K * (current_x - X0);
			state_y = Y0 + K * (current_y - Y0);
			covariance = (1 - K) * P0;
	// Integral Part of PID
			sum_error_x += -state_x;
			sum_error_y += -state_y;
	// PID
			twist.linear.x = -current_y * 0.1 +  sum_error_y * 0.01 + 1 *(-current_y - last_error_y);
			twist.linear.y = -current_x * 0.1 +  sum_error_x * 0.01 + 1 *(-current_x - last_error_x);
			twist.linear.z = -(transform.getOrigin().z()-1)*0.3;
			printf("Measured vals x: %f, y: %f\n", transform.getOrigin().x(), transform.getOrigin().y());
			printf("Corrected vals x: %f, y: %f\n", state_x, state_y);
	//printf("I-error x: %f y: %f\n", sum_error_x, sum_error_y);
	//printf("D-error x: %f, y: %f\n\n", -transform.getOrigin().x() -last_error_x, -transform.getOrigin().y() - last_error_y);
	//printf("P-term x: %f, y: %f\n", -transform.getOrigin().x()*0.13,-transform.getOrigin().y()*0.13);
	//printf("I-term x: %f y: %f\n", 0.02 * sum_error_x * 0.1, 0.02 *sum_error_y * 0.1);
	//("D-term x: %f, y: %f\n\n", 0.45 * (-transform.getOrigin().x() -last_error_x), 0.45 * (-transform.getOrigin().y() - last_error_y));
			printf("Final Twist x: %f, y: %f\n", -current_y * 0.13 + 0.45 *(-state_y - last_error_y), -current_x * 0.13 + 0.45 * (-state_x - last_error_x));
			printf("Final Twist Corrected x: %f, y: %f\n", twist.linear.x,twist.linear.y);
			printf("------- Frame End ---------\n");
			last_error_x = -current_x;
			last_error_y = -current_y;
	//}
	//}
		}
		catch(tf::TransformException &ex) { // If no tf are received: break operations until next loop
			ROS_ERROR("%s", ex.what());
			ros::Duration(1.0).sleep();
		//continue;
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
	// State 0: takeoff
		if(state == 0) {
			ros::Publisher pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff",1);
			double time_start = (double)ros::Time::now().toSec();
			while((double)ros::Time::now().toSec() < time_start + 3.0) { // send conmmand for 3 sec
				pub.publish(std_msgs::Empty());
				ros::spinOnce();
			}
		}
	// State - follow marker
		if(state == 1) {
			ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
			double time_start = (double)ros::Time::now().toSec();
			while((double)ros::Time::now().toSec() < time_start + 0.25) { //send command for 0.1 sec
				pub.publish(twist);
				ros::spinOnce();
			}
		}
	 // State - land the drone
		if(state == 2) {
			ros::Publisher pub = n.advertise<std_msgs::Empty>("/ardrone/land", 1);
			double time_start = (double)ros::Time::now().toSec();
			while((double)ros::Time::now().toSec() < time_start + 3.0) {
				pub.publish(std_msgs::Empty()); // Land the drone
				ros::spinOnce();
				rate.sleep();
			}
			state = 4;
		}
		if(state == 4) {
			return 0;
		}
		rate.sleep();
	}
	return 0;
}
