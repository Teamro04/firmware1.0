#ifndef L298N_H
#define L298N_H
#include <Arduino.h>


class L298N{
    public:
        int enablePin;
        int in1;
        int in2;
        int motorSpeed;
    public:
        L298N(int a,int b,int c);
        void attach(int);
        void on();
        void off();
        void forward(float);
        void backward(float);
        float setSpeed(float n);
        float readSpeed();
        void left();
        void right();
        bool isOn();
        bool isOff();
        void speed(int);

};
#endif
