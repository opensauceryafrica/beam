//----------------------------------- Libraries ------------------------------------
// Include the correct display library
// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
//-----------------------------------------------------------------------------------

//--------------------------------- Wifi Details ------------------------------------
const char* WIFI_SSID = "Engineer-D";//"Perfection\'s Pixel 7 Pro";
const char* WIFI_PASSWORD = "Dheeeeey"; //"pythonbite";
//-----------------------------------------------------------------------------------

//----------------------------- MQTT Variable ---------------------------------------
const char* MQTT_HOST = "5761c8bf86f142c2957642a199137f12.s2.eu.hivemq.cloud";
const int MQTT_PORT = 8883;
const char* MQTT_CLIENT_ID = "NodeMCU";
const char* MQTT_USER = "opensaucerer";
const char* MQTT_PASSWORD = "u@n6EFCeBJPHD6";

const char* TOPIC1 = "balance_empty";
const char* TOPIC2 = "balance_low";
const char* TOPIC3 = "balance_change";
const char* TOPIC4 = "get_balance";
const char* TOPIC5 = "balance_charge";
//-----------------------------------------------------------------------------------

//---------------------------------- Other Deifferences -----------------------------
#define relay D7
#define led D8

int count = 0;
float sum_energy = 0;
//unsigned long currentTime;
unsigned long prevTime;
unsigned long diff;

unsigned long lastTariffTime = 0;  // the last time the tariff reduced
unsigned long tariffDelay = 5000;  // the aim is to reduce the tariff every 5 secs

// WiFiClient espClient;
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, SDA, SCL);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
PZEM004Tv30 pzem(D5, D6); // Software Serial pin 5 (RX) & 6 (TX)
//-----------------------------------------------------------------------------------

//================================== Root Certificate ===============================
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";
//===================================================================================

//========================= MeasurementData Struct ==================================
struct MeasurementData {
    float voltage;
    float current;
    float power;
    float energy;
};

MeasurementData measurement;
//===================================================================================

//============================ Calbacks =============================================
void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';  // Ensure null-termination of the payload
    float tariff = atof((char*)payload);  // Convert payload to float
    
    String value = String((char*) payload); //.toInt()

    Serial.println(topic);
    Serial.println(value);

    if (tariff > 0.0){
      // clear the display
      display.clear();
      display.setFont(ArialMT_Plain_16);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(60, 0, "Unit: " + value);
      display.display();

      // PZEM-004t Reading and display
      displayPZEM(measurement);
      }
    else
    {
      display.clear();
      display.setFont(ArialMT_Plain_16);
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(60, 0, "Unit: " + value);
      display.setFont(ArialMT_Plain_16);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 20, F("BALANCE LOW"));
      display.drawString(64, 50, F("RECHARGE"));
      display.display();
    }
    
    if (String(topic) == "balance_change")
    {
      // Meaning you still have enough unit
      digitalWrite(led, LOW);
      //Turn on relay
      digitalWrite(relay, HIGH);
    }
    else if (String(topic) == "balance_low")
    {
      digitalWrite(led, HIGH);
      //Turn on relay
      digitalWrite(relay, HIGH);
    }
    else if (String(topic) == "balance_empty")
    {
      //Turn off relay
      digitalWrite(relay, LOW);
      //if balance is  low
      digitalWrite(led, HIGH);
    }
    else
    {
      //Turn off relay
      digitalWrite(relay, LOW);
      //if balance is  low
      digitalWrite(led, LOW);
    }
}
//====================================================================================

//========================= Publish Method ===========================================
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true));
      // Serial.println("Message publised ["+String(topic)+"]: "+payload);
}
//====================================================================================

//========================= Reconnect ================================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
      //......display
      display.clear();
      display.setFont(ArialMT_Plain_16);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(20, 5, "MQTT");
      display.drawString(64, 20, "Connected");
      // write the buffer to the display
      display.display();
      // publish to get balance
      char mqtt_message[4];
      publishMessage(TOPIC4, mqtt_message, true);
      // ... and resubscribe
      client.subscribe(TOPIC1);
      client.subscribe(TOPIC2);
      client.subscribe(TOPIC3);
      return;
    } 
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(40, 20, F("Configuring"));
    display.drawString(64, 40, F("MQTT"));
    // write the buffer to the display
    display.display();
    // Wait 5 seconds before retrying
    digitalWrite(relay, LOW);
    delay(5000);
  }
}
//====================================================================================

//==================================== Setup ==========================================
void setup() {
  Serial.begin(9600);

  //------------------Initialize LCD---------------
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  //-----------------------------------------------

  //-------------------- Other Initialiation -------
  pinMode(relay, OUTPUT); digitalWrite(relay, LOW);
  pinMode(led, OUTPUT); digitalWrite(led, LOW);
  //------------------------------------------------

  // -----------------Connecting to wifi ----------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
      display.setFont(ArialMT_Plain_16);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(20, 5, "Wifi");
      display.drawString(64, 20, "Connecting...");
      // write the buffer to the display
      display.display();
      digitalWrite(relay, LOW);
      delay(50);
  }
  //------------------------------------------------

  //-------------------ESP Setup -------------------
  #ifdef ESP8266
    espClient.setInsecure();
  #else
    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  #endif
  //------------------------------------------------

  //---------------------- MQTT Config --------------
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
  display.clear();
  while (!client.connected()) {
    if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
        display.clear();
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(20, 5, "MQTT");
        display.drawString(64, 20, "Connected");
        // write the buffer to the display
        display.display();
        // get balance
        char mqtt_message[4];
        publishMessage(TOPIC4, mqtt_message, true);
        // subscribe
        client.subscribe(TOPIC1);
        client.subscribe(TOPIC2);
        client.subscribe(TOPIC3);    

        delay(2000);  
    } else {
        display.setFont(ArialMT_Plain_16);
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(40, 20, F("Configuring"));
        display.drawString(64, 40, F("MQTT"));
        // write the buffer to the display
        display.display();
        digitalWrite(relay, LOW);
        delay(50);
    }
  }

  //-------------------- Welcome Text --------------
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 20, F("PHY/17/4871"));
  // write the buffer to the display
  display.display();
  delay(2000);
  // measurement.power = pzem.power();
  // sum_energy = measurement.power;
  //--------------------------------------------------
}
//=====================================================================================

//==================================== Loop ============================================
void loop() 
{
  //--------------Maintain connection
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //----------------------------------

  //---------------- Take Reading --------------------------c
  readPZEM();
  //--------------------------------------------------------

  //--------------- once it's 5 seconds after previous publish
  if ((millis() - lastTariffTime) > tariffDelay) 
  {
    // Calculate the average energy used
    float energy_used = sum_energy / count;
    // Convert to string to send
    char buffer[8];  // Adjust the size based on your float value
    sprintf(buffer, "%.4f", energy_used);  // "%.2f" formats the float to two decimal places
    String strValue = buffer;

    if (!isnan(measurement.power)) publishMessage(TOPIC5, strValue, true); // publish the average energy to topic5
    else{
        // get balance
        char mqtt_message[4];
        publishMessage(TOPIC4, mqtt_message, true);
    }
      

    // Reset parameters
    count = 0;
    sum_energy = 0;
    lastTariffTime = millis();
  }
  else{
    //keep adding the energy consumed together and increase count
    if (!isnan(measurement.power)) {
      sum_energy += measurement.power;
      count ++;
    }     
  }
  //----------------------------------
  delay(1000);
}
//=====================================================================================

//============================== Measure Date =========================================
void readPZEM()
{
  // PZEM-004t Reading
  measurement.voltage = pzem.voltage();
  measurement.current = pzem.current();
  measurement.power = pzem.power();
  measurement.energy = pzem.energy();
}
//======================================================================================

//============================== Read PZEM and Display Values ===========================
void displayPZEM(const MeasurementData& measurement)
{
  if (measurement.voltage != NAN) {
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 15, "Voltage: " + String(measurement.voltage) + "V");
  } 
  else {
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 20,F("Error reading voltage"));
  }

  if (measurement.current != NAN) {
    //--------------- Dislay Current
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 30, "Current: " + String(measurement.current) + "A");
    //--------------- Diplay power
    display.drawString(0, 45, "Power: " + String(measurement.power) + "W");
  } 
  else {
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(0, 40,F("Error reading Parameters"));
  }
  display.display();
}
//==========================================================================================