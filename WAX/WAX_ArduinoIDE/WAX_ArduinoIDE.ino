/********************Include Library******************************/
// #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

/**************Wifi Credientials***************************/
const char* WIFI_SSID = "Perfection\'s Pixel 7 Pro";
const char* WIFI_PASSWORD = "pythonbite";

/*******************MQTT Variables***********************/
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

#define red D6
#define green D7
#define yellow D8

unsigned long lastTime = 0;
unsigned long server_Delay = 5000; //1000;
//unsigned long currentTime;
unsigned long prevTime;
unsigned long diff;

// WiFiClient espClient;
WiFiClientSecure espClient;
PubSubClient client(espClient);

// NTP Client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

/****** root certificate *********/

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

void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    String value = String((char*) payload); //.toInt()

    Serial.println(topic);
    Serial.println(value);
    
    if (String(topic) == "balance_change")
    {
      digitalWrite(red, LOW);
      digitalWrite(green, HIGH);
      digitalWrite(yellow, LOW);
//      char mqtt_message[128];
//      publishMessage(TOPIC5, mqtt_message, true);
    }
    else if (String(topic) == "balance_low")
    {
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, HIGH);
//      char mqtt_message[128];
//      publishMessage(TOPIC5, mqtt_message, true);
    }
    else if (String(topic) == "balance_empty")
    {
      digitalWrite(red, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      lastTime = 0;
    }
    else
    {
      digitalWrite(red, LOW);
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
    }

}

/**** Method for Publishing MQTT Messages **********/
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}

void setup()
{
    Serial.begin(115200);

    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(yellow, OUTPUT);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to Wi-Fi");

    #ifdef ESP8266
    espClient.setInsecure();
    #else
      espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
    #endif

    // Configure NTP client
    timeClient.begin();
    timeClient.setTimeOffset(0);

    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(callback);

    while (!client.connected()) {
        if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
            Serial.println("Connected to MQTT broker");
            client.subscribe(TOPIC1);
            client.subscribe(TOPIC2);
            client.subscribe(TOPIC3);
            
        } else {
            delay(500);
            Serial.print(".");
        }
    }

    char mqtt_message[128];
    publishMessage(TOPIC4, mqtt_message, true);
}

void loop()
{
    client.loop();

    // Get current time from NTP server
    timeClient.update();
  
    // Format the time as a string
    String currentTime = timeClient.getFormattedTime();

    if ((millis() - lastTime) >= server_Delay) 
    { 
      Serial.print("Time: ");
      Serial.println(currentTime.c_str());

      if (digitalRead(yellow) == HIGH || digitalRead(green) == HIGH)
      {
//        unsigned long currentTime = timeClient.getEpochTime();
        diff = (millis() - lastTime)/1000;
        String message = "Light has been powered for " + String(diff) + " Secs";
        Serial.println(message);
        publishMessage(TOPIC5, String(diff), true);
      }
      lastTime = millis();
    }


}