// Include the correct display library
// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <PZEM004Tv30.h>

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
PZEM004Tv30 pzem(D5, D6); // Software Serial pin 5 (RX) & 6 (TX)
#define relay D7
#define led D8

float tariff = 5.0;
unsigned long lastTariffTime = 0;  // the last time the tariff reduced
unsigned long tariffDelay = 5000;  // the aim is to reduce the tariff every 5 secs

void setup() 
{
  Serial.begin(9600);
  Serial.println("Hello World!!");
  pinMode(relay, OUTPUT);digitalWrite(relay, LOW);
  pinMode(led, OUTPUT);digitalWrite(led, LOW);
  
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();

  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, "PHY/17/4871");
  // write the buffer to the display
  display.display();
  delay(2000);
}

void loop() 
{
  // clear the display
  display.clear();
  if (tariff >= 0.5)
  {
    //Turn on relay
    digitalWrite(relay, HIGH);

    //Check if balance is almost low
    if (tariff < 1.5) digitalWrite(led, HIGH);

    // check if balance is okay
    if (tariff >= 1.5) digitalWrite(led, LOW);

    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(60, 0, "Tariff: " + String(tariff) + "SOL");

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
    delay(1000);
  }
  else
  {
    //Turn on relay
    digitalWrite(relay, LOW);
    //if balance is  low
    digitalWrite(led, HIGH);
  }

  if ((millis() - lastTariffTime) > tariffDelay) 
  {
    tariff -= 0.5;
    lastTariffTime = millis();

    if (tariff < 0) tariff = 0.00;

    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(60, 0, "Tariff: " + String(tariff) + "SOL");
    display.display();
  }
  display.clear();
  // Serial.print("Tariff: ");
  // Serial.println(tariff);
}
