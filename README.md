# 机器人多点循环导航功能包

# 功能包说明

cycle_nav: 接收多个导航点和循环次数，实现多点循环导航功能

nav_panel: rviz插件，用以接收单个导航点，设置循环次数，发布多个导航点和循环次数

nav_tool: rviz工具，发布单个导航点

# 使用说明：
#使用该功能包需有仿真或者真实机器人
1.首先启动仿真机器人和仿真环境或者真实机器人
2.打开rviz，添加Cycle_Goal工具和NavPanel插件
3.使用Cycle_Goal工具点击多个导航点
4.rosrun cycle_nav cycle_nav 启动节点
5.在NavPanel中设置循环次数按下回车，在send PointArray框中按下回车即可发布循环次数和多导航点

#使用之前需配备move_base包和rviz包

#联系本人
1.QQ邮箱: 554123904@qq.com
2.CSDN博客： boshen97

#问题说明
1.在关闭节点时会出现core dumped（问题未解决，如有解决方案可联系本人）
2.如有其他问题或疑问均可联系本人
