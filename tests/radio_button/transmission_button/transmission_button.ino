#include <SPI.h>
#include <RF24.h>
// The transmitter is the digispark
// The pin numbers are the ones written on the ports 

#define CE_PIN 9//3
#define CSN_PIN 8//4
//create an RF24 object
RF24 radio(CE_PIN, CSN_PIN);  // CE, CSN

const int pushButton = 5; // P5 is the only port left
const int led = 5;


//address through which two modules communicate.
const uint64_t address = "Butt1"; // any 5 character String or 40 bit address 

void setup()
{
  Serial.begin(9600);
  if (!radio.begin()) {
    while (1) {}  // hold in infinite loop
  }
  Serial.println("Successfull radio channel");
  radio.setPALevel(RF24_PA_LOW);
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();
  pinMode(pushButton, INPUT);
}

void loop()
{
  int buttonState = digitalRead(pushButton);
  
  const char text[] = "Button";
  if(buttonState == HIGH){
    radio.write(&text, sizeof(text));
    Serial.println("Button pressed");
  }
  delay(100);

  }
