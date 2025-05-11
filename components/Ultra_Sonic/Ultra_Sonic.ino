#include <WiFi.h>
#include <PubSubClient.h>

// WiFi Credentials
// const char* ssid = "nafih";            
// const char* password = "password";

// MQTT Broker
// const char* mqtt_server = "test.mosquitto.org";
// const char* distanceTopic = "esp32/distance";

// Initialize WiFi & MQTT Client
// WiFiClient espClient;
// PubSubClient client(espClient);

// Ultrasonic Sensor (HC-SR04) Pins
const int trigPin = 5;   // GPIO5 -> Trigger Pin
const int echoPin = 18;  // GPIO18 -> Echo Pin


// Set the total height of your water tank (in cm)
const float tankHeight = 100.0;  // change this to your actual tank height
const float sensorgap=10.0;



// Function to Measure Distance
float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.034) / 2;  // Convert to cm

  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // setup_wifi();
  // client.setServer(mqtt_server, 1883);
  // client.setCallback(callback);
}

void loop() {
  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();

  // Measure Distance
  float distance = measureDistance();

  // Calculate Water Level
  float waterLevel = (tankHeight - distance)+sensorgap;
  if (waterLevel < 0) waterLevel = 0; // Clamp if distance > tankHeight

  // Display
  Serial.print("Distance from sensor: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" cm");

  delay(2000); // 2 seconds delay
}
