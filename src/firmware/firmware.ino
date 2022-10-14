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

