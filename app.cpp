#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myservo;

#define ir_enter 2
#define ir_back  4
#define ir_car1 5
#define ir_car2 6

int S1 = 0, S2 = 0;
int flag1 = 0, flag2 = 0;
int slot = 2;


void setup() {
  Serial.begin(9600);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);

  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);

  myservo.attach(3);
  myservo.write(90);

  lcd.init(); 
  lcd.backlight(); 

  Read_Sensor();

  lcd.setCursor(0, 0);
  lcd.print("   Vehicle    ");
  lcd.setCursor(0, 1);
  lcd.print("Parking System");
  delay(1000);

  lcd.clear();
  lcd.clear();
}

void loop() {
  Read_Sensor();
  
  lcd.setCursor(0, 0);
  lcd.print("S1: ");
  lcd.print(S1 == 1 ? "Full  " : "Empty ");

  lcd.setCursor(0, 1);
  lcd.print("S2: ");
  lcd.print(S2 == 1 ? "Full  " : "Empty ");

  if (digitalRead(ir_enter) == LOW && flag1 == 0) {
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(180);
        slot = slot - 1;
      }
    } else {
      displayFull();
      lcd.clear();
    }
  }

  if (digitalRead(ir_back) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(180);
      slot = slot + 1;
    }
  }

  if (flag1 == 1 && flag2 == 1) {
    delay(1000);
    myservo.write(90);

    flag1 = 0;
    flag2 = 0;
  }
  delay(1);

}

void Read_Sensor() {
  S1 = 0, S2 = 0;

  if (digitalRead(ir_car1) == 0) {
    S1 = 1;
  }
  if (digitalRead(ir_car2) == 0) {
    S2 = 1;
  }
}

void displayFull() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    SORRY :(    ");
  lcd.setCursor(0, 1);
  lcd.print("  Parking Full  ");
  delay(1000);
}
