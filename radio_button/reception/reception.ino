//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN

//address through which two modules communicate.
const uint64_t address = "Butt1";

void setup()
{
  Serial.begin(9600);
  if (!radio.begin()) {
    while (1) {Serial.println("no radio");}  // hold in infinite loop
  }else{
    Serial.println("radio ok");
  }
  Serial.println("Successfull radio channel");
  radio.setPALevel(RF24_PA_LOW);
  //set the address
  radio.openReadingPipe(0, address); // 0 to 5 is the pipe to be opened 

  //Set module as receiver
  radio.startListening();
}

void loop()
{
  //Read the data if available in buffer
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
  delay(1000);
}
