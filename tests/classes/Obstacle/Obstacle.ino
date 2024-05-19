#include <Servo.h>

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
    int front
  public:
    enum State state;

    Obstacle(Servo &s) : servo(s){
    }

    void init(int servo_pin, int front_sensor_pin, int opened, int closed){
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
      pinMode(front_sensor_pin, INPUT);
      delay(100);
    }

    int get_position(){
      return servo.read();
    }

    void update(){
      value_front_sensor = analogRead(front_sensor_pin);

      if(pos == opened_pos){
        state = Opening;
      }else if(pos == closed_pos){
        state = Closing;
      }

      if(state == Opening){
        if (opened_pos < closed_pos){
          pos +=1;
        }else{
          pos -=1;
        }
      }else if(state == Closing){
        if (opened_pos < closed_pos){
          pos -= 1;
        }else{
          pos += 1;
        }
      }

      servo.write(pos);
    }
};

Servo obs1;
Obstacle obs(obs1);

Servo obs2;
Obstacle r_door(obs2);

void setup() {
  // put your setup code here, to run once:
  obs.init(9, 0, 60);
  r_door.init(10, 60, 0);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  obs.update();
  r_door.update();
  delay(50);
  Serial.print("left door state : ");
  Serial.print(obs.state);
  Serial.print("    right door state : ");
  Serial.println(r_door.state);
  
}
