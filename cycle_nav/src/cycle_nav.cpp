//代码说明：接收循环次数和多个导航点，对数据进行处理，循环发布导航点至move_base
//需要的头文件
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/PoseArray.h>
#include <std_msgs/Int8.h>
//定义一个vector用以接收多个导航点
std::vector<geometry_msgs::Pose> pose_list;
//声明循环次数和多导航点的接收者
ros::Subscriber vec_sub;
ros::Subscriber tim_sub;
//声明循环次数，导航目标个数，循环需要的中间值
int Cycle_times,Goals_num,count = 0;
//接收循环次数
void timcallback(const std_msgs::Int8::ConstPtr& msg)
{
	Cycle_times = msg->data;
}
//接收导航点数组，循环发布导航点到move_base
void callback(const geometry_msgs::PoseArray::ConstPtr& pose_array)
{
	//取得导航点个数
	Goals_num = pose_array->poses.size();
	//存入多个导航点
	for(int i =0;i<Goals_num;i++)
	{
		pose_list.push_back(pose_array->poses[i]);
	}	
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> cycle_client("move_base",true);
	ROS_INFO("Waiting for move_base action server...");
	if(!cycle_client.waitForServer(ros::Duration(60)))
	{
		ROS_INFO("Can't connected to move base server");
	}
	ROS_INFO("Connected to move base server");
	ROS_INFO("Starting navigation ");
	ROS_INFO("cycle times is %d",Cycle_times);
	ROS_INFO("cycle goals is %d",Goals_num);
	//多次循环
	for (int i =0 ; i<Cycle_times ; i++)
	{
		//循环一次所发布的多个导航点
		while(count <Goals_num)
		{
			move_base_msgs::MoveBaseGoal goal;
			goal.target_pose.header.frame_id = "map";
			goal.target_pose.header.stamp = ros::Time::now();
			goal.target_pose.pose = pose_list[count];
			cycle_client.sendGoal(goal);
			//60S内如果不能达到目标点则放弃该目标点
			bool finished_within_time = cycle_client.waitForResult(ros::Duration(60));
			if(!finished_within_time)
			{
				cycle_client.cancelGoal();
				ROS_INFO("Timed out achieving goal");
			}
			else
			{
				//导航成功
				if(cycle_client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				{
					ROS_INFO("Goal %d succeeded!",count+1);
				}
				//导航失败
				else
				{
					ROS_INFO("move plan is error");
				}
		
			}
			count += 1;
	}
	count = 0;
	}
	//导航结束，清空存放多个导航点的vector
	ROS_INFO("The Cycle Goals is over");
	pose_list.clear();
}

int main(int argc,char** argv)
{
	//初始化并设置节点名
	ros::init(argc,argv,"cycle_nav");
	//创建句柄
	ros::NodeHandle n;
	ROS_INFO("Cycle_nav is start...");
	//定义循环次数的接收者
	tim_sub = n.subscribe("Cycle_times",5,timcallback);
	//定义多导航点的接受者
	vec_sub = n.subscribe("Nav_PoseArray",5,callback);
	//循环等待回调函数
	ros::spin();
	return 0;
}
