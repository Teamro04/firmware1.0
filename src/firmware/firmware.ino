#define IR1 
#define IR2
#define IR3
#define ena
#define enb
#define in1
#define in2
#define in3
#define in4
#define ultrasonic1
#define ultrasonic2
#define ultrasonic3

void setup(){

};
void loop(){

};

void motor_on(int enablePin){
  // Full speed
  digitalWrite(enablePin,HIGH);
};

void motor_off(int enablePin){
  //Turn off motor
  digitalWrite(enablePin,LOW);
};

void robot_on(){
  motor_on(ena);
  motor_on(enb);
};

void robot_off(){
  motor_off(ena);
  motor_off(enb);
}

void forward(enablePin,speed){
  // Speed in percentage [0->100]
  float value = map(speed,0,100,0,255);
  analogWrite(enablePin,value);

  // Mechanics   
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  
};

void backward(enablePin,speed){
  // Speed in percentage [0->100]
  float value = map(speed,0,100,0,255);
  analogWrite(enablePin,value);

  // Mechanics   
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  
};

void right(enablePin,int speed){
  float value = map(speed,0,100,0,255);
  analogWrite(enablePin,value);
  //Differential steering
  digitalWrite(in1,LOW); // First motor back
  digitalWrite(in2,HIGH);

  digitalWrite(in3,HIGH); // Second Motor Front
  digitalWrite(in4,LOW);
};

void left(int speed){
  void right(enablePin,int speed){
  float value = map(speed,0,100,0,255);
  analogWrite(enablePin,value);
  //Differential steering
  digitalWrite(in1,HIGH); // First motor back
  digitalWrite(in2,LOW);

  digitalWrite(in3,LOW); // Second Motor Front
  digitalWrite(in4,HIGH);
};

void U_turn_Right(int speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(in1);
  motor_off(in2);
  backward(75);
};

void U_turn_Left(int speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(in3);
  motor_off(in4);
  backward(75);
};
// Read distance from ultrasonic sensor 
float distance(int echo,int trigger){

  digitalWrite(trigger,LOW);
  delayMicroseconds(10);

  digitalWrite(trigger,HIGH);

  long dr = pulseIn(echo,HIGH);
  long distance = dr*0.034/2;

  return distance;
};


