#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Ethernet.h> // Include Ethernet library for web server communication

// Define IR sensor pins
const int irSensorPins[4] = {2, 3, 4, 5}; // Adjust pin numbers accordingly
const int gateOpenSensorPin = 6;
const int gateCloseSensorPin = 7;

// Define LED pins
const int ledPins[4] = {8, 9, 10, 11}; // Adjust pin numbers accordingly

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

// Ethernet settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC address of Arduino
EthernetServer server(80); // Port 80 for HTTP

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Initialize gate servo
  gateServo.attach(servoPin);
  gateServo.write(gateClosedAngle);

  // Initialize IR sensors
  for (int i = 0; i < 4; i++) {
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
  
  // Initialize Ethernet connection
  Ethernet.begin(mac);
  server.begin();
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
  
  // Check for incoming client requests
  EthernetClient client = server.available();
  if (client) {
    // Handle client request
    processClientRequest(client);
    client.stop();
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
  lcd.print("Parking Status:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 4; i++) {
    if (digitalRead(irSensorPins[i]) == LOW) {
      lcd.print("Slot ");
      lcd.print(i + 1);
      lcd.print(": Empty");
    } else {
      lcd.print("Slot ");
      lcd.print(i + 1);
      lcd.print(": Booked");
    }
    lcd.setCursor(0, 1 + i);
  }
  delay(1000); // Adjust delay if necessary
}

void processClientRequest(EthernetClient client) {
  // Read the HTTP request
  String request = "";
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      request += c;
      if (request.endsWith("\r\n\r\n")) {
        break;
      }
    }
  }
  
  // Check if the request is for booking a parking slot
  if (request.indexOf("GET /book_slot") != -1) {
    // Parse the slot number from the request
    int slotNumber = request.substring(request.indexOf("=") + 1, request.indexOf(" ")).toInt();
    
    // Update the parking status
    if (emptySlots > 0 && digitalRead(irSensorPins[slotNumber - 1]) == LOW) {
      emptySlots--;
      digitalWrite(ledPins[slotNumber - 1], LOW); // Turn off LED for occupied slot
      displayParkingStatus();
      // Send HTTP response
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Slot booked successfully!");
    } else {
      // Send HTTP response
      client.println("HTTP/1.1 400 Bad Request");
      client.println("Content-Type: text/plain");
      client.println();
      client.println("Slot is already booked or parking is full!");
    }
  } else {
    // Send HTTP response for invalid request
    client.println("HTTP/1.1 400 Bad Request");
    client.println("Content-Type: text/plain");
    client.println();
    client.println("Invalid request!");
  }
}
