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

//file paths for audio
const char* file_paths_meuh[] = {
    "/meuh/Meuh1.mp3",
    "/meuh/Meuh2.mp3",
    "/meuh/Meuh3.mp3",
    "/meuh/Meuh4.mp3",
    "/meuh/Meuh5.mp3",
    "/meuh/Meuh6.mp3",
    "/meuh/Meuh7.mp3",
    "/meuh/Meuh8.mp3",
};

const char* file_paths_bip[] = {
  "/bipbip/Bip1.mp3",
  "/bipbip/Bip2.mp3",
  "/bipbip/Bip3.mp3",
  "/bipbip/Bip4.mp3",
  "/bipbip/Bip5.mp3",
  "/bipbip/Bip6.mp3",
};

const char* file_paths_groin[] = {
  "/groin/Groin1.mp3",
  "/groin/Groin2.mp3",
  "/groin/Groin3.mp3",
  "/groin/Groin4.mp3",
  "/groin/Groin5.mp3",
  "/groin/Groin6.mp3",
  "/groin/Groin7.mp3",
};

const char* file_paths_cot[] = {
  "/poule/Poule5.mp3",
  "/poule/Poule4.mp3",
  "/poule/Poule3.mp3",
  "/poule/Poule2.mp3",
  "/poule/Poule1.mp3",
};

const char* file_paths_yay[] = {
  "/marley/marley1.mp3",
  "/yay/Hiphoura2.mp3",
  "/yay/Houra1.mp3",
  "/yay/titu.mp3",
  "/yay/Uhou1.mp3",
  "/yay/uhouai.mp3",
  "/yay/uhu.mp3",
  "/yay/vamos.mp3",
  "/yay/Yay1.mp3",
  "/yay/Yay2.mp3",
  "/yay/Yay3.mp3",
  "/yay/Yay4.mp3",
  "/yay/Youpi1.mp3",
  "/yay/Youpi2.mp3",
  "/marley/marley1.mp3",
  "/ohNo/Ohoh1.mp3",
};

const int num_files_meuh = sizeof(file_paths_meuh) / sizeof(file_paths_meuh[0]);
const int num_files_bip = sizeof(file_paths_bip) / sizeof(file_paths_bip[0]);
const int num_files_groin = sizeof(file_paths_groin) / sizeof(file_paths_groin[0]);
const int num_files_yay = sizeof(file_paths_yay) / sizeof(file_paths_yay[0]);
const int num_files_cot = sizeof(file_paths_cot) / sizeof(file_paths_cot[0]);

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
Sound character_type;

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

  //choose character type
  int mode_1 = digitalRead(MODE_1);
  int mode_2 = digitalRead(MODE_2);
  if((mode_1 == HIGH) && (mode_2 == HIGH)){
    character_type = COW;
  }else if(mode_2 == HIGH){
    character_type = PIG;
  }else{
    character_type = COT;
  }

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
  DF1201S.setVol(20);
  //deactivate prompts
  DF1201S.setPrompt(false);
  //Enter music mode
  DF1201S.switchFunction(DF1201S.MUSIC);
  //Set playback mode to "repeat all"
  DF1201S.setPlayMode(DF1201S.SINGLE);

  delay(500);

  //test sensor
  if( (bar_1.get_front_sensor() > THRESHOLD) ||  (bar_1.get_back_sensor() > THRESHOLD) || (bar_2.get_front_sensor() > THRESHOLD) ||
      (bar_2.get_back_sensor() > THRESHOLD) || (house.get_sensor() > THRESHOLD)){
    Serial.println("ERROR : sensor obstructed");
    DF1201S.playFileNum(/*File Number = */2);
    delay(10000);
  }

  DF1201S.playFileNum(/*File Number = */1);
  delay(200);

  
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
      sound = BIP;
    }
  }else{
    buttonObs = false;
  }

  if (buttonForwLastState != buttonForwState){
    if (buttonForwState == HIGH){
      motorState = FORWARD;
      sound = character_type;
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
  if (mode == SLOW){
    bar_1_update = bar_1.update(buttonObs, motorState);
    bar_2_update = bar_2.update(buttonObs, motorState);
    house_update = house.update(buttonObs, motorState);
  }else{
    bar_1_update = bar_1.update_continuous(buttonObs);
    bar_2_update = bar_2.update_continuous(buttonObs);
    house_update = house.update_continuous(buttonObs);
  }

  if (house_update){
    sound = YAY;
  }
  

  if (verbose){
    //Serial.println(house.get_state());
    Serial.print("mode 1 :");
    Serial.print(digitalRead(MODE_1));
    Serial.print("  mode_2 : ");
    Serial.println(digitalRead(MODE_2));
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
    case COT:
      playRandomSong(file_paths_cot, num_files_cot);
      sound = NONE;
      break;
    case PIG:
      playRandomSong(file_paths_groin, num_files_groin);
      sound = NONE;
      break;
    case COW:
      playRandomSong(file_paths_meuh, num_files_meuh);
      sound = NONE;
      break;
    case YAY:
      playRandomSong(file_paths_yay, num_files_yay);
      sound = NONE;
      break;
    case BIP:
      playRandomSong(file_paths_bip, num_files_bip);
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

void playRandomSong(const char* file_paths[], const int num_files) {
  // Select a random file path from the list
  int randomIndex = random(num_files);
  const char* randomFile = file_paths[randomIndex];

  // Play the selected file
  DF1201S.playSpecFile(randomFile);
}
