// AUTONOMOUS ROBOT CHALLENGE 
#define IR1 28 // Right IR sensor
#define IR2 30 // Left IR sensor
#define IR3 2 // Middle IR sensor , (May not be installed)

#define ena 13  // Enable pin for the right motor pair
#define enb 8 // Enable pin for the left motor pair

#define in1 29 // Input 1 for the right motor
#define in2 23 // Input 2 for the right motor

#define in3 10 // Input 1 for the left motor 
#define in4 9 // Input 2 for the left motor

#define ultrasonic1_echo 47 // To be installed in the front
#define ultrasonic1_trigger 46 

#define ultrasonic2_echo 39 // To be installed on the right side
#define ultrasonic2_trigger 38

#define ultrasonic3_echo 53 // To be installed on the left side
#define ultrasonic3_trigger 52

#define motorspeed  90 // Default motor speed in percentage %

#define limit_fr 5 // Distance limit for the front ultrasonic sensor 
#define limit_sd 5 // Distance limit for the right and left ultrasonic sensors

#define front_distance distance(ultrasonic1_echo,ultrasonic1_trigger) // Front ultrasonic sensor
#define left_distance  distance(ultrasonic2_echo,ultrasonic2_trigger) // Left ultrasonic sensor
#define right_distance distance(ultrasonic3_echo,ultrasonic3_trigger) // Right ultrasonic sensor

// Arrays to hold all the pins 

uint8_t outputs[] = {IR1,IR2,IR3,ena,enb,in1,in2,in3,in4,ultrasonic1_trigger,ultrasonic2_trigger,ultrasonic3_trigger}; // Output pins
uint8_t inputs[] = {ultrasonic1_echo,ultrasonic2_echo,ultrasonic3_echo}; // Input pins

// Position of the robot ( May not be necessary in the long run )
#define position NULL

// Function Declaration: For some reason this algorithm can't work without function prototypes
double distance(uint8_t echo,uint8_t trigger); // Function to read distance from ultrasonic sensors
void motor_on(uint8_t enablePin); // Turn a specific motor on
void motor_off(uint8_t enablePin); // Turn a specific motor off
bool isOn(uint8_t enablePin); // Check if a pin is on
bool isOff(uint8_t enablePin); // Check if a pin is off
void forward(float speed); // Move forward
void backward(float speed); // Move backwards
void right(float speed); // Move right using differential thrust method
void left(float speed); // Move left using differential thrust method
void robot_off(); // Turn the motors off at once
void robot_on(); // Turn the motors on at once
void UTurn_Right(float speed); // Right U Turn
void UTurn_Left(float speed); // Left U Turn 
void PauseAndResume(); // Pause for 3 seconds
void followLine(); // Line following functionality
void brake(); // Brake the robot
bool blackLineDetected(); // Return true if any of the IR sensors detects a black line so as to trigger the followLine() function
void mazeSolve(float fr, float lf,float rg);
void setup()
{
  // Loop through the output pins array 
  for(int pin = 0; pin<12; pin++) // Output pin array
  { 
    pinMode(outputs[pin],OUTPUT); // Set pinmode to output
  }
  // Loop through the input pins array
  for(int i = 0; i<3; i++) // Input pin array
  { 

    pinMode(inputs[i],INPUT); // Set pinmode to input
  }

  robot_on(); // Turn the robot on
}


void loop(){
  
  /*
    TODO: 
    Maze solving
    Obstacle Avoidance 
    Pause and Resume
  */
  double fr = distance(ultrasonic1_echo,ultrasonic1_trigger);
  double rg = distance(ultrasonic2_echo,ultrasonic2_trigger);
  double lf = distance(ultrasonic3_echo,ultrasonic3_trigger);
  Serial.begin(9600);
  Serial.print("Front ");
  Serial.println(fr);
  Serial.print("Right ");
  Serial.println(rg);
  Serial.print("Left ");
  Serial.println(lf);
  Serial.print("IR 1 ");
  int ir1_val = digitalRead(IR1);
  int ir2_val = digitalRead(IR2);
  Serial.println(ir1_val);
  Serial.print("IR 2 ");
  Serial.println(ir2_val);

  forward(100);
  delay(1000);
  backward(100);


  if(blackLineDetected()){
    if(isOn(ena) && isOn(enb)){
    followLine();
    }else{
      robot_on();
    }
  }else{
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

      if((front_distance == limit_fr) || (front_distance-limit_fr <3) || (limit_fr-front_distance <3) ) // Check the three test cases above
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

      if((right_distance+left_distance+9.8) <=710){
        // Robot in maze
        bool maze = true;
        do{
          mazeSolve(front_distance,left_distance,right_distance);
        }while(maze);
      }
     
    }
  }else{

    if(isOff(ena)){
      motor_on(ena);  
    }

    if(isOff(enb)){
      motor_on(enb);
      }
    }
  }
}

// Turn on a Motor
void motor_on(uint8_t enablePin){
  // Full speed
  digitalWrite(enablePin,HIGH);
}
// Turn off a motor
void motor_off(uint8_t enablePin){
  //Turn off motor
  digitalWrite(enablePin,LOW);
}
// Turn on the whole robot
void robot_on(){
  motor_on(ena);
  motor_on(enb);
}
// Turn off the whole robot
void robot_off(){
  motor_off(ena);
  motor_off(enb);
}

// MECHANICS

// Move forward
void forward(float speed){
  // Speed in percentage [0->100]
  float value = map(speed,0,100,0,255);

  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);

  analogWrite(ena,value);
  analogWrite(enb,value);
  
}

// Move backwards
void backward(float speed){
  // Speed in percentage [0->100]
  float value = map(speed,0,100,0,255);

  // Mechanics   
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);

  analogWrite(ena,value);
  analogWrite(enb,value);  
}
// Differential steering 
// Move right
void right(float speed){
  float value = map(speed,0,100,0,255);


  digitalWrite(in1,LOW); // First motor back
  digitalWrite(in2,HIGH);

  digitalWrite(in3,HIGH); // Second Motor Front
  digitalWrite(in4,LOW);

  analogWrite(ena,value);
  analogWrite(enb,value);

}
// Move right
void left(float speed){
  float value = map(speed,0,100,0,255);

  digitalWrite(in1,HIGH); // First motor front
  digitalWrite(in2,LOW);

  digitalWrite(in3,LOW); // Second Motor back
  digitalWrite(in4,HIGH);

  analogWrite(ena,value);
  analogWrite(enb,value);
  
}
//  U turn Right
void UTurn_Right(float speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(ena);
  digitalWrite(in3,LOW); // Left Motor back
  digitalWrite(in4,HIGH);   

  analogWrite(enb,value);
  
}
//  U turn left
void UTurn_Left(float speed){
  float value = map(speed,0,100,0,255);
  // Assuming that in1 and in2 are the right motors and in3 and in4 being left motors
  motor_off(enb);
  digitalWrite(in1,LOW); // Second Motor back
  digitalWrite(in2,HIGH);

  analogWrite(enb,value);
  
}
// Read distance from ultrasonic sensor 
double distance(uint8_t echo,uint8_t trigger){
  digitalWrite(trigger,LOW);
  delayMicroseconds(1);
  digitalWrite(trigger,HIGH);
  long dr = pulseIn(echo,HIGH);
  long distance = dr*0.034/2;
  return distance;
}
// Check if a pin is off
bool isOff(uint8_t pin){
  if(digitalRead(pin)==LOW){
    return true;
  }else{
    return false;
  }
}
// Check if a pin is on
bool isOn(uint8_t pin){
  if(digitalRead(pin)==HIGH){
    return true;
  }else{
    return false;
  }
}
// Off for 3 seconds then on again
void PauseAndResume(){
  robot_off();
  delay(3000);
  robot_on();
}
// Brake the wheels
void brake(){
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);  
}
// Line following function
void followLine(){
  int ir1 = digitalRead(IR1); // Right IR sensor
  int ir2 = digitalRead(IR2); // Left IR sensor
  // If no line detected, Move forward  
  if(ir1 ==1 && ir2 == 1){
    forward(75);
    if(front_distance == limit_fr) 
    { 
     // Obstacle detected while following a line 
      if(right_distance > left_distance){
        // Turn right 
        right(25);
        delay(500);
        left(50);
        delay(500);
        // Continue moving forward
        forward(75);
      }
      else if(left_distance>right_distance){
        left(25);
        delay(500);
        right(50);
        delay(500);
        // Continue moving forward
        forward(75);              
      }
    }
  }
  // If right IR detects black line, turn right
  else if(ir1 == LOW && ir2 == HIGH){
    right(75);
  }
  // If left IR detects black line turn left
  else if(ir1 == HIGH && ir2 == LOW){
    left(75);    
  }
  // If both IR sensors are high, We've possibly reached the end of the maze
  else if(ir1 == LOW && ir2 == LOW){
    PauseAndResume(); // Pause for 3 seconds
  }
}
// Check if any of the sensors detects a black line so as to trigger the line following functionality 
bool blackLineDetected(){
  int ir1 = digitalRead(IR1); // Right IR sensor
  int ir2 = digitalRead(IR2); // Left IR sensor
  if(ir1 == HIGH || ir2 == HIGH){ 
    return true;
  }else{
    return false; 
  }
}
// Maze solving function
void mazeSolve(float fr, float lf,float rg){
  if(fr>=limit_fr){
    // Check where there is more distance 
    if(rg>lf){
      right(50);
    }else if(rg<lf){
      left(50);      
    }else{
      // Recursive function
      mazeSolve( fr,  lf,  rg);      
    }
  }
   if(fr<limit_fr){
    if(rg>lf){
      right(50);
    }else if(rg<lf){
      left(50);
    }else{
      mazeSolve( fr,  lf,  rg);
    }
  }
}
