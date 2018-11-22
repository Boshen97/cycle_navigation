////代码说明：通过使用该工具在rviz中的地图中点击将产生一个导航点，将导航点发布
//需要的头文件
#include <tf/transform_listener.h>
#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PoseArray.h>
#include "rviz/display_context.h"
#include "rviz/properties/string_property.h"
#include "nav_tool.h"
#include <vector>
#include <QDebug>

namespace rviz
{
Cycle_Goal::Cycle_Goal()
{
  shortcut_key_ = 'g';

  topic_property_ = new StringProperty( "Topic", "Cycle_Goal",
                                        "The topic on which to publish navigation goals.",
                                        getPropertyContainer(), SLOT( updateTopic() ), this );

}
	

void Cycle_Goal::onInitialize()
{
  PoseTool::onInitialize();
  setName( "Cycle Goal" );
  updateTopic();
}

void Cycle_Goal::updateTopic()
{
 	pub_ = nh_.advertise<geometry_msgs::PoseStamped>( topic_property_->getStdString(), 1 );
}


//接收单个导航点并发布
void Cycle_Goal::onPoseSet(double x, double y, double theta)
{
  std::string fixed_frame = context_->getFixedFrame().toStdString();
  tf::Quaternion quat;
  quat.setRPY(0.0, 0.0, theta);
  tf::Stamped<tf::Pose> p = tf::Stamped<tf::Pose>(tf::Pose(quat, tf::Point(x, y, 0.0)), ros::Time::now(), fixed_frame);
	geometry_msgs::PoseStamped goal;
  tf::poseStampedTFToMsg(p, goal);
  ROS_INFO("Setting goal: Frame:%s, Position(%.3f, %.3f, %.3f), Orientation(%.3f, %.3f, %.3f, %.3f) = Angle: %.3f\n", 			    fixed_frame.c_str(),
  goal.pose.position.x, goal.pose.position.y, goal.pose.position.z,
  goal.pose.orientation.x, goal.pose.orientation.y, goal.pose.orientation.z, goal.pose.orientation.w, theta);
	pub_.publish(goal);
}
}

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS( rviz::Cycle_Goal, rviz::Tool )
