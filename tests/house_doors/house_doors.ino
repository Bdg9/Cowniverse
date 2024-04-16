#include <Servo.h>

Servo door_fr;
Servo door_fl;
Servo door_hl;
Servo door_hr;

int pos_door_fr = 0;
int pos_door_fl = 180;
int pos_door_hl = 0;
int pos_door_hr = 180;

int b1 = 2;
int b2 = 4;

int state_b1;
int last_state_b1;

int state_b2;
int last_state_b2;


void setup() {
  // put your setup code here, to run once:
  //servos only to PWM ports
  door_fr.attach(6);
  door_fl.attach(9);
  door_hl.attach(10);
  door_hr.attach(11);

  pinMode(b1, INPUT);
  pinMode(b2, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  state_b1 = digitalRead(b1);
  state_b2 = digitalRead(b2);

  if (state_b1 != last_state_b1){
    if (state_b1 == HIGH){
      open_front_doors();
      delay(500);
      close_front_doors();
    }
    delay(50);
  }

  if (state_b2 != last_state_b2){
    if (state_b2 == HIGH){
      open_hind_doors();
      delay(500);
      close_hind_doors();
    }
    delay(50);
  }

  last_state_b1 = state_b1;
  last_state_b2 = state_b2;
}

void move_doors(int pos1, int pos2, Servo door_1, Servo door_2){
  door_1.write(pos1);
  door_2.write(pos2);
}

void open_front_doors(){
  int _ = 0;
  for (_=0; _<=120; _+=1) {
    pos_door_fr += 1;
    pos_door_fl -= 1;
    move_doors(pos_door_fl, pos_door_fr, door_fl, door_fr);
    delay(15);
  }
}

void open_hind_doors(){
  int _ = 0;
  for (_=0; _<=120; _+=1) {
    pos_door_hl += 1;
    pos_door_hr -= 1;
    move_doors(pos_door_hl, pos_door_hr, door_hl, door_hr);
    delay(15);
  }
}

void close_front_doors(){
  int _ = 0;
  for (_=0; _<=120; _+=1) {
    pos_door_fr -= 1;
    pos_door_fl += 1;
    move_doors(pos_door_fl, pos_door_fr, door_fl, door_fr);
    delay(15);
  }
}

void close_hind_doors(){
  int _ = 0;
  for (_=0; _<=120; _+=1) {
    pos_door_hl -= 1;
    pos_door_hr += 1;
    move_doors(pos_door_hl, pos_door_hr, door_hl, door_hr);
    delay(15);
  }
}