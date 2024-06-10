// HOUSE.h
#ifndef HOUSE_H
#define HOUSE_H

#include <Arduino.h>
#include <Servo.h>
#include "States.h"

#define THRESHOLD 100
#define DELAY_CLOSE 5000

class House {
  private:
    Servo &servo1;
    Servo &servo2;
    int pos_1;
    int pos_2;
    int opened_pos_1;
    int opened_pos_2;
    int closed_pos_1;
    int closed_pos_2;
    int sensor;
    int sensor_pin;
    MotorState lastMotorState;
  public:
    enum ObsState state;

    House(Servo &s1, Servo &s2);
    void init(int servo_pin_1, int servo_pin_2, int sensor_pin, int opened_1, int closed_1, int opened_2, int closed_2);
    int get_sensor();
    ObsState get_state();
    bool update(bool button, MotorState motorState);
    bool update_continuous(bool button);
};

#endif // HOUSE_H