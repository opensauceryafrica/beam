// Include the correct display library
// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <PZEM004Tv30.h>

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
PZEM004Tv30 pzem(D5, D6); // Software Serial pin 5 (RX) & 6 (TX)
#define relay D7

void setup() 
{
  Serial.begin(9600);
  Serial.println("Hello World!!");
  
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, "PHY/17/4871");
  // write the buffer to the display
  display.display();
  pinMode(relay, OUTPUT);
  delay(2000);
}

void loop() 
{
  digitalWrite(relay, HIGH);
  // clear the display
  display.clear();
  
  // PZEM-004t Reading
  float voltage = pzem.voltage();

  if (voltage != NAN) 
  {
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println("V");

    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 20, "Voltage: " + String(voltage) + "V");
  } 
  else 
  {
    Serial.println("Error reading voltage");
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 20,"Error reading voltage");
  }

  float current = pzem.current();
  if (current != NAN) 
  {
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println("A");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 30, "Current: " + String(current) + "A");
  } 
  else 
  {
    Serial.println("Error reading current");
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 30,"Error reading current");
  }

  float power = pzem.power();
  if (current != NAN) 
  {
    Serial.print("Power: ");
    Serial.print(power);
    Serial.println("W");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 40, "Power: " + String(power) + "W");
  } 
  else 
  {
    Serial.println("Error reading power");
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 40,"Error reading power");
  }

  float energy = pzem.energy();
  if (current != NAN) 
  {
    Serial.print("Energy: ");
    Serial.print(energy, 3);
    Serial.println("kWh");
    
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 50, "Energy: " + String(energy) + "Kwh");

  } 
  else 
  {
    Serial.println("Error reading energy");
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 50,"Error reading energy");
  }

  float frequency = pzem.frequency();
  if (current != NAN) {
    Serial.print("Frequency: ");
    Serial.print(frequency, 1);
    Serial.println("Hz");
  } else {
    Serial.println("Error reading frequency");
  }

  float pf = pzem.pf();
  if (current != NAN) {
    Serial.print("PF: ");
    Serial.println(pf);
  } else {
    Serial.println("Error reading power factor");
  }

  Serial.println();
  display.display();
  delay(2000);
}
