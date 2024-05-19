#include <Servo.h>
#include "Obstacle.h"
#include "States.h"

// Define the motor control pins
#define ENA 9
#define IN1 8
#define IN2 7

// Define buttons pin
#define B_FORWARD 4
#define B_BACKWARD 3
#define B_OBS 2

//left door
Servo l_door_servo;
Obstacle l_door(l_door_servo);

//right door
Servo r_door_servo;
Obstacle r_door(r_door_servo);

//obstacle button
int buttonObsState = LOW;
int buttonObsLastState = LOW;
bool buttonObs = false;

//motor forward button
int buttonForwState = LOW;
int buttonForwLastState = LOW;

//motor backward button
int buttonBackState = LOW;
int buttonBackLastState = LOW;

//motor state
MotorState motorState = STOP;

//detection timer
unsigned long detection_timer = 0;
const unsigned long detection_interval = 500;

void setup() {
  //put your setup code here, to run once:
  l_door.init(11, A2, 60, 0);
  r_door.init(10, A2, 0, 60);

  // Set the buttons pin as outputs
  pinMode(B_OBS, INPUT);
  pinMode(B_FORWARD, INPUT);
  pinMode(B_BACKWARD, INPUT);

  // Set the motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  //serial monitor set up
  Serial.begin(9600);
  delay(100);
}

void loop() {
  // check button
  buttonObsState = digitalRead(B_OBS);
  buttonForwState = digitalRead(B_FORWARD);
  buttonBackState = digitalRead(B_BACKWARD);

  // check if the button is pressed
  if (buttonObsLastState != buttonObsState){
    if (buttonObsState == HIGH){
      buttonObs = true;
    }
  }else{
    buttonObs = false;
  }

  if (buttonForwLastState != buttonForwState){
    if (buttonForwState == HIGH){
      motorState = FORWARD;
    }
  }

  if (buttonBackLastState != buttonBackState){
    if (buttonBackState == HIGH){
      motorState = BACKWARD;
    }
  }

  //update buttons state
  buttonObsLastState = buttonObsState;
  buttonForwLastState = buttonForwState;
  buttonBackLastState = buttonBackState;

  //update the obstacles
  bool l_door_update = l_door.update(buttonObs);
  bool r_door_update = r_door.update(buttonObs);

  //update the detection timer to see if player is detected > 100ms
  if (l_door_update || r_door_update) {
    if( detection_timer == 0){
      detection_timer = millis();
    }
  }else{
    detection_timer = 0;
  }

  //update the motors
  //stop motors if player is detected for more than 100ms
  if(millis() - detection_timer < detection_interval){
    motorState = STOP;
  }

  switch (motorState) {
    case FORWARD:
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 255);
      break;
    case BACKWARD:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, 255);
      break;
    case STOP:
    default:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 0);  
      break;
  }
  
}
