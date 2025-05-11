const int relaypin = 2;
void setup() {
  Serial.begin(115200);
  pinMode(relaypin,OUTPUT);
  digitalWrite(5, HIGH);
}
void loop(){
  digitalWrite(relaypin, LOW);
  Serial.println("on");
  delay(2000);
  
  digitalWrite(relaypin, HIGH);
  Serial.println("off");
  delay(2000);
}
  