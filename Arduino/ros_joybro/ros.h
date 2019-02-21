#ifndef _ROS_H_
#define _ROS_H_


#include <ArduinoHardware.h>
#include <ros/node_handle.h>

namespace ros
{
  // default is 25, 25, 512, 512  
  typedef NodeHandle_<ArduinoHardware, 3, 3, 512, 512> NodeHandle;

  // This is legal too and will use the default 25, 25, 512, 512
  //typedef NodeHandle_<ArduinoHardware> NodeHandle;
}

#endif
