//代码说明：在rviz中生成一个插件，用以接收单个导航点以及循环次数，并通过可控方式发布多个导航点和循环次数
//需要的头文件
#include <stdio.h>
#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Pose.h>
#include <std_msgs/Int8.h>
#include <QDebug>
#include "nav_panel.h"

std::vector<geometry_msgs::Pose> pose_list;
geometry_msgs::PoseArray msg;
std_msgs::Int8 t;
namespace rviz_cycle_nav
{
int i = 0;
int count = 0;
// 接收单个导航目标点，顺序存入vector
void callback(const geometry_msgs::PoseStamped::ConstPtr& goal)
{
	geometry_msgs::Point point;
	geometry_msgs::Quaternion quaternion;
	geometry_msgs::Pose pose;
	point.x = goal->pose.position.x;
	point.y = goal->pose.position.y;
	point.z = 0;
	quaternion.x = 0;
	quaternion.y = 0;
	quaternion.z = goal->pose.orientation.z;
	quaternion.w = goal->pose.orientation.w;
	pose.position = point;
	pose.orientation = quaternion;
	pose_list.push_back(pose);
	count++;
}
// 构造函数，初始化变量
NavPanel::NavPanel( QWidget* parent )
  : rviz::Panel( parent )

{
	//框头
	QVBoxLayout* topic_layout = new QVBoxLayout;
	//创建循环次数输入框
	topic_layout->addWidget( new QLabel( "set cycle times" ));
  output_topic_editor_ = new QLineEdit;
  topic_layout->addWidget( output_topic_editor_ );
	//创建数据发布框
	topic_layout->addWidget( new QLabel( "send PointArray" ));
  output_topic_editor_1 = new QLineEdit;
  topic_layout->addWidget( output_topic_editor_1 );
	//框尾
  QHBoxLayout* layout = new QHBoxLayout;
  layout->addLayout( topic_layout );
  setLayout( layout );
  //定义循环次数、多导航点发布者，单个导航点接收者
	vec_pub = n.advertise<geometry_msgs::PoseArray>("Nav_PoseArray",10);
	times_pub = n.advertise<std_msgs::Int8>("Cycle_times",10);
	vec_sub = n.subscribe("Cycle_Goal",1,callback);
	//连接信号，敲下回车，执行SLOT内的函数
	connect( output_topic_editor_, SIGNAL( returnPressed() ), this, SLOT( sendtimes() ));
  connect( output_topic_editor_1, SIGNAL( returnPressed() ), this, SLOT( sendgoal() ));
}

//析构函数
NavPanel::~NavPanel()
{
	delete output_topic_editor_;
	delete output_topic_editor_1;
}

//发送循环次数
void NavPanel::sendtimes()
{
	QString temp_string = output_topic_editor_->text();
    int value = temp_string.toInt();  
    t.data = value;
		times_pub.publish(t);
}

//发送多导航点数组
void NavPanel::sendgoal()
{
	msg.poses = pose_list;
	vec_pub.publish(msg);
}


// 重载父类的功能
void NavPanel::save( rviz::Config config ) const
{
    rviz::Panel::save( config );
}

// 重载父类的功能，加载配置数据
void NavPanel::load( const rviz::Config& config )
{
    rviz::Panel::load( config );

}

}

// 声明此类是一个rviz的插件
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(rviz_cycle_nav::NavPanel,rviz::Panel )
// END_TUTORIAL
