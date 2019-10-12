/* 
 *  Display:
 *  VCC zu 3.3V
 *  GND zu GND :-)
 *  SDA zu A4
 *  SCL zu A5
 *  
 *  Ultrasonic Sensor:
 *  VCC zu 5V
 *  GND zu GND
 *  Echo zu D12
 *  Trigger zu D13
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HCSR04.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initialize sensor that uses digital pins 13 and 12.
int triggerPin = 13;
int echoPin = 12;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

// Variables for the filled bars animation on the screen
int numberOfBars = 5;
int widthOfBars = display.width() / numberOfBars;
int heightOfBars = display.height();
int filledRect = 0;
int emptyRect = 5;

void setup() {
  Serial.begin(9600);
  Serial.print("Hello");
  SSD1306_SWITCHCAPVCC; // = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();

}

void loop() {
  double distance = distanceSensor.measureDistanceCm();
  display.clearDisplay();
  if (distance > 25) {
    filledRect = 0;
    emptyRect = 5;
  } else if (distance > 20) {
    filledRect = 1;
    emptyRect = 4;
  } else if (distance > 15) {
    filledRect = 2;
    emptyRect = 3;
  } else if (distance > 10) {
    filledRect = 3;
    emptyRect = 2;
  } else if (distance > 5) {
    filledRect = 4;
    emptyRect = 1;
  } else if (distance < 5) {
    filledRect = 5;
    emptyRect = 0;
  }

  for (int i = 0; i < numberOfBars; i++) {
    for (int k = 0; k < filledRect; k++) {
      display.fillRect(k * widthOfBars, 0, widthOfBars, heightOfBars, WHITE); 
    }
    for (int l = filledRect; l < emptyRect + filledRect; l++) {
      display.drawRect(l * widthOfBars, 0, widthOfBars, heightOfBars, WHITE); 
    }  
  }
  Serial.println(distance);
  display.display();
  delay(500);
}
