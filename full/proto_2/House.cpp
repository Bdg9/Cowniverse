// House.cpp
#include "House.h"
#include "States.h"

House::House(Servo &s1, Servo &s2) : servo1(s1), servo2(s2){
}

void House::init(int servo_pin_1, int servo_pin_2, int s_pin, int opened_1, int closed_1, int opened_2, int closed_2){
    //attach servo to its pin
    servo1.attach(servo_pin_1);
    servo2.attach(servo_pin_2);
    //set opened and closed pos
    opened_pos_1 = opened_1;
    closed_pos_1 = closed_1;
    opened_pos_2 = opened_2;
    closed_pos_2 = closed_2;
    //set init state
    state = OPENED;
    lastMotorState = STOP;
    //go to init state
    servo1.write(opened_pos_1);
    servo2.write(opened_pos_2);
    //update pos
    pos_1 = servo1.read();
    pos_2 = servo2.read();

    //initialize sensor
    sensor_pin = s_pin;
    pinMode(sensor_pin, INPUT);
}

int House::get_sensor(){
    return analogRead(sensor_pin);
}

ObsState House::get_state(){
  return state;
}

bool House::update(bool button, MotorState motorState){
    static unsigned long openTime = 0;  // Time when the house was opened


    if (motorState != STOP){
      lastMotorState = motorState;
    }
    
    sensor = analogRead(sensor_pin);

    if((state == CLOSED) && button){
      state = OPENING;
      openTime = millis();
    }else if((state == OPENED) && (sensor > THRESHOLD) && ((millis() - openTime >= DELAY_CLOSE) || button)){
      state = CLOSING;
    }

    if(state == CLOSING){
    if (opened_pos_1 < closed_pos_1){
        pos_1 +=1;
    }else{
        pos_1 -=1;
    }
    if (opened_pos_2 < closed_pos_2){
        pos_2 +=1;
    }else{
        pos_2 -=1;
    }
    }else if(state == OPENING){
    if (opened_pos_1 < closed_pos_1){
        pos_1 -= 1;
    }else{
        pos_1 += 1;
    }
    if (opened_pos_2 < closed_pos_2){
        pos_2 -= 1;
    }else{
        pos_2 += 1;
    }
    }

    if(pos_1 == opened_pos_1){
    state = OPENED;
    }else if(pos_1 == closed_pos_1){
    state = CLOSED;
    }

    servo1.write(pos_1);
    servo2.write(pos_2);

    return (sensor > THRESHOLD);
}

bool House::update_continuous(bool button){
    sensor = analogRead(sensor_pin);
    
    if((state == CLOSED) && (button)){
      state = OPENING;
    }else if((state == OPENED) && (button)){
      state = CLOSING;
    }

    if(state == CLOSING){
    if (opened_pos_1 < closed_pos_1){
        pos_1 +=1;
    }else{
        pos_1 -=1;
    }
    if (opened_pos_2 < closed_pos_2){
        pos_2 +=1;
    }else{
        pos_2 -=1;
    }
    }else if(state == OPENING){
    if (opened_pos_1 < closed_pos_1){
        pos_1 -= 1;
    }else{
        pos_1 += 1;
    }
    if (opened_pos_2 < closed_pos_2){
        pos_2 -= 1;
    }else{
        pos_2 += 1;
    }
    }

    if(pos_1 == opened_pos_1){
    state = OPENED;
    }else if(pos_1 == closed_pos_1){
    state = CLOSED;
    }

    servo1.write(pos_1);
    servo2.write(pos_2);

    return (sensor > THRESHOLD);
}