/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-mqtt-publish-bme680-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>



#define WIFI_SSID "TALKTALK77D706"
#define WIFI_PASSWORD "DWTJF8G9"

#define LED 2

// Raspberry Pi Mosquitto MQTT Broker
#define MQTT_HOST IPAddress(192, 168, 1, 11)
// For a cloud MQTT broker, type the domain name
//#define MQTT_HOST "example.com"
#define MQTT_PORT 1883

// Temperature MQTT Topics
#define MQTT_PUB_TEMP "esp/bme680/temperature"
#define MQTT_PUB_HUM  "esp/bme680/humidity"
#define MQTT_PUB_PRES "esp/bme680/pressure"
#define MQTT_PUB_GAS  "esp/bme680/gas"
#define MQTT_SUS_BT1  "esp/bme680/button1"

/*#define BME_SCK 14
#define BME_MISO 12
#define BME_MOSI 13
#define BME_CS 15*/


//Adafruit_BME680 bme(BME_CS); // hardware SPI
//Adafruit_BME680 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

// Variables to hold sensor readings
float temperature;
float humidity;
float pressure;
float gasResistance;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

void getBME680Readings(){
  // Tell BME680 to begin measurement.
  temperature = 50;
  pressure = 100;
  humidity = 200;
  gasResistance = 300;
}

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);

  uint16_t packetIdSub = mqttClient.subscribe(MQTT_SUS_BT1, 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);

}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}
void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);

  if (String(topic) == MQTT_SUS_BT1) {
    Serial.println("Changing output **************** ");
    if (digitalRead(LED)==HIGH){
      digitalWrite(LED,LOW);
    }else
    {
      digitalWrite(LED,HIGH);
    }
  }



}

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(LED,OUTPUT);
 
  
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  // If your broker requires authentication (username and password), set them below
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");
  connectToWifi();

 

}

void loop() {
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    
    getBME680Readings();
    Serial.println();
    Serial.printf("Temperature = %.2f ºC \n", temperature);
    Serial.printf("Humidity = %.2f % \n", humidity);
    Serial.printf("Pressure = %.2f hPa \n", pressure);
    Serial.printf("Gas Resistance = %.2f KOhm \n", gasResistance);
    
    // Publish an MQTT message on topic esp/bme680/temperature
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temperature).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_TEMP, packetIdPub1);
    Serial.printf("Message: %.2f \n", temperature);

    // Publish an MQTT message on topic esp/bme680/humidity
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(humidity).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
    Serial.printf("Message: %.2f \n", humidity);

    // Publish an MQTT message on topic esp/bme680/pressure
    uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_PRES, 1, true, String(pressure).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_PRES, packetIdPub3);
    Serial.printf("Message: %.2f \n", pressure);

    // Publish an MQTT message on topic esp/bme680/gas
    uint16_t packetIdPub4 = mqttClient.publish(MQTT_PUB_GAS, 1, true, String(gasResistance).c_str());
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_GAS, packetIdPub4);
    Serial.printf("Message: %.2f \n", gasResistance);

    

  }
}
