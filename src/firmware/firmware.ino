// INTELLIGENT ROBOT CHALLENGE 

#define IR1 12 // Right IR sensor
#define IR2 11 // Left IR sensor
#define IR3 10 // Middle IR sensor , (May not be installed)

#define ena 9  // Enable pin for the right motor pair
#define enb 8 // Enable pin for the left motor pair

#define in1 7 // Input 1 for the right motor
#define in2 23 // Input 2 for the right motor

#define in3 24 // Input 1 for the left motor 
#define in4 25 // Input 2 for the left motor

#define ultrasonic1_echo 26 // To be installed in the front
#define ultrasonic1_trigger 27 

#define ultrasonic2_echo 28 // To be installed on the right side
#define ultrasonic2_trigger 29

#define ultrasonic3_echo 30 // To be installed on the left side
#define ultrasonic3_trigger 31

#define motorspeed  75 // Default motor speed in percentage

#define limit_fr 5 // Distance limit for the front ultrasonic sensor 
#define limit_sd 2 // Distance limit for the right and left ultrasonic sensors

#define front_distance distance(ultrasonic1_echo,ultrasonic1_trigger)
#define left_distance distance(ultrasonic2_echo,ultrasonic2_trigger)
#define right_distance distance(ultrasonic3_echo,ultrasonic3_trigger)

// Arrays to hold all the pins 
uint8_t outputs[] = {IR1,IR2,IR3,ena,enb,in1,in2,in3,in4,ultrasonic1_trigger,ultrasonic2_trigger,ultrasonic3_trigger}; // Output pins
uint8_t inputs[] = {ultrasonic1_echo,ultrasonic2_echo,ultrasonic3_echo}; // Input pins

// Function prototypes
float distance(uint8_t echo,uint8_t trigger);
void motor_on(uint8_t enablePin);
void motor_off(uint8_t enablePin);
bool isOn(uint8_t enablePin);
bool isOff(uint8_t enablePin);
void forward(float speed);
void backward(float speed);
void right(float speed);
void left(float speed);
void robot_off();
void robot_on();
void UTurn_Right(float speed);
void UTurn_Left(float speed);
void PauseAndResume();
void followLine();
void brake();

void setup()
{
  for(int pin = 0; pin<12; pin++) // Output pin array
  { 
    pinMode(outputs[pin],OUTPUT);
  }
  for(int i = 0; i<3; i++) // Input pin array
  { 
    pinMode(inputs[i],INPUT);
  }
}


void loop(){



  /*
    TODO: 
    Maze solving
    Obstacle Avoidance 
    Pause and Resume
  */

    // Ensure that the motors are on before starting the challenge 
  if(isOn(ena) && isOn(enb))
  {
    if(front_distance > limit_fr){ // Move forwards by default
      forward(75);      
    }else{ 
      // CASE 1: Corner situation 

      /*
        Front distance would be:
          1 Something slightly above the set limit distance from the wall 
          2 At the set limit distance from the wall       
          3 Something slightly below the distance
      */

      /*
        Side distances would be:
        1 At a distance equal to the limit 
        2 At a distance lower than the limit
        3 At a distance higher than the limit
      */

      if((front_distance == limit_fr) || (front_distance-limit_fr <2) || (limit_fr-front_distance <2) ) // Check the three test cases above
      {
        // Proceed to check the right and left 
        if(right_distance > left_distance){
          right(motorspeed); // Turn right
        }

        else
         if(left_distance >right_distance)
        { 
          left(motorspeed); // Turn left
        }
        
      }
      
      // Middle Align inside maze 
      /*
        Check if the distance between the left and right sensors is less than δ/2:
        δ = (width of maze-width of robot)
      */

      if
      (right_distance == limit_sd)
      {
        do{ 
          left(25);
        }while(right_distance != left_distance);
      }

      if
      ((left_distance == limit_sd))
      {
        do{ 
          right(25);
        }while(right_distance != left_distance); 
      }

  }}

  else{

    if(isOff(ena)){
      motor_on(ena);  
    }

    if(isOff(enb)){
      motor_on(enb);
    }

  }
}


void motor_on(uint8_t enablePin){
  // Full speed
  digitalWrite(enablePin,HIGH);
}

void motor_off(uint8_t enablePin){
  //Turn off motor
  digitalWrite(enablePin,LOW);
}

void robot_on(){
  motor_on(ena);
  motor_on(enb);
}

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
  
  
}

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
  
}

void right(float speed){
  float value = map(speed,0,100,0,255);

  analogWrite(ena,value);
  analogWrite(enb,value);

  //Differential steering
  digitalWrite(in1,LOW); // First motor back
  digitalWrite(in2,HIGH);

  digitalWrite(in3,HIGH); // Second Motor Front
  digitalWrite(in4,LOW);
}

void left(float speed){
  float value = map(speed,0,100,0,255);
  analogWrite(ena,value);
  analogWrite(enb,value);

  //Differential steering
  digitalWrite(in1,HIGH); // First motor front
  digitalWrite(in2,LOW);

  digitalWrite(in3,LOW); // Second Motor back
  digitalWrite(in4,HIGH);
}

void UTurn_Right(float speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(ena);

  digitalWrite(in3,LOW); // Left Motor back
  digitalWrite(in4,HIGH);  
  
}

void UTurn_Left(float speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(in3);
  motor_off(in4);

  digitalWrite(in1,LOW); // Second Motor back
  digitalWrite(in2,HIGH);
}
// Read distance from ultrasonic sensor 
float distance(uint8_t echo,uint8_t trigger){

  digitalWrite(trigger,LOW);
  delayMicroseconds(10);

  digitalWrite(trigger,HIGH);

  long dr = pulseIn(echo,HIGH);
  long distance = dr*0.034/2;

  return distance;
}

bool isOff(uint8_t pin){
  if(digitalRead(pin)==LOW){
    return true;
  }else{
    return false;
  }
}

bool isOn(uint8_t pin){
  if(digitalRead(pin)==HIGH){
    return true;
  }else{
    return false;
  }
}

void PauseAndResume(){
  robot_off();
  delay(3000);
  robot_on();
}
void brake(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);  
}

void followLine(){
  int ir1 = digitalRead(IR1); // Right IR sensor
  int ir2 = digitalRead(IR2); // Left IR sensor
  if(ir1 ==0 && ir2 == 0){
    forward(75);
    
  }
  else if(ir1 == 1 && ir2 == 0){
    right(75);
  }
  else if(ir1 == 0 && ir2 == 1){
    left(75);    
  }
  else if(ir1==1 && ir2==1){
    PauseAndResume();
  }
}