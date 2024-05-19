#include <Servo.h>

#define CLOSED 0
#define OPENING 1
#define OPEN 2
#define CLOSING 3



class Obstacle : public Servo{
  private:
    int open_pos;
    int closed_pos;
  public:
    
    int pos;
    int state;

    Obstacle(){
      pos = 0;
      state = OPEN;
    }

    void init(int pin_nb, int open_pos, int closed_pos){
      attach(pin_nb);
      write(pos);
      this->open_pos = open_pos;
      this->closed_pos = closed_pos;
    }

    void update(){
      if(this->pos == this->open_pos){
        this->state = OPENING;
      }else if(this->pos == this->closed_pos){
        this->state = CLOSING;
      }

      if(this->state == OPENING){
        this->pos += 1;
      }else if(this->state == CLOSING){
        this->pos-=1;
      }

      write(pos);
    }
};

class MyServo : public Servo {
  private:
    int offset; // Additional member variable for offset

  public:
    // Constructor
    MyServo() : offset(0) {}

    // Function to set offset
    void setOffset(int _offset) {
      offset = _offset;
    }

    // Function to attach and set initial position
    void attach(int pin, int initialPosition) {
      Servo::attach(pin);
      write(initialPosition + offset); // Adjust position with offset
    }

    // Function to move to a position with offset applied
    void moveToWithOffset(int position) {
      write(position + offset);
    }
};

Obstacle obs_1;
MyServo myServo;

void setup() {
  // put your setup code here, to run once:
  // obs_1.init(3, 0, 180);
  myServo.attach(3, 90); // Attach servo to pin 9 with initial position 90
  myServo.setOffset(10); // Set offset to 10
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // obs_1.update();
  myServo.moveToWithOffset(180); // Move servo to position 180 with offset applied
  delay(1000);
  myServo.moveToWithOffset(0); // Move servo to position 0 with offset applied
  delay(1000);
  Serial.println(obs_1.pos);
  delay(10);
}


