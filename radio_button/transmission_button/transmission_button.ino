#include <SPI.h>
#include <RF24.h>
// The transmitter is the digispark 
// The pin numbers are the ones written on the ports 

#define CE_PIN 9 //3 //digispark
#define CSN_PIN 8//4 //digispark
//create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);  // CE, CSN

const int pushButton = 5; // P5 is the only port left
int last_state;

//address through which two modules communicate.
const uint64_t address = "Butt1"; // any 5 character String or 40 bit address 

void setup()
{
  Serial.begin(9600);
  if (!radio.begin()) {
    //while (1) {}  // hold in infinite loop
    Serial.println("Could not turn radio on");
  }else{
    Serial.println("Radio successfully turned on");
  }
  // radio settings
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(4); // sizeof("Meuh")
  radio.setChannel(108); // 0 to 127
  radio.setPALevel (RF24_PA_LOW); // for now //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  
  //set the address for communication and set module as transmitter
  radio.openWritingPipe(address);
  radio.stopListening();//Set module as transmitter
  
  pinMode(pushButton, INPUT);
}

void loop()
{
  int buttonState = digitalRead(pushButton);
  
  const char text[] = "Meuh";
  if(buttonState != last_state){
    if (buttonState == HIGH){
      bool transmit = radio.write(&text, sizeof(text));
      if(transmit){
        Serial.println("Message Sent & Acknowledged");
      }
      Serial.println("Button pressed");
    }
    delay(50);
  }
  last_state = buttonState;
  
}
