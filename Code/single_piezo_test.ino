// PROJECT VECTOR - Day 1, Step 8
// Single Piezo Sensor Test (breadboard, A0)
// Confirms one sensor + 1M-ohm resistor gives readable values.
 
const int PIEZO_PIN  = A0;
const int NOISE_FLOOR = 5;
 
void setup() {
  Serial.begin(115200);
  Serial.println("Single Piezo Test - tap the sensor gently");
}
 
void loop() {
  int reading = analogRead(PIEZO_PIN);
  if (reading > NOISE_FLOOR) {
    Serial.print("Piezo value: ");
    Serial.println(reading);
  }
  delay(1000);
}

