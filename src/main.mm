#include<Arduino.h>
#include "l298n.h"
#define ena 6
#define enb 7
#define in1 3
#define in2 4
#define in3 5
#define in4 8
L298N motor1(ena,in1,in2);
L298N motor2(enb,in3,in4);
void setup(){

}
void loop(){

}

// Mechanics
void forward(){
    motor1.forward(50);
    motor2.backward(50);
}
void backwards(){
    motor2.forward(50);
    motor1.backward(50);
}

