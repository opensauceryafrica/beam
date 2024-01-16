#include <PZEM004Tv30.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


PZEM004Tv30 pzem(D5, D6); // Software Serial pin 5 (RX) & 6 (TX)
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() 
{
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  delay(2000);
  lcd.clear();
}

void loop()
{
  float voltage = pzem.voltage();
  lcd.clear();
  if (voltage != NAN) 
  {
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println("V");
    lcd.setCursor(0,0);
    lcd.print("Voltage:");
    lcd.print(voltage);
    lcd.print("v");
  } else {
    Serial.println("Error reading voltage");
    lcd.setCursor(0,0);
    lcd.print("Error reading voltage");
  }

  float current = pzem.current();
  if (current != NAN) {
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println("A");
    lcd.setCursor(0,1);
    lcd.print("Current:");
    lcd.print(current);
    lcd.print("A");
  } else {
    Serial.println("Error reading current");
    lcd.setCursor(0,1);
    lcd.print("Error reading current");
  }

  float power = pzem.power();
  if (current != NAN) {
    Serial.print("Power: ");
    Serial.print(power);
    Serial.println("W");
    lcd.setCursor(0,2);
    lcd.print("Power:");
    lcd.print(power);
    lcd.print("W");
  } else {
    Serial.println("Error reading power");
    lcd.setCursor(0,2);
    lcd.print("Error reading power");
  }

  float energy = pzem.energy();
  if (current != NAN) {
    Serial.print("Energy: ");
    Serial.print(energy, 3);
    Serial.println("kWh");
    lcd.setCursor(0,3);
    lcd.print("Energy:");
    lcd.print(energy);
    lcd.print("kWh");

  } else {
    Serial.println("Error reading energy");
    lcd.setCursor(0,3);
    lcd.print("Error reading energy");
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
  delay(2000);
}