#! /usr/bin/python3

import rospy

from geometry_msgs.msg import Twist
from std_msgs.msg import Int16
from joybro.msg import JoyBro

class DogTeleop():

    def __init__(self):

        self.state = 'stop'
        
        self.max_linear_vel = rospy.get_param('~max_linear_vel',0.5)
        self.max_angular_vel = rospy.get_param('~max_angular_vel',1.5)
        self.threshold = rospy.get_param('~threshold',20)
        self.max_height = rospy.get_param('~max_height',0.5)
        
        self.sub = rospy.Subscriber("/joybro", JoyBro, self.callback)
        self.pub = rospy.Publisher("/cmd_vel", Twist, queue_size=5)
        self.pub_rc = rospy.Publisher("/rc_mode", Int16, queue_size=5)


    def callback(self, data):

        linear_vel_x = 0
        angular_vel_z = 0
        linear_vel_y = 0 
        rc_mode = Int16()

        if data.sw3:
            self.state = "stand"
            rc_mode.data = 3
            self.pub_rc.publish(rc_mode)

        if data.sw2:
            self.state = "move"
            rc_mode.data = 11
            self.pub_rc.publish(rc_mode)
        
        if data.sw1:
            self.state = "stop"
            rc_mode.data = 2
            self.pub_rc.publish(rc_mode)
        
        if data.btn1:
            self.state = "stop"
            rc_mode.data = 12
            self.pub_rc.publish(rc_mode)
        

        if abs(data.left_y) > self.threshold:
            linear_vel_x = (data.left_y/512.0)*self.max_linear_vel

        if abs(data.left_x) > self.threshold:    
            angular_vel_z = -(data.left_x/512.0)*self.max_angular_vel

        if abs(data.right_x) > self.threshold:    
            linear_vel_y = -(data.right_x/512.0)*self.max_angular_vel
        
        height = -((512.0 - data.slider1)/512.0)*self.max_height

        if (self.state == 'stand'):
            twist = Twist()
            twist.angular.y = linear_vel_x
            twist.angular.x = linear_vel_y/4
            twist.angular.z = angular_vel_z/5
            twist.linear.z = height
            self.pub.publish(twist)
        else:
            twist = Twist()
            twist.linear.x = linear_vel_x
            twist.linear.y = linear_vel_y
            twist.angular.z = angular_vel_z
            twist.linear.z = height 
            self.pub.publish(twist)

        
    def run(self):
        rospy.loginfo("Start Dog Teleop")
        rospy.spin()


if __name__ == '__main__':
            
    rospy.init_node('dog_teleop', anonymous=True)
    teleop = DogTeleop()
    teleop.run()