# Efficient Vehicle Parking Management System

## Overview
Efficient Vehicle Parking Management System is an IoT project designed to streamline the process of vehicle parking. The system utilizes various IoT components to automate the parking management process and provide real-time information about parking availability.

## Components Used
- Arduino Uno
- iR sensors
- Servo Motor SG - 90
- 2x16 LCD Display with i2c module
- Breadboard
- Jumper wires

## System Setup
The system consists of the following components:
- One entry iR sensor and one exit iR sensor
- One servo motor SG - 90 acting as a gate
- Two iR sensors used to check which parking slots are empty or full

## Functionality
1. **Parking Availability Check**: When a vehicle approaches the entry iR sensor, the system checks if there are any available parking slots.
2. **Gate Control**: If parking slots are available, the gate controlled by the servo motor opens to allow the vehicle to enter.
3. **Slot Status Display**: The LCD display shows the status of each parking slot in real-time, indicating whether they are empty or full.
4. **Parking Full Notification**: If all parking slots are occupied, the LCD display shows a message indicating that the parking is full.

## Usage
- **Vehicle Entry**: When a vehicle approaches the entry point, the system checks parking availability and opens the gate if slots are available.
- **Parking Status Check**: Users can check the availability of parking slots by viewing the status displayed on the LCD display.
- **Vehicle Exit**: When a vehicle leaves the parking area, the system updates the slot status accordingly.

## Note
- This project aims to efficiently manage vehicle parking, reduce manual intervention, and provide users with real-time parking availability information.

## Authors
- Anirudha B G Somayaji
- Shivaganesh