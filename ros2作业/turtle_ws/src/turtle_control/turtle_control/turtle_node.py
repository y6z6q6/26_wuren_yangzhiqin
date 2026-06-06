import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import math

class Turtle(Node):
    #初始化
    def __init__(self):
        super().__init__("turtle_control_node")#创建节点名字为turtle_control_node
        #初始化节点参数数值
        self.declare_parameters(
            namespace="",
            parameters=[
                ("xiansudu", 0.25),
                ("jiaosudu", 1.8), 
                ("zhouqi", 12.0),   
                ("pinlv", 10.0)      
            ]
        )
        #读取yaml文件里设定好的参数值
        self.xiansudu = self.get_parameter("xiansudu").get_parameter_value().double_value
        self.jiaosudu = self.get_parameter("jiaosudu").get_parameter_value().double_value
        self.zhouqi = self.get_parameter("zhouqi").get_parameter_value().double_value
        self.pinlv = self.get_parameter("pinlv").get_parameter_value().double_value
        #发出速度话题
        self.publisher = self.create_publisher(Twist, "/turtle1/cmd_vel", 10)
        timer_period = 1.0 / self.pinlv
        #设定更新频率，每timer_period秒，就进入timer_callback函数一次
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.start_time = self.get_clock().now().nanoseconds / 1e9
    #回调函数
    def timer_callback(self):
    	#计算现在时间
        now = self.get_clock().now().nanoseconds / 1e9
        t = now - self.start_time
        omega = self.jiaosudu * math.sin(2 * math.pi * t / self.zhouqi)
        #初始化twist为Twist数据类型
        twist = Twist()
        twist.linear.x = self.xiansudu
        twist.angular.z = omega
        #发布速度话题
        self.publisher.publish(twist)

def main(args=None):
    rclpy.init(args=args)
    node = Turtle() 
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()
