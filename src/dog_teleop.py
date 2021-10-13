#! /usr/bin/python3

import rospy

from geometry_msgs.msg import Twist
from std_msgs.msg import Int16
from joybro.msg import JoyBro

class DogTeleop():

    def __init__(self):

        self.state = 'stop'
        
        self.max_linear_vel = rospy.get_param('~max_linear_vel',0.25)
        self.max_angular_vel = rospy.get_param('~max_angular_vel',2)
        self.threshold = rospy.get_param('~threshold',20)
        
        self.sub = rospy.Subscriber("/joybro", JoyBro, self.callback)
        self.pub = rospy.Publisher("/cmd_vel", Twist, queue_size=5)
        self.pub = rospy.Publisher("/rc_mode", Int16, queue_size=5)


    def callback(self, data):

        linear_vel_x = 0
        angular_vel_z = 0
        linear_vel_y = 0 

        if self.state == 'stop' and data.btn3 == 1:
            self.state = 'move'

        if(self.state == 'move' and data.btn3 == 0):
            twist = Twist()
            self.pub.publish(twist)
            self.state = 'stop'

        if (self.state == "move" or self.state == "stop" and data.btn2 == 1):
            twist = Twist()
            self.pub.publish(twist)
            self.state == "stand"

        if abs(data.left_y) > self.threshold:
            linear_vel_x = (data.left_y/512.0)*self.max_linear_vel

        if abs(data.left_x) > self.threshold:    
            angular_vel_z = -(data.left_x/512.0)*self.max_angular_vel

        if abs(data.right_x) > self.threshold:    
            linear_vel_y = -(data.left_x/512.0)*self.max_angular_vel


        if (self.state == 'move'):
            twist = Twist()
            twist.linear.x = linear_vel_x
            twist.linear.y = linear_vel_y
            twist.angular.z = angular_vel_z
            twist.linear.z = data.slider1 #* K #TBD!!!!
            self.pub.publish(twist)

        if (self.state == 'move'):
            twist = Twist()
            twist.angular.x = linear_vel_x
            twist.angular.y = linear_vel_y
            twist.angular.z = angular_vel_z
            twist.linear.z = data.slider1 #* K #TBD!!!!
            self.pub.publish(twist)

    def run(self):
        rospy.loginfo("Start Dog Teleop")
        rospy.spin()


if __name__ == '__main__':
            
    rospy.init_node('dog_teleop', anonymous=True)
    teleop = DogTeleop()
    teleop.run()