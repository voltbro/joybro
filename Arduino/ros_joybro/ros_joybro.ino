#define USE_USBCON
#define __AVR_ATmega168__ 1
#include "Arduino.h"
#include "ros.h" 
#include <joybro/JoyBro.h>


ros::NodeHandle nh;
joybro::JoyBro joy_msg;
ros::Publisher chatter("joybro", &joy_msg);

uint16_t left_zero_x,left_zero_y,right_zero_x,right_zero_y;

const byte NUMBER_OF_PINS = 15;
int inputPins[NUMBER_OF_PINS] = {A0, A1, A2, A3, A4, A5, 4, 5, 6, 7, 8, 9, 10, 11, 12};


void setup()
{
  for(int i=0; i < NUMBER_OF_PINS; i++){
      pinMode(inputPins[i], INPUT);
  }
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(chatter);
  
  left_zero_x = analogRead(A4);
  left_zero_y = analogRead(A3);

  right_zero_x = analogRead(9);
  right_zero_y = analogRead(10);
  
}

void loop()
{
  
  joy_msg.left_x = analogRead(A4) - left_zero_x;
  
  joy_msg.btn1 = digitalRead(A0);
  
  chatter.publish( &joy_msg );
  nh.spinOnce();
  delay(50);
}
