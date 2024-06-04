// Obstacle.cpp
#include "Obstacle.h"
#include "States.h"

Obstacle::Obstacle(Servo &s) : servo(s){
}

void Obstacle::init(int servo_pin, int f_sensor_pin, int b_sensor_pin, int opened, int closed){
    //attach servo to its pin
    servo.attach(servo_pin);
    //set opened and closed pos
    opened_pos = opened;
    closed_pos = closed;
    //set init state
    state = CLOSED;
    lastMotorState = STOP;
    //go to init state
    servo.write(closed_pos);
    //update pos
    pos = servo.read();

    //initialize front sensor
    front_sensor_pin = f_sensor_pin;
    back_sensor_pin = b_sensor_pin;
    pinMode(front_sensor_pin, INPUT);
    pinMode(back_sensor_pin, INPUT);
}

int Obstacle::get_position(){
    return servo.read();
}

int Obstacle::get_front_sensor(){
  return analogRead(front_sensor_pin);
}

int Obstacle::get_back_sensor(){
  return analogRead(back_sensor_pin);
}

ObsState Obstacle::get_state(){
  return state;
}

bool Obstacle::update(bool button, MotorState motorState){
    if (motorState != STOP){
      lastMotorState = motorState;
    }

    if (lastMotorState == FORWARD){
      front_sensor = analogRead(front_sensor_pin);
      back_sensor = analogRead(back_sensor_pin);
    }else{ //inverse back and front if the motor is in the other direction
      front_sensor = analogRead(back_sensor_pin);
      back_sensor = analogRead(front_sensor_pin);
    }

    if((state == CLOSED) && button && (front_sensor > THRESHOLD)){
      state = OPENING;
    }else if((state == OPENED) && (button || (back_sensor > THRESHOLD))){
      state = CLOSING;
    }

    if(state == CLOSING){
    if (opened_pos < closed_pos){
        pos +=1;
    }else{
        pos -=1;
    }
    }else if(state == OPENING){
    if (opened_pos < closed_pos){
        pos -= 1;
    }else{
        pos += 1;
    }
    }

    if(pos == opened_pos){
    state = OPENED;
    }else if(pos == closed_pos){
    state = CLOSED;
    }

    servo.write(pos);

    return (front_sensor > THRESHOLD);
}