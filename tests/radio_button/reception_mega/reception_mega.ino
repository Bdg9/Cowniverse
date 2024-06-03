//#include <SoftwareSerial.h> //SoftwareSerial mySerial(3, 2); // RX, TX, the grounds of both are supposed to be connected together
//#include <AltSoftSerial.h> //AltSoftSerial mySerial; // Pins 8 (RX) and 9 (TX) on Arduino Uno

#define FWDpin 2
#define BWDpin 3
int last_fwd = 0;
int last_bwd = 0;

void setup() {
  Serial.begin(9600);
  //mySerial.begin(9600);
  pinMode(FWDpin, INPUT); // Pin for fwd
  pinMode(BWDpin, INPUT); // Pin for bwd
}

void loop() {

  int fwdState = digitalRead(FWDpin);
  int bwdState = digitalRead(BWDpin);
  
  if(fwdState != last_fwd && fwdState == HIGH){
    Serial.println("forward");
    delay(200); 
  }
  last_fwd = fwdState;
  
  if(bwdState != last_bwd && bwdState == HIGH){
    Serial.println("backward");
    delay(200); 
  }
  last_fwd = fwdState;

  
}
//if (mySerial.available() > 0) {
  //  char incomingByte = mySerial.read(); // Read a byte from the serial port
  //  if (incomingByte != '\r' && incomingByte != '\n') {
  //    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
  //    delay(100); // Wait for 100 milliseconds
  //    digitalWrite(LED_BUILTIN, LOW); // Turn the LED off
      //Serial.print("Received: ");
      //Serial.println(incomingByte); // Print the received character
  //  }
  //}
