#! /usr/bin/python3

import rospy, time

from joybro.msg import JoyBro
from std_msgs.msg import Int16

class ANtiTeleop():

    top_cam_UD = 70
    top_cam_LR = 100
    top_camera_step = 2

    poliv_UD = 155
    poliv_LR = 180
    poliv_step = 2

    zabor_UD = 180

    def __init__(self):
        
        rospy.init_node('joy_bro_anti_teleop', anonymous=True)
        
        self.max_linear_vel = rospy.get_param('~max_linear_vel',0.25)
        self.max_angular_vel = rospy.get_param('~max_angular_vel',2)
        self.threshold = rospy.get_param('~threshold',40)
        
        self.camera_ud_pub = rospy.Publisher("/camera_ud", Int16, queue_size=5)
        self.camera_lr_pub = rospy.Publisher("/camera_lr", Int16, queue_size=5)

        self.poliv_ud_pub = rospy.Publisher("/poliv_ud", Int16, queue_size=5)
        self.poliv_lr_pub = rospy.Publisher("/poliv_lr", Int16, queue_size=5)  

        self.zabor_ud_pub = rospy.Publisher("/zabor_ud", Int16, queue_size=5)  

        self.init_pose()

        self.sub = rospy.Subscriber("/joybro", JoyBro, self.callback)

    def init_pose(self):

        time.sleep(1)
        self.camera_ud_pub.publish(self.top_cam_UD)
        self.camera_lr_pub.publish(self.top_cam_LR)

        self.poliv_ud_pub.publish(self.poliv_UD)
        self.poliv_lr_pub.publish(self.poliv_LR)

        self.zabor_ud_pub.publish(self.zabor_UD)

        
    def callback(self, data):
        self.top_camera_cb(data)
        self.poliv_cb(data)
        self.zabor_cb(data)

    def zabor_cb(self, data):
        self.zabor_ud_pub.publish(int(180 - ((data.slider2/1024) * 180)))
            

    def poliv_cb(self, data):
        
        if data.btn4 == 1:

            if abs(data.left_y) > self.threshold * 3:

                if(data.left_y >0):
                    self.poliv_UD = self.dec(self.poliv_UD, self.poliv_step, 50) 

                if(data.left_y < 0):
                    self.poliv_UD = self.inc(self.poliv_UD, self.poliv_step, 155) 

            self.poliv_ud_pub.publish(self.poliv_UD)


            if abs(data.left_x) > self.threshold * 3:

                if(data.left_x >0):
                    self.poliv_LR = self.dec(self.poliv_LR, self.poliv_step, 30) 

                if(data.left_x < 0):
                    self.poliv_LR = self.inc(self.poliv_LR, self.poliv_step, 180)

            self.poliv_lr_pub.publish(self.poliv_LR)


    def top_camera_cb(self, data):

        if abs(data.right_y) > self.threshold * 3:
            if(data.right_y >0):
                self.top_cam_UD = self.dec(self.top_cam_UD, self.top_camera_step, 0) 

            if(data.right_y < 0):
                self.top_cam_UD = self.inc(self.top_cam_UD, self.top_camera_step, 180) 

        self.camera_ud_pub.publish(self.top_cam_UD)

        if abs(data.right_x) > self.threshold * 3:
            if(data.right_x >0):
                self.top_cam_LR = self.inc(self.top_cam_LR, self.top_camera_step, 180)

            if(data.right_x < 0):
                self.top_cam_LR = self.dec(self.top_cam_LR, self.top_camera_step, 0)

        self.camera_lr_pub.publish(self.top_cam_LR)        


    def inc(self, val, step, max_val):
        return max_val if (val + step > max_val) else (val + step)

    def dec(self, val, step, min_val):
        return min_val if (val - step < min_val) else (val - step)        


    def run(self):
        rospy.loginfo("Start JoyBro ANTI Teleop")
        rospy.spin()


if __name__ == '__main__':
    teleop = ANtiTeleop()
    teleop.run();