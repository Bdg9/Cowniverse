#include <Adafruit_PWMServoDriver.h>                             //Include the PWM Driver library
#include <RF24.h> // download library by TMRh20 last version (1.4.8)

#define DFR 0 //door front right
#define DFL 1 //door front left
#define DHL 2 //door hind left
#define DHR 3 //door hind right
#define GATE 4 //gate
#define DELAY_SERVO 10

#define DIR 8
#define PUL 9

// Radio settings
#define CE_PIN 4
#define CSN_PIN 7
RF24 radio(CE_PIN, CSN_PIN);
const uint64_t address = "Butt1";


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);    //Create an object of board 1

int servoMin =  135;        // This is the servos minimum pulse length count (out of 4096)
int servoMax =  360;        // This is the servos maximum pulse length count (out of 4096)
int servoFrequency = 50;    // Servo update frequency, analog servos typically run at ~50 Hz

//default pulse length for servos
int dl_min = 135;
int dl_max = 360;
int dr_min = 235;
int dr_max = 425;
int gate_min = 135;
int gate_max = 270;

//variables for stepper motor 
int x;
int trajet;
int state_bigDome;
int last_state_bigDome = LOW;
const int speed = 2000;
const int bigDome = 2; 
const int nb_rev_trajet_1 = 1890;
const int nb_rev_trajet_2 = 2065;


void setup() {
  // put your setup code here, to run once:
  //servos only to PWM ports
  pwm.begin();             //Start board
  pwm.setOscillatorFrequency(27000000);    //Set the PWM oscillator frequency, used for fine calibration
  pwm.setPWMFreq(servoFrequency);          //Set the servo operating frequency

  // for stepper motor driver
  pinMode(PUL,OUTPUT); // PUL
  pinMode(DIR,OUTPUT); // DIR
  pinMode(bigDome, INPUT);
  // serial
  Serial.begin(9600);
  delay(3000);
  // initialisation trajet n°1
  trajet = 1;
  Serial.println(trajet);
  // direction of rotation
  digitalWrite(DIR,LOW); // direction of rotation

  //puts servos at the right position
  pwm.setPWM(DFR, 0, dr_max);
  delay(10);
  pwm.setPWM(DFL, 0, dl_min);
  delay(10);
  pwm.setPWM(DHR, 0, dr_max);
  delay(10);
  pwm.setPWM(DHL, 0, dl_min);
  delay(10);
  pwm.setPWM(GATE, 0, gate_min);
  delay(500);
  
  if (!radio.begin()) {
    //while (1) {}// hold in infinite loop
      Serial.println("Could not turn radio on");  
  }else{
    Serial.println("Radio successfully turned on, now listening");
  }
  // radio settings
  radio.setPayloadSize(4); // sizeof("Meuh")
  radio.setChannel(108); // 0 to 127
  radio.setPALevel (RF24_PA_LOW); // for now //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  
  //set the address for communication and set module as receiver
  radio.openReadingPipe(0, address); // 0 to 5 is the pipe to be opened 
  radio.startListening();
    

}

void loop() {
  // put your main code here, to run repeatedly:
  state_bigDome = digitalRead(bigDome); //uncomment for physically wired button
  if (radio.available()){
    char text[5] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
    state_bigDome = HIGH;
  }


  if (state_bigDome != last_state_bigDome) { // trajet 1
    if ((trajet == 1) && (state_bigDome == HIGH)) {
      open_hind_doors();
      turn_stepper(nb_rev_trajet_1);
      close_hind_doors();
      trajet = 2;
      state_bigDome = LOW;
    }

    if ((trajet == 2) && (state_bigDome == HIGH)) {
      open_gate_front_doors();
      turn_stepper(nb_rev_trajet_2);
      close_gate_front_doors();
      trajet = 1;    
    }
    delay(50);
  }
  
  last_state_bigDome = state_bigDome;
}

//functions for house doors
void open_front_doors(){
  Serial.println("opening front doors");
  for (int i = 0 ; i <= 225 ; i++){
    pwm.setPWM(DFL, 0, dl_min + i);
    pwm.setPWM(DFR, 0, dr_max - i);
    delay(DELAY_SERVO);
  }
}

void open_hind_doors(){
  for (int i = 0 ; i <= 225 ; i++){
    pwm.setPWM(DHL, 0, dl_min + i);
    pwm.setPWM(DHR, 0, dr_max - i);
    delay(DELAY_SERVO);
  }
}

void close_front_doors(){
  for (int i = 0 ; i <= 225 ; i++){
    pwm.setPWM(DFL, 0, dl_max - i);
    pwm.setPWM(DFR, 0, dr_min + i);
    delay(DELAY_SERVO);
  }
}

void close_hind_doors(){
  for (int i = 0 ; i <= 210 ; i++){
    pwm.setPWM(DHL, 0, dl_max - i);
    pwm.setPWM(DHR, 0, dr_min + i);
    delay(DELAY_SERVO);
  }
}

//functions gate
void open_gate(){
    for (int i = 0 ; i <= 135 ; i++){
    pwm.setPWM(GATE, 0, gate_min + i);
    delay(DELAY_SERVO);
  }
}

void close_gate(){
    for (int i = 0 ; i <= 135 ; i++){
    pwm.setPWM(GATE, 0, gate_max - i);
    delay(DELAY_SERVO);
  }
}

void open_gate_front_doors(){
  for (int i = 0 ; i <= 225 ; i++){
    pwm.setPWM(DFL, 0, dl_min + i);
    pwm.setPWM(DFR, 0, dr_max - i);
    if (i<=135){
      pwm.setPWM(GATE, 0, gate_min + i);
    }
    delay(DELAY_SERVO);
  }
}

void close_gate_front_doors(){
  for (int i = 0 ; i <= 225 ; i++){
    pwm.setPWM(DFL, 0, dl_max - i);
    pwm.setPWM(DFR, 0, dr_min + i);
    if (i<=135){
      pwm.setPWM(GATE, 0, gate_max - i);
    }
    delay(DELAY_SERVO);
  }
}

//functions stepper motor
void turn_stepper(int nb_rev){
  for(x = 0; x < nb_rev; x++) { // set number of revolution
      digitalWrite(PUL,HIGH);  // set high level direction of rotation
      delayMicroseconds(speed); // set rotate speed
      digitalWrite(PUL,LOW); // output low
      delayMicroseconds(speed);
    }
}

