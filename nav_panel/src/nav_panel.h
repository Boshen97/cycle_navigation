#ifndef NAV_PANEL_H
#define NAV_PANEL_H


#ifndef Q_MOC_RUN
#include <ros/ros.h>
#include <ros/console.h>
#include <rviz/panel.h>   
#endif

class QLineEdit;

namespace rviz_cycle_nav
{
// 所有的plugin都必须是rviz::Panel的子类
class NavPanel: public rviz::Panel
{
// 后边需要用到Qt的信号和槽，都是QObject的子类，所以需要声明Q_OBJECT宏
Q_OBJECT
public:
    // 构造函数，在类中会用到QWidget的实例来实现GUI界面，这里先初始化为0即可
    NavPanel( QWidget* parent = 0 );
		virtual ~NavPanel();
    // 重载rviz::Panel积累中的函数，用于保存、加载配置文件中的数据，在我们这个plugin
    // 中，数据就是topic的名称
    virtual void load( const rviz::Config& config );
    virtual void save( rviz::Config config ) const;

// 内部槽.
protected Q_SLOTS:
    void sendgoal();                // 发布多个导航目标点
		void sendtimes();								// 发布循环次数

// 内部变量.
protected:
    // 循环次数输入框
    QLineEdit* output_topic_editor_;
    QString output_topic_;
		// 发送导航点数组
		QLineEdit* output_topic_editor_1;
    QString output_topic_1;
    // ROS的publisher，用来发布速度topic
    ros::Publisher vec_pub;
	  ros::Subscriber vec_sub;
		ros::Publisher times_pub;
    // ROS节点句柄
    ros::NodeHandle n;
		int value;
};

} 

#endif 
