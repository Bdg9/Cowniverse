#include <SPI.h>
#include <RF24.h>

#define CE_PIN 9 //3 in digispark
#define CSN_PIN 10 //4 in digispark

RF24 radio(CE_PIN, CSN_PIN);  
const int fwdButt = 3; // green 
const int bwdButt = 2; // blue

int last_fwd = 0;
int last_bwd = 0;
//address through which two modules communicate.
const uint64_t address = "Butt1"; // any 5 character String or 40 bit address 
int last = 0;

void setup()
{
  Serial.begin(9600);
  if (!radio.begin()) {
    //while (1) {}  // hold in infinite loop
    Serial.println("FAil");
  }else{
    Serial.println("Radio successfully turned on");
  }

  // radio settings
  //radio.setPayloadSize(6); // sizeof("Meuh")
  radio.setChannel(108); // 0 to 127
  radio.setPALevel (RF24_PA_LOW); // for now //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  
  //set the address for communication and set module as transmitter
  radio.openWritingPipe(address);
  radio.stopListening();//Set module as transmitter
  //radio.setRetries(15,10);
  
  //const char ack[] = "START";
  //while(! radio.write(&ack, sizeof(ack))){
    //digitalWrite(led,HIGH);
  //}
  //turn LED outside of the box OFF to signify the program can transmit
  //digitalWrite(led,LOW);

  pinMode(fwdButt, INPUT);
  pinMode(bwdButt, INPUT);
  
}

void loop()
{
  int fwdState = digitalRead(fwdButt);
  int bwdState = digitalRead(bwdButt);


  const char fwd[] = "fwd";
  const char bwd[] = "bwd";
  
    if(fwdState != last_fwd){ 
    if (fwdState == HIGH){
      while(! radio.write(&fwd, sizeof(fwd))){
        delay(50);
        Serial.println("Could not transmit");
      }
      Serial.println("forward");
    }
    delay(100); 
  }
  last_fwd = fwdState;

  //Same for backward button
  if(bwdState != last_bwd){ 
    if (bwdState == HIGH){
      radio.write(&bwd, sizeof(bwd));
      Serial.println("back");
    }
    delay(100);
  }
  last_bwd = bwdState;
}
