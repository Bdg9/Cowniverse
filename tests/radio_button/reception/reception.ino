//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
// The receiver is the central arduino

#define CE_PIN 3
#define CSN_PIN 7
//create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);

//address through which two modules communicate.
const uint64_t address = "Butt1";

void setup()
{
  Serial.begin(9600);
  if (!radio.begin()) {
      Serial.println("Could not turn radio on");  
  }else{
    Serial.println("Radio successfully turned on");
  }
  // radio settings
  //radio.setPayloadSize(6); // sizeof("Meuh")
  radio.setChannel(108); // 0 to 127
  radio.setPALevel (RF24_PA_LOW); // for now //transmitter power level. To choose RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //exchange rate. To choose RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  
  //set the address for communication and set module as receiver
  radio.openReadingPipe(0, address); // 0 to 5 is the pipe to be opened 
  radio.startListening();
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available()){
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
    //Serial.println("tudeconnes_ou_quoi");
  }

  delay(500);// delay to be changed depending on how often you want to check if the button was pressed 
}
