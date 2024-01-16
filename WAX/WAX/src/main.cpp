#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "Engineer-D";
const char* WIFI_PASSWORD = "Dheeeeey";

const char* MQTT_HOST = "5761c8bf86f142c2957642a199137f12.s2.eu.hivemq.cloud";
const int MQTT_PORT = 8883;
const char* MQTT_CLIENT_ID = "mqttjs_34f95deb";
const char* MQTT_USER = "opensaucerer";
const char* MQTT_PASSWORD = "u@n6EFCeBJPHD6";

const char *TOPIC = "balance_empty";

WiFiClient client;
PubSubClient mqttClient(client);

void callback(char* topic, byte* payload, unsigned int length)
{
    payload[length] = '\0';
    int value = String((char*) payload).toInt();

    Serial.println(topic);
    Serial.println(value);
}

void setup()
{
    Serial.begin(9600);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to Wi-Fi");

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCallback(callback);

    while (!client.connected()) {
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
            Serial.println("Connected to MQTT broker");
        } else {
            delay(500);
            Serial.print(".");
        }
    }

    mqttClient.subscribe(TOPIC);
}

void loop()
{
    mqttClient.loop();
}

// /********************Include Library******************************/
// #include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <PubSubClient.h>

// /**************Wifi Credientials***************************/
// const char* WIFI_SSID = "Engineer-D";
// const char* WIFI_PASSWORD = "Dheeeeey";

// /*******************MQTT Variables***********************/
// const char* MQTT_HOST = "5761c8bf86f142c2957642a199137f12.s2.eu.hivemq.cloud";
// const int MQTT_PORT = 8883;
// const char* MQTT_CLIENT_ID = "mqttjs_34f95deb";
// const char* MQTT_USER = "opensaucerer";
// const char* MQTT_PASSWORD = "u@n6EFCeBJPHD6";

// const char* TOPIC1 = "balance_empty";
// const char* TOPIC2 = "balance_low";
// const char* TOPIC3 = "balance_change";

// WiFiClient espClient;
// PubSubClient client(espClient);

// void callback(char* topic, byte* payload, unsigned int length)
// {
//     payload[length] = '\0';
//     int value = String((char*) payload).toInt();

//     Serial.println(topic);
//     Serial.println(value);
// }

// void setup()
// {
//     Serial.begin(115200);

//     WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }

//     Serial.println("Connected to Wi-Fi");

//     client.setServer(MQTT_HOST, MQTT_PORT);
//     client.setCallback(callback);

//     while (!client.connected()) {
//         if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD )) {
//             Serial.println("Connected to MQTT broker");
//             client.subscribe(TOPIC1);
//             client.subscribe(TOPIC2);
//             client.subscribe(TOPIC3);
            
//         } else {
//             delay(500);
//             Serial.print(".");
//         }
//     }
// }

// void loop()
// {
//     client.loop();
// }