#define PUL 9
#define DIR 8

int x;
int trajet;
int state_bigDome;
int last_state_bigDome = LOW;
const int speed = 2000;
const int bigDome = 2;

void setup() {
// for stepper motor driver
pinMode(PUL,OUTPUT); // PUL
pinMode(DIR,OUTPUT); // DIR
// for stepper motor button
pinMode(bigDome, INPUT);
// serial
Serial.begin(9600);
delay(3000);
// initialisation trajet n°1
trajet = 1;
Serial.println(trajet);
// direction of rotation
digitalWrite(DIR,LOW); // direction of rotation
}

void loop() {

state_bigDome = digitalRead(bigDome);

  if (state_bigDome != last_state_bigDome) { // trajet 1
    if ((trajet == 1) && (state_bigDome == HIGH)) {
      //Serial.println(trajet);
      for(x = 0; x < 1890; x++) { // set number of revolution
        digitalWrite(PUL,HIGH);  // set high level direction of rotation
        delayMicroseconds(speed); // set rotate speed
        digitalWrite(PUL,LOW); // output low
        delayMicroseconds(speed);
      }
      //Serial.println(x);
      trajet = 2;
    state_bigDome = LOW;
    last_state_bigDome = state_bigDome;
    }
  }
  delay(50);

  

  if ((trajet == 2) && (state_bigDome == HIGH))  { //trajet 2
    //Serial.println(trajet);    
    if (state_bigDome == HIGH) {
      for(x = 0; x < 2065; x++) { // set number of revolution
        digitalWrite(PUL,HIGH);  // set high level direction of rotation
        delayMicroseconds(speed); // set rotate speed
        digitalWrite(PUL,LOW); // output low
        delayMicroseconds(speed);
      }
      //Serial.println(x);
      trajet = 1;
    }
  state_bigDome = LOW;
  last_state_bigDome = state_bigDome;
  }
  delay(50);

}