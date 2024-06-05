#include <SPI.h>
#include <RF24.h>

#define BWDbutt 2 
#define FWDbutt 3 
int last_fwd = 0;
int last_bwd = 0;

#define CE_PIN 9
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN);  
const uint64_t address = "Butt1"; //address : any 5 character String or 40 bit address 

void setup()
{
  Serial.begin(9600);
  if (!radio.begin()){Serial.println("Failed to turn radio on");
  }else{Serial.println("Radio successfully turned on");}

  // Radio settings
  radio.setDataRate (RF24_250KBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS // 250 best
  radio.setChannel(108); // 0 to 127 // 108 above wifi freq bands
  radio.setPALevel (RF24_PA_LOW); //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.openWritingPipe(address);
  radio.stopListening();//module = transmitter
  
  //TODO : start comm ack 
  //const char ack[] = "START";
  //while(! radio.write(&ack, sizeof(ack))){
    //digitalWrite(led,LOW);}
  //turn LED outside of the box ON to signify ready to go and that box is ON
  //digitalWrite(led,HIGH);

  pinMode(FWDbutt, INPUT);
  pinMode(BWDbutt, INPUT);
}

void loop()
{
  int fwdState = digitalRead(FWDbutt);
  int bwdState = digitalRead(BWDbutt);

  const char fwd[] = "fwd";
  const char bwd[] = "bwd";
  char message[] = "fwd";
  boolean send = false;
  
  if(fwdState != last_fwd && fwdState == HIGH){
      strcpy(message, fwd);
      send = true;
      Serial.println("forward pressed");
      delay(200);
  }
  last_fwd = fwdState;
  
  if(bwdState != last_bwd && bwdState == HIGH){
      strcpy(message, bwd);
      send = true;
      Serial.println("backward pressed");
      delay(200);
  }
  last_bwd = bwdState;

  if(send == true){
    if(! radio.write(&message, sizeof(message))){
        Serial.println("Could not transmit");
    }else{
      Serial.println(message);
      send = false;
    }
  }
}