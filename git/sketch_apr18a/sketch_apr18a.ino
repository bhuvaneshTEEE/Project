cpp
#include <DHT.h>

#define DHTPIN 2          // DHT11 pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int smokePin = A0;  // MQ-2 sensor
const int buzzerPin = 3;  // Buzzer
const int redPin = 4;     // RGB LED red
const int greenPin = 5;   // RGB LED green
const int bluePin = 6;    // RGB LED blue
const int relayPin = 7;   // Relay for pump/fan
int smokeThreshold = 300; // Adjust based on MQ-2 readings
float tempThreshold = 50.0; // Fire if >50°C

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  dht.begin();
  Serial.begin(9600);

  // Initial safe state
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  digitalWrite(bluePin, LOW);
}

void loop() {
  // Read sensors
  int smokeLevel = analogRead(smokePin);
  float temp = dht.readTemperature();

  // Check for fire
  if (smokeLevel > smokeThreshold || temp > tempThreshold) {
    // Fire detected
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);
    digitalWrite(bluePin, LOW);
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);

    // Suppress fire
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, HIGH);
    digitalWrite(relayPin, HIGH); // Turn on pump/fan
    delay(10000);                // Suppress for 10 seconds
    digitalWrite(relayPin, LOW);  // Turn off
    digitalWrite(bluePin, LOW);
  } else {
    // Safe state
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);
    digitalWrite(bluePin, LOW);
  }

  // Debug output
  Serial.print("Smoke: ");
  Serial.print(smokeLevel);
  Serial.print(" | Temp: ");
  Serial.println(temp);

  delay(1000); // Check every second
}
