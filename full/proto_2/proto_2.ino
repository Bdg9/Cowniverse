#include <Servo.h>
#include "Obstacle.h"
#include "States.h"
#include "House.h"
//librairies audio
#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

// Define the motor control pins
#define ENA 9
#define IN1 8
#define IN2 7
#define SPEED 90

// Define buttons pin
#define B_FORWARD 4
#define B_BACKWARD 3
#define B_OBS 2

// Define servos pin
#define BAR_1 11
#define BAR_2 10
#define HOUSE_L 13
#define HOUSE_R 12 

//debug prints
bool verbose = false;

//init audio
SoftwareSerial mySerial(14, 15); // RX, TX
// Initialiser DFPlayer
DFPlayerMini_Fast myDFPlayer;

//left door
Servo bar_1_servo;
Obstacle bar_1(bar_1_servo);

//right door
Servo bar_2_servo;
Obstacle bar_2(bar_2_servo);

//house
Servo house_1;
Servo house_2;
House house(house_1, house_2);

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
Sound sound = NONE;

//detection timer
unsigned long detection_timer = 0;
const unsigned long detection_interval = 500;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); //begin serial for audio

  //init gates
  bar_1.init(BAR_1, A1, A0, 70, 165);
  bar_2.init(BAR_2, A3, A2, 90, 10);

  //init house
  house.init(HOUSE_L, HOUSE_R, A4, 60, 0, 8, 60);

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

  //test sensor
  if( (bar_1.get_front_sensor() > THRESHOLD) ||  (bar_1.get_back_sensor() > THRESHOLD) || (bar_2.get_front_sensor() > THRESHOLD) ||
      (bar_2.get_back_sensor() > THRESHOLD) || (house.get_sensor() > THRESHOLD)){
    Serial.println("ERROR : sensor obstructed");
  }
  

  delay(100);
}

void loop() {
  
  // check button
  /*
  buttonObsState = digitalRead(B_OBS);
  buttonForwState = digitalRead(B_FORWARD);
  buttonBackState = digitalRead(B_BACKWARD);
  */

  //use serial instead of physical buttons
  if (Serial.available() > 0) {
    String receivedText = Serial.readStringUntil('\n'); // Read the incoming data until newline character

    receivedText.trim(); // Remove any leading/trailing whitespace characters
    
    if (receivedText == "f") {
      buttonForwState = HIGH;
    } else if (receivedText == "b") {
      buttonBackState = HIGH;
    } else if (receivedText == "o") {
      buttonObsState = HIGH;
    } else {
      Serial.println("Unknown command: " + receivedText);
    }
  }else{
    buttonObsState = LOW;
    buttonForwState = LOW;
    buttonBackState = LOW;
  }

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
      sound = YAY;
    }
  }

  //update buttons state
  buttonObsLastState = buttonObsState;
  buttonForwLastState = buttonForwState;
  buttonBackLastState = buttonBackState;


  //update the obstacles
  bool bar_1_update = bar_1.update(buttonObs, motorState);
  bool bar_2_update = bar_2.update(buttonObs, motorState);
  bool house_update = house.update(buttonObs, motorState);

  if (verbose){
    Serial.println(house.get_state());
  }

  //update the motors
  //stop motors if player is detected and the obstacle is closed
  if ((bar_1_update && !bar_1.get_state()) || (bar_2_update && !bar_2.get_state()) || (house_update && !house.get_state()) || (!house.get_state())){
    motorState = STOP;
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

   switch (sound){
    case MARLEY:
      myDFPlayer.play(0);
      sound = NONE;
      break;
    case NARUTO:
      myDFPlayer.play(1);
      sound = NONE;
      break;
    case POULE:
      myDFPlayer.play(2);
      sound = NONE;
      break;
    case PIG:
      myDFPlayer.play(5);
      sound = NONE;
      break;
    case COW:
      myDFPlayer.play(6);
      sound = NONE;
      break;
    case YAY:
      myDFPlayer.play(2);
      sound = NONE;
      Serial.println("yay");
      break;
    case BRAKE:
      myDFPlayer.play(8);
      sound = NONE;
      break;
    case NONE:
      break;
  }
  
}
