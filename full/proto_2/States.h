// States.h
#ifndef STATES_H
#define STATES_H

enum MotorState {
  STOP,
  FORWARD,
  BACKWARD
};

enum ObsState {
  CLOSED, //0
  OPENING, //1
  OPENED, //2
  CLOSING //3
};

enum Sound {
  PIG,
  COW,
  COT, //poule
  YAY,
  BIP,
  NONE
};

enum GameMode {
  SLOW,
  CONTINUOUS,
};

#endif