// Define the motor control pins
#define ENA 6
#define IN1 8
#define IN2 7

const int speed = 200;

void setup() {
  // Set the motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Move the motor in one direction
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed); // Full speed
  Serial.println("one");

  delay(1000); // Wait for 2 seconds

  // Stop the motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  delay(1000); // Wait for 1 second

  // Move the motor in the other direction
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed); // Full speed

  delay(1000); // Wait for 2 seconds

  // Stop the motor
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  delay(1000); // Wait for 1 second
}
