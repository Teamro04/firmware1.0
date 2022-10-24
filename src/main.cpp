#include <Arduino.h>
#include <l298n.h>

L298N motor1(6,8,9);
L298N motor2(7,10,11);

void setup(){
  Serial.begin(9600);
}

void loop(){
  motor1.forward(100);
  motor2.forward(100);
  delay(1000);
  motor1.off();
  motor2.off();
  delay(1000);
    // backward
  motor1.backward(50);
  motor2.backward(50);
  delay(1000);
  motor1.off();
  motor2.off();
}
