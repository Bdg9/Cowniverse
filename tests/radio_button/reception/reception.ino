//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <SoftwareSerial.h>
//#include <AltSoftSerial.h>

//SoftwareSerial mySerial(5, 6); // RX, TX
//AltSoftSerial mySerial; // Pins 8 (RX) and 9 (TX) on Arduino Uno

#define CE_PIN 9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);
const uint64_t address = "Butt1"; //address through which two modules communicate.

#define FWD_PIN 2
#define BWD_PIN 3

unsigned long prevMillisfwd = 0;   // Stores the last time a message was sent
unsigned long prevMillisbwd = 0;
const unsigned long interval = 10;  // Interval between messages 
bool fwdState = LOW;
bool bwdState = LOW;

void setup()
{
  Serial.begin(9600);
  //mySerial.begin(9600);

  if (!radio.begin()){Serial.println("Could not turn radio on"); 
  }else{Serial.println("Radio successfully turned on");}

  //Radio settings
  //radio.setPayloadSize(6); // sizeof("Meuh")
  radio.setDataRate (RF24_250KBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS/ lowest the better
  radio.setChannel(108); // 0 to 127
  radio.setPALevel (RF24_PA_LOW); // for now //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.openReadingPipe(0, address); // set the address for communication, 0 to 5 is the pipe to be opened 
  radio.startListening(); // set module as receiver

  pinMode(FWD_PIN, OUTPUT); // Pin for fwd
  pinMode(BWD_PIN, OUTPUT); // Pin for bwd
}

void loop()
{
  unsigned long currentMillis = millis();

  if (radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);

    if(strcmp(text, "fwd") == 0){
      digitalWrite(FWD_PIN, HIGH);
      prevMillisfwd = currentMillis;
      fwdState = HIGH;

    }else if (strcmp(text, "bwd") == 0){
      digitalWrite(BWD_PIN, HIGH);
      prevMillisbwd = currentMillis;
      bwdState = HIGH; 
    }else{
      Serial.println("Didn't recognize message");
    }
    
  }

  // Check if the pin should be set to LOW
  if (fwdState == HIGH && (currentMillis - prevMillisfwd) >= interval) {
    digitalWrite(FWD_PIN, LOW);
    fwdState = LOW;
  }

  if (bwdState == HIGH && (currentMillis - prevMillisbwd) >= interval) {
    digitalWrite(BWD_PIN, LOW);
    bwdState = LOW;
  }
    //mySerial.write(text);
  
}
