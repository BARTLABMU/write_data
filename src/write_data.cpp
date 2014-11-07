#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include "geometry_msgs/PoseStamped.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <std_msgs/Float32.h>

using namespace std;
double robot_x = 0;
double robot_y = 0 ;
float z= 0;
int id = 1;
  std::ofstream myfile;
  time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];


void qrpos_callback(const std_msgs::Float32ConstPtr& msg)
{
     z = msg->data;

}
void qrr_callback(const std_msgs::String::ConstPtr& msg)
{

 myfile.open ("/home/bl-teleop/Desktop/map/qr.csv", std::ios::out | std::ios::app);
  ROS_INFO("heard: [%s]", msg->data.c_str());
  time_t     now1 = time(0);
    struct tm  tstruct1;
    char       buf1[80];

    tstruct1 = *localtime(&now1);
      strftime(buf1, sizeof(buf1), "%Y-%m-%d.%X", &tstruct1);

myfile << id <<"; " << buf1 <<"; " << msg->data<<"; " << robot_x <<"; " << robot_y<<"; "<< z <<"; " << endl;
myfile.close();
id++;
}

void pos_callback(const geometry_msgs::PoseStampedConstPtr& pos)
{
    robot_x = pos->pose.position.x;
    robot_y = pos->pose.position.y;
}


int main(int argc, char **argv)
{

  ros::init(argc, argv, "write");

  ros::NodeHandle n("~");


  ros::Subscriber qrr_sub = n.subscribe("/qr_detection/qrdata", 1, qrr_callback);

  ros::Subscriber pos_sub = n.subscribe("slam_out_pose",1, pos_callback);
  ros::Subscriber posqr_sub = n.subscribe("/qr_detection/qr_pos",1,qrpos_callback);
  //ros::Subscriber qpos_sub = n.subscribe("qr_pos",1,qrpos_callback);
  string mission = "MISSION";
  n.getParam("mission",mission);
  tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

  myfile.open ("/home/bl-teleop/Desktop/map/qr.csv", std::ios::out | std::ios::app);
  myfile << "BART LAB RESCUE" << endl;
  myfile << buf << endl;
  myfile << mission.c_str() << endl << endl;
  myfile.close();
  ros::spin();

  return 0;
}

