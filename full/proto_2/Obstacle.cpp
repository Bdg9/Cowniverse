// Obstacle.cpp
#include "Obstacle.h"

Obstacle::Obstacle(Servo &s) : servo(s){
}

void Obstacle::init(int servo_pin, int f_sensor_pin, int opened, int closed){
    //attach servo to its pin
    servo.attach(servo_pin);
    //set opened and closed pos
    opened_pos = opened;
    closed_pos = closed;
    //set init state
    state = Closed;
    //go to init state
    servo.write(closed_pos);
    //update pos
    pos = servo.read();

    //initialize front sensor
    front_sensor_pin = f_sensor_pin;
    pinMode(front_sensor_pin, INPUT);
}

int Obstacle::get_position(){
    return servo.read();
}

int Obstacle::get_front_sensor(){
    return front_sensor;
}

void Obstacle::update(bool button){
    front_sensor = analogRead(front_sensor_pin);

    if((state == Closed) && button && (front_sensor > THRESHOLD)){
      state = Opening;
      Serial.println("opening");
    }else if((state == Opened) && button && (front_sensor > THRESHOLD)){
      state = Closing;
      Serial.println("closing");
    }

    if(state == Closing){
    if (opened_pos < closed_pos){
        pos +=1;
    }else{
        pos -=1;
    }
    }else if(state == Opening){
    if (opened_pos < closed_pos){
        pos -= 1;
    }else{
        pos += 1;
    }
    }

    if(pos == opened_pos){
    state = Opened;
    }else if(pos == closed_pos){
    state = Closed;
    }

    servo.write(pos);
}