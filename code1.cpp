#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_INA219.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_INA219 ina219;

Servo solarServo;

// LDR pins
int ldrLeft = 2;
int ldrRight = 3;

// Servo pin
int servoPin = 9;

// Servo angle
int angle = 90;

void setup() {

  pinMode(ldrLeft, INPUT);
  pinMode(ldrRight, INPUT);

  solarServo.attach(servoPin);
  solarServo.write(angle);

  ina219.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.display();
}

void loop() {

  // Read LDR states
  int leftState = digitalRead(ldrLeft);
  int rightState = digitalRead(ldrRight);

  // Move toward LEFT light
  if (leftState == 1 && rightState == 0) {

    angle = angle + 1;

    if (angle > 180)
      angle = 180;

    solarServo.write(angle);
  }

  // Move toward RIGHT light
  if (rightState == 1 && leftState == 0) {

    angle = angle - 1;

    if (angle < 0)
      angle = 0;

    solarServo.write(angle);
  }

  // Read INA219
  float voltage = ina219.getBusVoltage_V() +
                  (ina219.getShuntVoltage_mV() / 1000.0);

  float current = ina219.getCurrent_mA();

  // Time since startup
  unsigned long seconds = millis() / 1000;

  // OLED display
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Voltage
  display.setCursor(0, 0);
  display.print("Voltage: ");
  display.print(voltage, 2);
  display.println(" V");

  // Current
  display.setCursor(0, 16);
  display.print("Current: ");
  display.print(current, 3);
  display.println(" mA");

  // Angle
  display.setCursor(0, 32);
  display.print("Angle: ");
  display.print(angle);
  display.println((char)247);

  // Time
  display.setCursor(0, 48);
  display.print("Time: ");
  display.print(seconds);
  display.print(" s");

  display.display();

  delay(20);
}