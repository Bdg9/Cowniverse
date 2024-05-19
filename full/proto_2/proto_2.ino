#include <Servo.h>
#include "Obstacle.h"

//left door
Servo l_door_servo;
Obstacle l_door(l_door_servo);

//right door
Servo r_door_servo;
Obstacle r_door(r_door_servo);

//buttons
const int buttonPin = 2;
int buttonState = LOW;
int buttonLastState = LOW;
bool button = false;


void setup() {
  // put your setup code here, to run once:
  l_door.init(9, A2, 60, 0);
  r_door.init(10, A2, 0, 60);

  //button setup
  pinMode(buttonPin, INPUT);
  
  //serial monitor set up
  Serial.begin(9600);
  delay(100);
}

void loop() {
  // check button
  buttonState = digitalRead(buttonPin);

  if (buttonLastState != buttonState){
    if (buttonState == HIGH){
      button = true;
    }
  }else{
    button = false;
  }

  buttonLastState = buttonState;

  l_door.update(button);
  r_door.update(button);
  //prints for debug
  //Serial.print("sensor value : ");
  //Serial.println(l_door.get_front_sensor());
  
}
