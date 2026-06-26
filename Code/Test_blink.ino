// PROJECT VECTOR - Day 1, Step 6
// Blink Test - confirms board, drivers, and USB connection
// before anything else is wired up.
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Project Vector - Blink Test");
}
 
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  Serial.println("Blink OK");
}

