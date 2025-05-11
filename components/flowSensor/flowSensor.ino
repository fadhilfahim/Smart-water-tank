volatile int pulseCount = 0;
unsigned long oldTime = 0;
float flowRate;       // in L/min
float totalLitres = 0;
// float calibrationFactor =0.958;  

#define FLOW_SENSOR_PIN 27

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(115200);
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
}

void loop() {
  if ((millis() - oldTime) > 1000) { // Every second
    detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));

    // Flow rate in L/min
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / 7.5;
    // flowRate *= calibrationFactor;

    // Convert flow rate to volume per second, then add to total
    float litresPerSecond = flowRate / 60.0;
    totalLitres += litresPerSecond;

    oldTime = millis();
    pulseCount = 0;

    Serial.print("Flow rate: ");
    Serial.print(flowRate);
    Serial.print(" L/min\t");

    Serial.print("Total: ");
    Serial.print(totalLitres, 3);  // up to 3 decimal places
    Serial.println(" L");

    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  }
}
