#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include<Servo.h>

// Define IR sensor pins
const int irSensorPins[6] = {2, 3, 4, 5, 6, 7}; // Adjust pin numbers accordingly
const int gateOpenSensorPin = 8;
const int gateCloseSensorPin = 9;

// Define LED pins
const int ledPins[4] = {10, 11, 12, 13}; // Adjust pin numbers accordingly

// Define servo motor pin
const int servoPin = A0; // Adjust pin number accordingly

// Define servo motor positions
const int gateOpenAngle = 0;
const int gateClosedAngle = 90;

// Define LCD properties
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address if necessary

// Define servo object
Servo gateServo;

// Variables to track parking slots
int emptySlots = 4;

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Initialize gate servo
  gateServo.attach(servoPin);
  gateServo.write(gateClosedAngle);

  // Initialize IR sensors
  for (int i = 0; i < 6; i++) {
    pinMode(irSensorPins[i], INPUT);
  }

  // Initialize gate open/close sensors
  pinMode(gateOpenSensorPin, INPUT);
  pinMode(gateCloseSensorPin, INPUT);

  // Initialize LED pins
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH); // Turn on all LEDs initially
  }
}

void loop() {
  // Check if gate needs to be opened
  if (digitalRead(gateOpenSensorPin) == HIGH) {
    openGate();
  }

  // Check if gate needs to be closed
  if (digitalRead(gateCloseSensorPin) == HIGH) {
    closeGate();
  }

  // Check IR sensors for parked cars
  for (int i = 0; i < 4; i++) {
    if (digitalRead(irSensorPins[i]) == HIGH) {
      emptySlots--;
      digitalWrite(ledPins[i], LOW); // Turn off LED for occupied slot
      displayParkingStatus();
    }
  }
}

void openGate() {
  gateServo.write(gateOpenAngle);
  delay(1000); // Adjust delay if necessary
}

void closeGate() {
  gateServo.write(gateClosedAngle);
  delay(1000); // Adjust delay if necessary
}

void displayParkingStatus() {
  lcd.clear();
  if (emptySlots == 0) {
    lcd.print("Parking is full");
  } else {
    lcd.print("Empty slots: ");
    for (int i = 0; i < 4; i++) {
      if (digitalRead(irSensorPins[i]) == LOW) {
        lcd.print(i + 1); // Display slot number starting from 1
        lcd.print(" ");
      }
    }
    lcd.print("\nFull slots: ");
    for (int i = 0; i < 4; i++) {
      if (digitalRead(irSensorPins[i]) == HIGH) {
        lcd.print(i + 1); // Display slot number starting from 1
        lcd.print(" ");
      }
    }
  }
  delay(1000); // Adjust delay if necessary
}
