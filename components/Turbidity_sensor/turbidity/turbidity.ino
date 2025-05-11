#define SENSOR_PIN 34  // Use any ADC-capable pin on ESP32

int read_ADC;
int ntu;

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);
  delay(500);
  
  Serial.println("Welcome to Turbidity Sensor");
}

void loop() {
  read_ADC = analogRead(SENSOR_PIN);

  // Clamp the value to max 208 like in your original code
  if (read_ADC > 208) read_ADC = 208;

  // Map ADC reading to NTU (you can calibrate this for more accuracy)
  ntu = map(read_ADC, 0, 208, 300, 0);

  Serial.print("Turbidity: ");
  Serial.print(ntu);
  Serial.print(" NTU | ");

  if (ntu < 10) {
    Serial.println("Water Very Clean");
  } else if (ntu >= 10 && ntu < 30) {
    Serial.println("Water Normally Clean");
  } else {
    Serial.println("Water Very Dirty");
  }

  delay(200);
}
