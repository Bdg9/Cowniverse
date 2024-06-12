// Obstacle.h
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <Arduino.h>
#include <Servo.h>
#include "States.h"

#define THRESHOLD 100

class Obstacle {
  private:
    Servo &servo;
    int pos;
    int opened_pos;
    int closed_pos;
    int front_sensor;
    int back_sensor;
    int front_sensor_pin;
    int back_sensor_pin;
    MotorState lastMotorState;
  public:
    enum ObsState state;

    Obstacle(Servo &s);
    void init(int servo_pin, int front_sensor_pin, int b_sensor_pin, int opened, int closed);
    int get_position();
    int get_front_sensor();
    int get_back_sensor();
    ObsState get_state();
    bool update(bool button, MotorState motorState);
    bool update_continuous(bool button, MotorState motorState);
};

#endif // OBSTACLE_H