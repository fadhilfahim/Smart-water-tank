#include <Wire.h>
#include <U8g2lib.h>

#include <WiFi.h>
#include <PubSubClient.h>

// ---------- WIFI and mqtt credential ----------
const char* ssid = "homewiwi";
const char* password = "Home@123";
//mqtt server
const char* mqtt_server = "test.mosquitto.org";
const char* topic = "smart/watertank";

// ---------- WIFI and mqtt initialise ----------
WiFiClient espClient;
PubSubClient client(espClient);

// ---------- DISPLAY SETUP ----------
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);

// ---------- FLOW SENSOR ----------
volatile int pulseCount = 0;
unsigned long oldTime = 0;
float flowRate = 0.0;
float totalLitres = 0.0;
#define FLOW_SENSOR_PIN 27

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

// ---------- ULTRASONIC SENSOR ----------
const int trigPin = 14;
const int echoPin = 12;
const float tankHeight = 23.55;
const float fullLevel = 17.73;
// const float sensorgap = 3;

float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return (duration * 0.034) / 2; // distance in cm
}

// ---------- TURBIDITY SENSOR ----------
#define TURBIDITY_SENSOR_PIN 34
int read_ADC;
int ntu;
String quality = "";

// ---------- RGB LED ----------
#define RED_PIN 5
#define GREEN_PIN 18
#define BLUE_PIN 19

void setColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

// Setup WiFi
void setup_wifi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}


// MQTT Reconnection
void reconnect() {
 while (!client.connected()) {
   Serial.println("Connecting to MQTT...");
   if (client.connect("ESP32_PIR")) {
     Serial.println("Connected to MQTT");
   } else {
     delay(5000);
   }
 }
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);
  
  setup_wifi(); //wifi
  client.setServer(mqtt_server, 1883); //server
  u8g2.begin(); //display

  // Flow sensor setup
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);

  // Ultrasonic sensor setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // RGB setup
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  delay(500);
  Serial.println("System Initialized");
}

// ---------- LOOP ----------
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // FLOW SENSOR MEASUREMENT
  if ((millis() - oldTime) > 1000) {
    detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / 7.5;
    float litresPerSecond = flowRate / 60.0;
    totalLitres += litresPerSecond;
    oldTime = millis();
    pulseCount = 0;
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  }

  // ULTRASONIC MEASUREMENT
  float distance = measureDistance();
  float waterLevel = (tankHeight - distance);
  if (waterLevel < 0) waterLevel = 0;
  int levelPercent = constrain((int)((waterLevel / fullLevel) * 100), 0, 100);
  if (levelPercent>95.0) levelPercent=100; 
  // TURBIDITY MEASUREMENT
  read_ADC = analogRead(TURBIDITY_SENSOR_PIN);
  if (read_ADC > 208) read_ADC = 208;
  ntu = map(read_ADC, 0, 208, 300, 0);

  if (ntu < 10) quality = "Very Clean";
  else if (ntu < 30) quality = "Normally Clean";
  else quality = "Very Dirty";

  // RGB LED CONTROL
  if (levelPercent > 90) {
    setColor(0, 255, 0);   // Green
  } else if (levelPercent > 25) {
    setColor(255, 255, 0); // Yellow
  } else {
    setColor(255, 0, 0);   // Red
  }

  // DISPLAY ON OLED
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setCursor(2, 14);
  u8g2.print("Quality: ");
  u8g2.print(quality);

  u8g2.setCursor(2, 30);
  u8g2.print("Volume: ");
  u8g2.print((int)totalLitres);
  u8g2.print("L");

  u8g2.setCursor(2, 46);
  u8g2.print("Level: ");
  u8g2.print(levelPercent);
  u8g2.print("%");

  // Vertical Progress Bar
  int barX = 110;
  int barY = 10;
  int barWidth = 10;
  int barHeight = 50;
  int filledHeight = map(levelPercent, 0, 100, 0, barHeight);
  u8g2.drawFrame(barX, barY, barWidth, barHeight);
  u8g2.drawBox(barX, barY + (barHeight - filledHeight), barWidth, filledHeight);

  u8g2.sendBuffer();

  // SERIAL OUTPUT
  Serial.print("Flow Rate: ");
  Serial.print(flowRate);
  Serial.print(" L/min | Total: ");
  Serial.print(totalLitres, 3);
  Serial.println(" L");

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.print(" cm | Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Turbidity: ");
  Serial.print(ntu);
  Serial.print(" NTU | Status: ");
  Serial.println(quality);

  Serial.print("Level %: ");
  Serial.print(levelPercent);
  Serial.println("%");

  Serial.println("---------------------------");

  delay(2000);

  // MQTT SEND
  String jsonPayload = "{";
  // jsonPayload += "\"timestamp\":" + String(millis()) + ",";
  jsonPayload += "\"water_level_cm\":" + String(waterLevel, 1) + ",";
  jsonPayload += "\"level_percent\":" + String(levelPercent) + ",";
  jsonPayload += "\"volume_used_liters\":" + String(totalLitres, 2) + ",";
  jsonPayload += "\"flow_rate_lpm\":" + String(flowRate, 2) + ",";
  jsonPayload += "\"ntu\":" + String(ntu) + ",";
  jsonPayload += "\"quality\":\"" + quality + "\"";
  jsonPayload += "}";
  client.publish("smartwater/data", jsonPayload.c_str());
}
