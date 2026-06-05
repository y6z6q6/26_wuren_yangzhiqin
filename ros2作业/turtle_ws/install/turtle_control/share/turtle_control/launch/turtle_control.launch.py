from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    pkg = get_package_share_directory("turtle_control")
    yaml_path = os.path.join(pkg, "config", "params.yaml")
    return LaunchDescription([
    	#第一个节点：模拟器
        Node(
            package="turtlesim",
            executable="turtlesim_node",
            output="screen"
        ),
        #第二个节点：控制乌龟方向速度
        Node(
            package="turtle_control",
            executable="turtle_run",
            parameters=[yaml_path],
            output="screen"
        )
    ])
