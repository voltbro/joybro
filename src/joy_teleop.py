#! /usr/bin/python

import rospy

from geometry_msgs.msg import Twist
from joybro.msg import JoyBro

class JoyBroTeleop():

    def __init__(self):
        
        rospy.init_node('joy_bro_teleop', anonymous=True)

        self.state = 'stop'
        
        self.max_linear_vel = rospy.get_param('~max_linear_vel',0.25)
        self.max_angular_vel = rospy.get_param('~max_angular_vel',2)
        self.threshold = rospy.get_param('~threshold',20)
        
        self.sub = rospy.Subscriber("/joybro", JoyBro, self.callback)
        self.pub = rospy.Publisher("/cmd_vel", Twist, queue_size=5)

    def callback(self, data):

        linear_vel = 0
        angular_vel = 0


        if self.state == 'stop' and data.btn3 == 1:
            self.state = 'move'

        if(self.state == 'move' and data.btn3 == 0):
            twist = Twist()
            self.pub.publish(twist)
            self.state = 'stop'

        if abs(data.left_y) > self.threshold:
            linear_vel = (data.left_y/512.0)*self.max_linear_vel

        if abs(data.left_x) > self.threshold:    
            angular_vel = -(data.left_x/512.0)*self.max_angular_vel

        if (self.state == 'move'):
            twist = Twist()
            twist.linear.x = linear_vel
            twist.angular.z = angular_vel
            self.pub.publish(twist)


    def run(self):
        rospy.loginfo("Start JoyBro Teleop")
        rospy.spin()


if __name__ == '__main__':
    teleop = JoyBroTeleop()
    teleop.run();