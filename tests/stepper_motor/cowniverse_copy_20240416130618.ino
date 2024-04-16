int x;
int trajet = 1;
int state_bigDome;
int last_state_bigDome = HIGH;
const int speed = 2000;
const int bigDome = 2;

void setup() {
// for stepper motor driver
pinMode(9,OUTPUT); // PUL
pinMode(8,OUTPUT); // DIR
// for stepper motor button
pinMode(bigDome, INPUT);
// serial
Serial.begin(9600);
delay(3000);
Serial.println(trajet);
// initialisation trajet n°1
//trajet = 1;
}

void loop() {

state_bigDome = digitalRead(bigDome);

  if (state_bigDome != last_state_bigDome) { // trajet 1
    if ((trajet == 1) && (state_bigDome == HIGH)) {
      Serial.println(trajet);
      trajet += 1;
      state_bigDome = LOW;
      digitalWrite(8,LOW);
    for(x = 0; x < 1900; x++) { // set number of revolution

      digitalWrite(9,HIGH);  // set high level direction of rotation
      delayMicroseconds(speed); // set rotate speed
      digitalWrite(9,LOW); // output low
      delayMicroseconds(speed);
      //Serial.println(x);
    }
    Serial.println(x);
    delay(1000); //pause 1 second
    digitalWrite(8,HIGH); // set LOW level direction
    }
  }
  delay(50);

  if ((trajet == 2) && (state_bigDome == HIGH))  { //trajet 2
    Serial.println(trajet);
    trajet -= 1;
    state_bigDome = LOW;
    if (state_bigDome == HIGH) {
      digitalWrite(8,LOW);
      
    for(x = 0; x < 2000; x++) { // set number of revolution

      digitalWrite(9,HIGH);  // set high level direction of rotation
      delayMicroseconds(speed); // set rotate speed
      digitalWrite(9,LOW); // output low
      delayMicroseconds(speed);
      //Serial.println(x);
    }
    Serial.println(x);
    delay(1000); //pause 1 second
    digitalWrite(8,HIGH); // set LOW level direction
    }
  }
  delay(50);

  last_state_bigDome = state_bigDome;

}