#include <Servo.h>
#include "Obstacle.h"
#include "States.h"
#include "House.h"
//librairies audio
#include <DFRobot_DF1201S.h>

// Define the motor control pins
#define ENA 6
#define IN1 8
#define IN2 7
#define SPEED 90

// Define buttons pin
#define B_FORWARD 32
#define B_BACKWARD 33
#define B_OBS 34
#define POT A10
#define MODE_1 2
#define MODE_2 3

// Define servos pin
#define BAR_1 11
#define BAR_2 10
#define HOUSE_L 13
#define HOUSE_R 12 

//debug prints
bool verbose = false;

// Initialiser DFPlayer
DFRobot_DF1201S DF1201S;
int vol;

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
bool bar_1_update = false;
bool bar_2_update = false;
bool house_update = false;

//game mode 
GameMode mode;

//detection timer
unsigned long detection_timer = 0;
const unsigned long detection_interval = 500;

void setup() {
  Serial.begin(9600);
  Serial3.begin(115200); //begin serial for audio

  //init gates
  bar_1.init(BAR_1, A1, A0, 70, 165);
  bar_2.init(BAR_2, A3, A2, 90, 10);

  //init house
  house.init(HOUSE_L, HOUSE_R, A4, 60, 0, 8, 60);

  // Set the buttons pin as outputs
  pinMode(B_OBS, INPUT);
  pinMode(B_FORWARD, INPUT);
  pinMode(B_BACKWARD, INPUT);
  pinMode(POT, INPUT);
  pinMode(MODE_1, INPUT_PULLUP);
  pinMode(MODE_2, INPUT_PULLUP);

  // Set the motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //init game mode
  mode = read_mode();

  //setup audio
  // Initialisation du DFPlayer
  while(!DF1201S.begin(Serial3)){
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }

  Serial.println("audio ready");
  Serial.print("mode :");
  Serial.println(mode);
  
  //Set volume to 15
  DF1201S.setVol(25);
  //deactivate prompts
  DF1201S.setPrompt(false);
  //Enter music mode
  DF1201S.switchFunction(DF1201S.MUSIC);
  //Set playback mode to "repeat all"
  DF1201S.setPlayMode(DF1201S.SINGLE);

  //test sensor
  if( (bar_1.get_front_sensor() > THRESHOLD) ||  (bar_1.get_back_sensor() > THRESHOLD) || (bar_2.get_front_sensor() > THRESHOLD) ||
      (bar_2.get_back_sensor() > THRESHOLD) || (house.get_sensor() > THRESHOLD)){
    Serial.println("ERROR : sensor obstructed");
  }

  delay(100);
}

void loop() {
  //change volume
  vol = analogRead(POT);            // reads the value of the potentiometer (value between 0 and 1023)
  vol = map(vol, 0, 1023, 0, 30);     // scale it for use with the servo (value between 0 and 180)
  DF1201S.setVol(/*VOL = */vol);

  //change mode
  mode = read_mode();
  
  // check button
  buttonObsState = digitalRead(B_OBS);
  buttonForwState = digitalRead(B_FORWARD);
  buttonBackState = digitalRead(B_BACKWARD);
  
  /*
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
      sound = YAY;
    } else {
      Serial.println("Unknown command: " + receivedText);
    }
  }else{
    buttonObsState = LOW;
    buttonForwState = LOW;
    buttonBackState = LOW;
  }*/

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
      sound = COW;
    }
  }

  if (buttonBackLastState != buttonBackState){
    if (buttonBackState == HIGH){
      motorState = BACKWARD;
      sound = BRAKE;
    }
  }

  //update buttons state
  buttonObsLastState = buttonObsState;
  buttonForwLastState = buttonForwState;
  buttonBackLastState = buttonBackState;


  //update the obstacles
  if (mode == SLOW){
    bar_1_update = bar_1.update(buttonObs, motorState);
    bar_2_update = bar_2.update(buttonObs, motorState);
    house_update = house.update(buttonObs, motorState);
  }else{
    bar_1_update = bar_1.update_continuous(buttonObs);
    bar_2_update = bar_2.update_continuous(buttonObs);
    house_update = house.update_continuous(buttonObs);
  }
  

  if (verbose){
    //Serial.println(house.get_state());
    Serial.print("mode :");
    Serial.println(mode);
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
      //DF1201S.playFileNum(4);
      sound = NONE;
      break;
    case NARUTO:
      //DF1201S.playFileNum(4);
      sound = NONE;
      break;
    case POULE:
      //DF1201S.playFileNum(4);
      sound = NONE;
      break;
    case PIG:
      //DF1201S.playFileNum(4);
      sound = NONE;
      break;
    case COW:
      Serial.println("cow");
      DF1201S.playFileNum(3);
      sound = NONE;
      break;
    case YAY:
      Serial.println("yay");
      DF1201S.playFileNum(4);
      sound = NONE;
      break;
    case BRAKE:
      Serial.println("brake");
      DF1201S.playFileNum(1);
      sound = NONE;
      break;
    case NONE:
      break;
  }
  
}

GameMode read_mode(){

  if(digitalRead(MODE_1) == HIGH){
    return SLOW;
  }else if(digitalRead(MODE_2) == HIGH){
    return CONTINUOUS;
  }else{
    return SLOW;
  }

}
