#include <Adafruit_PWMServoDriver.h>                             //Include the PWM Driver library

#define CLOSED 0
#define OPENING 1
#define OPENED 2
#define CLOSING 3

class Obstacle{
  private :
  int opened_pwm;
  int closed_pwm;
  int current_pwm;
  int pin;
  
  public :
  int state;

  Obstacle(int pin_nb, int opened, int closed){
    pin = pin_nb;
    opened_pwm = opened;
    closed_pwm = closed;
    current_pwm = closed;
    state = CLOSED;
  }

  void init(Adafruit_PWMServoDriver pwm){
    pwm.setPWM(pin, 0, closed_pwm);
  }

  void update(Adafruit_PWMServoDriver pwm){
    if(current_pwm == opened_pwm){
      state = OPENING;
    }else if(current_pwm == closed_pwm){
      state = CLOSING;
    }

    if(state == OPENING){
      current_pwm += 1;
    }else if(state == CLOSING){
      current_pwm -= 1;
    }

    pwm.setPWM(pin, 0, current_pwm);

  }
};

Obstacle obs_1(0, 135, 360);
Obstacle obs_2(1, 135, 360);

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);    //Create an object of board 1

int servoMin =  135;        // This is the servos minimum pulse length count (out of 4096)
int servoMax =  360;        // This is the servos maximum pulse length count (out of 4096)
int servoFrequency = 50;    // Servo update frequency, analog servos typically run at ~50 Hz


void setup() {
  // put your setup code here, to run once:
  pwm.begin();             //Start board
  pwm.setOscillatorFrequency(27000000);    //Set the PWM oscillator frequency, used for fine calibration
  pwm.setPWMFreq(servoFrequency);          //Set the servo operating frequency

  obs_1.init(pwm);
  obs_2.init(pwm);
}

void loop() {
  // put your main code here, to run repeatedly:
  obs_1.update(pwm);
  obs_2.update(pwm);
  delay(10);
}
