// Obstacle.h
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <Arduino.h>
#include <Servo.h>

#define THRESHOLD 900

enum State{
  Closed, //0
  Opening,//1
  Opened, //2
  Closing,//3
};

class Obstacle {
  private:
    Servo &servo;
    int pos;
    int opened_pos;
    int closed_pos;
    int front_sensor;
    int front_sensor_pin;
  public:
    enum State state;

    Obstacle(Servo &s);
    void init(int servo_pin, int front_sensor_pin, int opened, int closed);
    int get_position();
    int get_front_sensor();
    void update(bool button);
};

#endif // OBSTACLE_H