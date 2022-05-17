/*
 * Серво полива (44) вверхний максимум 155
 * Серво полива (44) нижний максиум 50
 * Серво полива поворот (45) крайнее правое 30
 * Серво полива поворот (45) крайнее левое 180
 * Серво забора жидкости (46) крайнее поднятое 180
 * Серво забора жидкости (46) крайнее опущенное 0
 * Серво верхней камеры (9) верхний максимум 
 * Серво верхней камеры (9) нижний максимум 
 * Серво верхней камеры (10) левый максимум 
 * Серво верхней камеры (10) провый максимум 
 * 
 */
 
#include <Servo.h>
#include <ros.h>
#include "std_msgs/Int16.h"
 
//arduino to ROS connect init
class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};
 
//ROS node init
ros::NodeHandle_<NewHardware>  nh;
 
//Servos and pins init
 
int poliv_ud_angle;
int poliv_lr_angle;
int zabor_ud_angle;
int camera_ud_angle;
 
int poliv_u_d_init = 155;
int poliv_l_r_init = 180;
int zabor_u_d_init = 180;
int camera_u_d_init = 70;
int camera_l_r_init = 100;
 
Servo Poliv_up_down;
Servo Poliv_left_right;
Servo Zabor_up_down;
Servo Camera_up_down;
Servo Camera_left_right;
 
//Pump_motor 
#define IN1 5  //pwm
#define IN2 12

//Flush_motor
#define IN3 6  //pwm
#define IN4 13
 
 
void pumpMotor(int m1) {  
 
  if (m1 == 0) {
    digitalWrite(IN1,0); 
    digitalWrite(IN2,0);
    } else {
    digitalWrite(IN1,0);
    digitalWrite(IN2,1);
  }
}

void flushMotor(int m2) {  
 
 
  if (m2 == 0) {
    digitalWrite(IN3,0);
    digitalWrite(IN4,0);
    } else {
    digitalWrite(IN3,1);
    digitalWrite(IN4,0);
  }
}
 
 
//RoverPN ROS topic read function
 
void CbPolivUD( const std_msgs::Int16& angle){
    poliv_ud_angle = angle.data;
    if (poliv_ud_angle > 155)
    {
      poliv_ud_angle = 155;
    }
    if (poliv_ud_angle < 50)
    {
      poliv_ud_angle = 50;
    }
    Poliv_up_down.write(poliv_ud_angle);
}
ros::Subscriber<std_msgs::Int16> subPolivUD("poliv_ud", &CbPolivUD );
 
 
void CbPolivLR( const std_msgs::Int16& angle){
    Poliv_left_right.write(angle.data);
}
ros::Subscriber<std_msgs::Int16> subPolivLR("poliv_lr", &CbPolivLR );
 
 
void CbZaborUD( const std_msgs::Int16& angle){
    Zabor_up_down.write(angle.data);
}
ros::Subscriber<std_msgs::Int16> subZaborUD("zabor_ud", &CbZaborUD );
 
 
void CbCameraUD( const std_msgs::Int16& angle){
    Camera_up_down.write(angle.data);
}
ros::Subscriber<std_msgs::Int16> subCameraUD("camera_ud", &CbCameraUD );

void CbCameraLR( const std_msgs::Int16& angle){
    Camera_left_right.write(angle.data);
}
ros::Subscriber<std_msgs::Int16> subCameraLR("camera_lr", &CbCameraLR );

void CbGetPump( const std_msgs::Int16& cmnd){
    pumpMotor(cmnd.data);
}
ros::Subscriber<std_msgs::Int16> subGetPump("get_pump", &CbGetPump );

void CbFlushPump( const std_msgs::Int16& cmnd){
    flushMotor(cmnd.data);
}
ros::Subscriber<std_msgs::Int16> subFlushPump("flush_pump", &CbFlushPump );

 
void setup() {
  //ROS nodes init
  nh.initNode();
  nh.subscribe(subPolivUD);
  nh.subscribe(subPolivLR);
  nh.subscribe(subZaborUD);
  nh.subscribe(subCameraUD);
  nh.subscribe(subCameraLR);
  nh.subscribe(subFlushPump);
  nh.subscribe(subGetPump);
  
  // Rotate to initial servos angles
  Poliv_up_down.attach(44);Poliv_up_down.write(poliv_u_d_init);
  Poliv_left_right.attach(45);Poliv_left_right.write(poliv_l_r_init);
  Zabor_up_down.attach(46);Zabor_up_down.write(zabor_u_d_init);
  Camera_up_down.attach(9);Camera_up_down.write(camera_u_d_init);
  Camera_left_right.attach(10);Camera_left_right.write(camera_l_r_init);
 
  // Water pump inits
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 
}
 
void loop() {
//ROS node spin
  nh.spinOnce();
  delay(1);
}