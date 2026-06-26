// PROJECT VECTOR — Single Pad Complete Test
// Tests: piezo sensor, WS2812B LED ring, active buzzer
// Arduino Mega
// Piezo  → A0 (with 1MΩ pull-down to GND)
// LED    → D6 (via 330Ω resistor)
// Buzzer → D8

#include <Adafruit_NeoPixel.h>

#define PIEZO_PIN    A0
#define LED_PIN      6
#define BUZZER_PIN   8
#define NUM_LEDS     12    // change to match your ring LED count

#define THRESHOLD    150   // adjust after watching Serial Monitor
                           // resting = 0-30, tap = 200+, racket hit = 500+

Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// ── state ─────────────────────────────────────────────────
int      hitCount     = 0;
bool     waitingForHit = true;
unsigned long lastHitTime = 0;
const int DEBOUNCE_MS = 300;

// ── colours ───────────────────────────────────────────────
uint32_t BLUE   = ring.Color(0,   0,   255);  // idle / waiting
uint32_t GREEN  = ring.Color(0,   255, 0);    // correct hit
uint32_t RED    = ring.Color(255, 0,   0);    // missed / error
uint32_t GOLD   = ring.Color(255, 180, 0);    // celebrating
uint32_t OFF    = ring.Color(0,   0,   0);

// ── helpers ───────────────────────────────────────────────
void setRing(uint32_t color) {
  ring.fill(color);
  ring.show();
}

void beep(int freq, int dur) {
  tone(BUZZER_PIN, freq, dur);
  delay(dur + 20);
}

void celebrate() {
  // quick gold flash sequence
  for (int i = 0; i < 3; i++) {
    setRing(GOLD);
    beep(1500, 100);
    setRing(OFF);
    delay(80);
  }
}

// ── setup ─────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  pinMode(PIEZO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  ring.begin();
  ring.setBrightness(80);
  ring.show();

  Serial.println("====================================");
  Serial.println("PROJECT VECTOR — Single Pad Test");
  Serial.println("====================================");
  Serial.println("Waiting for first hit...");
  Serial.print("Threshold set to: ");
  Serial.println(THRESHOLD);
  Serial.println("");

  // startup animation — sweep around ring
  for (int i = 0; i < NUM_LEDS; i++) {
    ring.setPixelColor(i, BLUE);
    ring.show();
    delay(40);
  }
  delay(300);
  setRing(BLUE);   // stay blue = ready and waiting
}

// ── loop ──────────────────────────────────────────────────
void loop() {
  int raw = analogRead(PIEZO_PIN);

  // always print raw value so you can watch it in Serial Monitor
  // comment this out once calibrated — it slows the loop slightly
  Serial.print("Piezo raw: ");
  Serial.println(raw);

  // check for hit
  bool debounceOK = (millis() - lastHitTime) > DEBOUNCE_MS;

  if (raw > THRESHOLD && debounceOK) {
    lastHitTime = millis();
    hitCount++;

    Serial.println("----------------------------------");
    Serial.print("HIT DETECTED #");
    Serial.println(hitCount);
    Serial.print("Peak value: ");
    Serial.println(raw);
    Serial.println("----------------------------------");

    // feedback sequence
    setRing(GREEN);          // flash green
    beep(1200, 150);         // high beep = hit
    delay(250);

    setRing(OFF);
    delay(100);

    // every 5 hits — celebrate
    if (hitCount % 5 == 0) {
      Serial.print(">>> ");
      Serial.print(hitCount);
      Serial.println(" hits! Nice work.");
      celebrate();
    }

    setRing(BLUE);           // back to waiting
  }

  delay(10);   // 100Hz scan rate
}
