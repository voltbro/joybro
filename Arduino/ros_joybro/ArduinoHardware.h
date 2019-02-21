
#ifndef ROS_ARDUINO_HARDWARE_H_
#define ROS_ARDUINO_HARDWARE_H_

#if ARDUINO>=100
  #include <Arduino.h>  // Arduino 1.0
#else
  #include <WProgram.h>  // Arduino 0022
#endif

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__) || defined(__MKL26Z64__)
  #if defined(USE_TEENSY_HW_SERIAL)
    #define SERIAL_CLASS HardwareSerial // Teensy HW Serial
  #else
    #include <usb_serial.h>  // Teensy 3.0 and 3.1
    #define SERIAL_CLASS usb_serial_class
  #endif
#elif defined(_SAM3XA_)
  #include <UARTClass.h>  // Arduino Due
  #define SERIAL_CLASS UARTClass
#elif defined(USE_USBCON)
  // Arduino Leonardo USB Serial Port
  #define SERIAL_CLASS Serial_
#elif (defined(__STM32F1__) and !(defined(USE_STM32_HW_SERIAL))) or defined(SPARK) 
  // Stm32duino Maple mini USB Serial Port
  #define SERIAL_CLASS USBSerial
#else 
  #include <HardwareSerial.h>  // Arduino AVR
  #define SERIAL_CLASS HardwareSerial
#endif

class ArduinoHardware {
  public:
    ArduinoHardware(SERIAL_CLASS* io , long baud=57600){
      iostream = io;
      baud_ = baud;
    }
    ArduinoHardware()
    {
#if defined(USBCON) and !(defined(USE_USBCON))
      /* Leonardo support */
      iostream = &Serial1;
#elif defined(USE_TEENSY_HW_SERIAL) or defined(USE_STM32_HW_SERIAL)
      iostream = &Serial1;
#else
      iostream = &Serial;
#endif
      baud_ = 57600;
    }
    ArduinoHardware(ArduinoHardware& h){
      this->iostream = h.iostream;
      this->baud_ = h.baud_;
    }
  
    void setBaud(long baud){
      this->baud_= baud;
    }
  
    int getBaud(){return baud_;}

    void init(){
#if defined(USE_USBCON)
      // Startup delay as a fail-safe to upload a new sketch
      delay(3000); 
#endif
      iostream->begin(baud_);
    }

    int read(){return iostream->read();};
    void write(uint8_t* data, int length){
      for(int i=0; i<length; i++)
        iostream->write(data[i]);
    }

    unsigned long time(){return millis();}

  protected:
    SERIAL_CLASS* iostream;
    long baud_;
};

#endif
