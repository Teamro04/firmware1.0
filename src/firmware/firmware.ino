#define IR1 12
#define IR2 11
#define IR3 10
#define ena 9 
#define enb 8
#define in1 7 
#define in2 23
#define in3 24
#define in4 25
#define ultrasonic1_echo 26 // To be installed in the front
#define ultrasonic1_trigger 27 

#define ultrasonic2_echo 28 // To be installed on the right side
#define ultrasonic2_trigger 29

#define ultrasonic3_echo 30 // To be installed on the left side
#define ultrasonic3_trigger 31

// Array to hold all the pins up there
uint8_t outputs[] = {IR1,IR2,IR3,ena,enb,in1,in2,in3,in4,ultrasonic1_trigger,ultrasonic2_trigger,ultrasonic3_trigger}; // Output pins
uint8_t inputs[] = {ultrasonic1_echo,ultrasonic2_echo,ultrasonic3_echo}; // Input pins
void setup(){
  for(int pin = 0; pin<12; pin++){
    pinMode(outputs[pin],OUTPUT);
  }
  for(int i = 0; i<3; i++){
    pinMode(inputs[i],INPUT);
  }
};
void loop(){
  /*
    TODO: 
    Maze solving
    Obstacle Avoidance 
    Pause and Resume
  */
  float front_distance = distance(ultrasonic1_echo,ultrasonic1_trigger);
  float right_distance = distance(ultrasonic2_echo,ultrasonic2_trigger);
  float left_distance = distance(ultrasonic3_echo,ultrasonic3_trigger);
  float speed = 50;
  do{
    forward(90);
  }while(front_distance > 3);
  
  if(front_distance < 3){

    if(right_distance <5){

      left(50);
      delay(1000);

      do{
        forward(90);
      }while(front_distance > 3);

    }
    else if(left_distance < 5){

      right(50);
      delay(1000);

      do{
        forward(90);
      }while(front_distance > 3);
    
    }
  }
};


void motor_on(uint8_t enablePin){
  // Full speed
  digitalWrite(enablePin,HIGH);
};

void motor_off(uint8_t enablePin){
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

void forward(float speed){
  // Speed in percentage [0->100]
  float value = map(speed,0,100,0,255);
  analogWrite(ena,value);
  analogWrite(enb,value);

  // Mechanics   
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
  
  
};

void backward(float speed){
  // Speed in percentage [0->100]
  float value = map(speed,0,100,0,255);
  analogWrite(ena,value);
  analogWrite(enb,value);
  // Mechanics   
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  
};

void right(float speed){
  float value = map(speed,0,100,0,255);

  analogWrite(ena,value);
  analogWrite(enb,value);

  //Differential steering
  digitalWrite(in1,LOW); // First motor back
  digitalWrite(in2,HIGH);

  digitalWrite(in3,HIGH); // Second Motor Front
  digitalWrite(in4,LOW);
};

void left(float speed){
  float value = map(speed,0,100,0,255);
  analogWrite(ena,value);
  analogWrite(enb,value);

  //Differential steering
  digitalWrite(in1,HIGH); // First motor front
  digitalWrite(in2,LOW);

  digitalWrite(in3,LOW); // Second Motor back
  digitalWrite(in4,HIGH);
};

void U_turn_Right(uint8_t speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(ena);

  digitalWrite(in3,LOW); // Left Motor back
  digitalWrite(in4,HIGH);  
  
};

void U_turn_Left(float speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(in3);
  motor_off(in4);

  digitalWrite(in1,LOW); // Second Motor back
  digitalWrite(in2,HIGH);
};
// Read distance from ultrasonic sensor 
float distance(uint8_t echo,uint8_t trigger){

  digitalWrite(trigger,LOW);
  delayMicroseconds(10);

  digitalWrite(trigger,HIGH);

  long dr = pulseIn(echo,HIGH);
  long distance = dr*0.034/2;

  return distance;
};

void isOff(uint8_t pin){
  if(digitalRead(pin)==LOW){
    return true;
  }else{
    return false;
  }
};

void isOn(uint8_t pin){
  if(digitalRead(pin)==HIGH){
    return true;
  }else{
    return false;
  }
};

void PauseAndResume(){
  robot_off();
  delay(3000);
  robot_on();
};