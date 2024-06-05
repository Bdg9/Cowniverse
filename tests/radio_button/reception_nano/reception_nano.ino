//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);
const uint64_t address = "Butt1";

#define FWD_PIN 2
#define BWD_PIN 3
#define OBS_PIN 4

// Store the last time a message was sent
unsigned long prevMillisfwd = 0;   
unsigned long prevMillisbwd = 0;
unsigned long prevMillisobs = 0;

// Interval between messages 
const unsigned long interval = 10;  

bool fwdState = LOW;
bool bwdState = LOW;
bool obsState = LOW;

void setup()
{
  Serial.begin(9600);

  if (!radio.begin()){Serial.println("Could not turn radio on"); 
  }else{Serial.println("Radio successfully turned on");}

  //Radio settings
  radio.setDataRate (RF24_250KBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS/ lowest the better
  radio.setChannel(108); // 0 to 127
  radio.setPALevel (RF24_PA_LOW); // transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.openReadingPipe(0, address); // set the address for communication, 0 to 5 is the pipe to be opened 
  radio.startListening(); // module = receiver

  //pinMode(FWD_PIN, OUTPUT); // Pin for fwd
  //pinMode(BWD_PIN, OUTPUT); // Pin for bwd
  //pinMode(OBS_PIN, OUTPUT); // Pin for obs
}

void loop()
{
  unsigned long currentMillis = millis();

  if (radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
    //Serial.println(text);

    if(strcmp(text, "fwd") == 0){
      //digitalWrite(FWD_PIN, HIGH);
      Serial.println("fwd");
      prevMillisfwd = currentMillis;
      fwdState = HIGH;

    }else if (strcmp(text, "bwd") == 0){
      //digitalWrite(BWD_PIN, HIGH);
      Serial.println("bwd");
      prevMillisbwd = currentMillis;
      bwdState = HIGH; 

    }else if (strcmp(text, "obs") == 0){
      //digitalWrite(OBS_PIN, HIGH);
      Serial.println("obs");
      prevMillisobs = currentMillis;
      obsState = HIGH; 
  
    }else{
      Serial.println("Didn't recognize message");
    }
  }

  // Check if the pin should be set to LOW
  if (fwdState == HIGH && (currentMillis - prevMillisfwd) >= interval) {
    //digitalWrite(FWD_PIN, LOW);
    fwdState = LOW;
  }
  if (bwdState == HIGH && (currentMillis - prevMillisbwd) >= interval) {
    //digitalWrite(BWD_PIN, LOW);
    bwdState = LOW;
  }
  if (obsState == HIGH && (currentMillis - prevMillisobs) >= interval) {
    //digitalWrite(OBS_PIN, LOW);
    obsState = LOW;
  }
  
}
