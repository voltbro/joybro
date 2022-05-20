
#include <Servo.h>

Servo myservo;  

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

int pos = 90;   
#define servopin 46

void setup() {
  myservo.attach(servopin); 
  Serial.begin(9600);
  Serial.println();
  Serial.println("Введите значение от 0 до 180. Сервопривод " + String(servopin) + " займет соответсвующую позицию.");
  Serial.println("При загрузке позиция 90. Номер канала серво меняйте в файле над void setup() ");
    // reserve 200 bytes for the inputString:
  inputString.reserve(200);

}

/*
 * Серво полива (44) вверхний максимум 120
 * Серво полива (44) нижний максиум 0
 * Серво полива поворот (45) крайнее правое 0
 * Серво полива поворот (45) крайнее левое 150
 * Серво забора жидкости (46) крайнее поднятое 180
 * Серво забора жидкости (46) крайнее опущенное 0
 * Серво верхней камеры (9) верхний максимум 
 * Серво верхней камеры (9) нижний максимум 
 * 
 */

void loop() {
 
    myservo.write(pos);    
    if (stringComplete) {
      pos = inputString.toInt();  
      if ( pos < 0 ) pos = 0;
      if ( pos > 180 ) pos = 180;
      myservo.write(pos);
      Serial.println(pos);
      inputString = "";
      stringComplete = false;
  } 
  
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
