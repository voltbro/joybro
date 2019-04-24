#include <Servo.h>
#include <ros.h>
#include <joybro/JoyBro.h>

class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};

ros::NodeHandle_<NewHardware>  nh;

Servo servo44;
Servo servo45;
Servo servo46;

//joybro::JoyBro joy_msg;

void messageCb( const joybro::JoyBro& data){

    servo44.write(int((data.slider1/6)+3));
    servo45.write(int((data.slider2/6)+3));
    servo46.write(int(((data.right_y+512)/6)+3));
}

ros::Subscriber<joybro::JoyBro> sub("joybro", &messageCb );

void setup() {

  nh.initNode();
  nh.subscribe(sub);
  
  //Serial.begin(9600);
  // put your setup code here, to run once:
  servo44.attach(44);servo44.write(90);
  servo45.attach(45);servo45.write(90);
  servo46.attach(46);servo46.write(90);

  

}


void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  //servo_test.write(180);
  delay(1);

}
