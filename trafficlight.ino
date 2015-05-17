#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <stdio.h>

#include "config.h"

const char* ssid = "muenchen.freifunk.net";

#define RED    5
#define YELLOW 4
#define GREEN   0


WiFiClient wifiClient;
PubSubClient client(MQTT_HOST, MQTT_PORT, callback, wifiClient);

void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.println("Callback: ");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  Serial.println(payload[0]);
  
  String topicStr = String(topic);
  
  
  int pin;
  if (topicStr == "mumalab/room/trafficlight/red") {
    pin = RED;
  } else if (topicStr == "mumalab/room/trafficlight/yellow") {
    pin = YELLOW;
  } else if (topicStr == "mumalab/room/trafficlight/green") {
    pin = GREEN;
  } else {
    return;
  }
  
  Serial.print("PIN: ");
  Serial.println(pin);
  
  if (payload[0] == '0') {
    digitalWrite(pin, HIGH);
  } else if (payload[0] == '1') {
    digitalWrite(pin, LOW);
  }
}


void setup(void)
{
  Serial.begin(9600);
  
  WiFi.begin(ssid);
  
  Serial.println("Wifi connecting...");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected.");
  
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  
  digitalWrite(RED, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(GREEN, HIGH);
  
}
 
void loop(void)
{ 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker...");
    
    if (client.connect("trafficlight", MQTT_USER, MQTT_PASS)) {
      Serial.println("MQTT connection successfull.");
      client.publish("mumalab/room/trafficlight/status","connected");
      client.subscribe("mumalab/room/trafficlight/red");
      client.subscribe("mumalab/room/trafficlight/yellow");
      client.subscribe("mumalab/room/trafficlight/green");
    } else {
      Serial.println("MQTT connection failed.");
    }
    
    delay(500);
  }
  
  client.loop();
  delay(500);
} 
