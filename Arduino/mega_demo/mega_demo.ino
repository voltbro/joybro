#include <ros.h>
#include <joybro/JoyBro.h>

class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};

ros::NodeHandle_<NewHardware>  nh;


void messageCb( const joybro::JoyBro& data){

}

ros::Subscriber<joybro::JoyBro> sub("joybro", &messageCb );

void setup() {

  nh.initNode();
  nh.subscribe(sub);
}


void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(1);

}
