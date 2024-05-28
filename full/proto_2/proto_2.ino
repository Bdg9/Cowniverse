#include <Servo.h>
#include "Obstacle.h"
#include "States.h"
//librairies audio
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

// Define the motor control pins
#define ENA 9
#define IN1 8
#define IN2 7
#define SPEED 110

// Define buttons pin
#define B_FORWARD 4
#define B_BACKWARD 3
#define B_OBS 2

//init audio
SoftwareSerial mySerial(19, 18); // RX, TX
// Initialiser DFPlayer
DFPlayerMini_Fast myDFPlayer;

//left door
Servo bar_1_servo;
Obstacle bar_1(bar_1_servo);

//right door
Servo bar_2_servo;
Obstacle bar_2(bar_2_servo);

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
  Serial.begin(9600);
  mySerial.begin(9600); //begin serial for audio

  bar_1.init(11, A1, A0, 70, 165);
  bar_2.init(10, A3, A2, 90, 10);

  // Set the buttons pin as outputs
  pinMode(B_OBS, INPUT);
  pinMode(B_FORWARD, INPUT);
  pinMode(B_BACKWARD, INPUT);

  // Set the motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //setup audio
  // Initialisation du DFPlayer
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Impossible de communiquer avec le DFPlayer Mini !");
    while (true);
  }

  Serial.println("DFPlayer Mini prêt !");

  myDFPlayer.volume(5);  // (0-30)
  

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
      //myDFPlayer.play(4);
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
  bool bar_1_update = bar_1.update(buttonObs, motorState);
  bool bar_2_update = bar_2.update(buttonObs, motorState);

  //update the motors
  //stop motors if player is detected and the obstacle is closed
  if((bar_1_update && !bar_1.get_state()) || (bar_2_update && !bar_2.get_state())){//(millis() - detection_timer < detection_interval){
    motorState = STOP;
    //myDFPlayer.play(8);
  }

  switch (motorState) {
    case FORWARD:
      analogWrite(ENA, SPEED);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      break;
    case BACKWARD:
      analogWrite(ENA, SPEED);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      break;
    case STOP:
    default:
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      break;
  }
  
}
