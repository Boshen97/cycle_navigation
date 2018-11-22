//需要的头文件
#ifndef NAV_TOOL_H
#define NAV_TOOL_H

#ifndef Q_MOC_RUN  
#include <QObject>

#include <ros/ros.h>
#include <rviz/panel.h>
#include "rviz/default_plugin/tools/pose_tool.h"
#endif
class QLineEdit;
namespace rviz
{
class Arrow;
class DisplayContext;
class StringProperty;

class Cycle_Goal: public PoseTool
{
Q_OBJECT
public:
  Cycle_Goal();
  virtual ~Cycle_Goal() {}
  virtual void onInitialize();

protected:
  virtual void onPoseSet(double x, double y, double theta);
	QLineEdit* output_topic_editor_;
    QString output_topic_;

private Q_SLOTS:
  void updateTopic();
  void over();
private:
	//生成句柄和导航点发布者
  ros::NodeHandle nh_;
  ros::Publisher pub_;

  StringProperty* topic_property_;
};

}

#endif

