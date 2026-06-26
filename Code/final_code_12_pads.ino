// ============================================================
//  PROJECT VECTOR — Firmware FINAL (no buzzer, no button)
//  Arduino Mega 2560
//  12 pads · 24 LEDs per ring · 4 active at once
//  Piezos: A0–A11
//  LED data: pin 6 · LED power: 5V external
//  Game starts automatically after calibration
//  Game ends after 25 attempts then auto-restarts
// ============================================================

#include <Adafruit_NeoPixel.h>

#define LED_PIN  6

const int NUM_PADS      = 12;
const int LEDS_PER_RING = 24;
const int NUM_LEDS      = NUM_PADS * LEDS_PER_RING;
const int ACTIVE_COUNT  = 4;
const int TOTAL_ROUNDS  = 25;

const int piezoPins[NUM_PADS] = {
  A0, A1, A2,  A3,
  A4, A5, A6,  A7,
  A8, A9, A10, A11
};

#define BRIGHTNESS  60

int SPIKE_THRESHOLD = 80;
int DEBOUNCE_MS     = 300;

const unsigned long PAD_TIMEOUT_MS   = 4000;
const unsigned long RESTART_DELAY_MS = 3000;  // pause before auto-restart

int baseline[NUM_PADS];

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t COL_TARGET;
uint32_t COL_HIT;
uint32_t COL_WRONG;
uint32_t COL_MISS;
uint32_t COL_GOLD;
uint32_t COL_OFF;

enum GameState { PLAYING, GAME_OVER_ANIM };
GameState state = PLAYING;

bool          activePads[NUM_PADS];
int           points        = 0;
int           totalHits     = 0;
int           totalAttempts = 0;
unsigned long padLitTime    = 0;
unsigned long lastHitTime[NUM_PADS];

void lightRing(int pad, uint32_t color) {
  int start = pad * LEDS_PER_RING;
  for (int i = 0; i < LEDS_PER_RING; i++)
    strip.setPixelColor(start + i, color);
}

void clearAll() { strip.clear(); strip.show(); }

void showActiveSet() {
  for (int p = 0; p < NUM_PADS; p++)
    lightRing(p, activePads[p] ? COL_TARGET : COL_OFF);
  strip.show();
}

void sendEvent(const char* type, int pad, int hits, int attempts, int pts) {
  Serial.print(type);
  Serial.print(","); Serial.print(pad);
  Serial.print(","); Serial.print(hits);
  Serial.print(","); Serial.print(attempts);
  Serial.print(","); Serial.println(pts);
}

void calibrateBaselines() {
  Serial.println("PV_CALIBRATING");
  // Blue sweep animation while calibrating
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 30, 80));
    strip.show();
    delay(3);
  }
  for (int p = 0; p < NUM_PADS; p++) {
    long sum = 0;
    for (int s = 0; s < 100; s++) {
      sum += analogRead(piezoPins[p]);
      delay(5);
    }
    baseline[p] = sum / 100;
    Serial.print("PV_BASELINE,");
    Serial.print(p);
    Serial.print(",");
    Serial.println(baseline[p]);
  }
  clearAll();
  Serial.println("PV_CALIBRATED");
}

void activateNewSet() {
  memset(activePads, false, sizeof(activePads));
  int picked = 0;
  while (picked < ACTIVE_COUNT) {
    int c = random(0, NUM_PADS);
    if (!activePads[c]) { activePads[c] = true; picked++; }
  }
  padLitTime = millis();
  showActiveSet();
  int mask = 0;
  for (int p = 0; p < NUM_PADS; p++)
    if (activePads[p]) mask |= (1 << p);
  Serial.print("PV_ACTIVATE,");
  Serial.println(mask);
}

void gameOverSequence() {
  state = GAME_OVER_ANIM;
  clearAll();

  // Gold flash
  for (int f = 0; f < 4; f++) {
    for (int i = 0; i < NUM_LEDS; i++)
      strip.setPixelColor(i, COL_GOLD);
    strip.show();
    delay(250);
    clearAll();
    delay(120);
  }

  // Show results on rings: green = hits, red = misses
  for (int p = 0; p < NUM_PADS && p < totalHits; p++)
    lightRing(p, COL_HIT);
  for (int p = totalHits; p < NUM_PADS && p < totalAttempts; p++)
    lightRing(p, COL_MISS);
  strip.show();

  float accuracy = totalAttempts > 0
    ? (float)totalHits / totalAttempts * 100.0f : 0.0f;

  Serial.print("PV_END,");
  Serial.print(totalHits);     Serial.print(",");
  Serial.print(totalAttempts); Serial.print(",");
  Serial.print(points);        Serial.print(",");
  Serial.println(accuracy, 1);

  // Pause then auto-restart
  delay(RESTART_DELAY_MS);
  startGame();
}

void registerHit(int pad) {
  totalHits++;
  totalAttempts++;
  points += 10;
  lightRing(pad, COL_HIT);
  strip.show();
  sendEvent("PV_HIT", pad, totalHits, totalAttempts, points);
  delay(200);
  if (totalAttempts >= TOTAL_ROUNDS) { gameOverSequence(); return; }
  activateNewSet();
}

void registerWrongHit(int pad) {
  totalAttempts++;
  points = max(0, points - 5);
  lightRing(pad, COL_WRONG);
  strip.show();
  sendEvent("PV_WRONG", pad, totalHits, totalAttempts, points);
  delay(200);
  if (totalAttempts >= TOTAL_ROUNDS) { gameOverSequence(); return; }
  showActiveSet();
}

void registerMiss() {
  totalAttempts++;
  for (int p = 0; p < NUM_PADS; p++)
    if (activePads[p]) lightRing(p, COL_MISS);
  strip.show();
  sendEvent("PV_MISS", -1, totalHits, totalAttempts, points);
  delay(300);
  if (totalAttempts >= TOTAL_ROUNDS) { gameOverSequence(); return; }
  activateNewSet();
}

void startGame() {
  state         = PLAYING;
  points        = 0;
  totalHits     = 0;
  totalAttempts = 0;
  memset(activePads, false, sizeof(activePads));
  for (int i = 0; i < NUM_PADS; i++) lastHitTime[i] = 0;
  Serial.println("PV_START");
  activateNewSet();
}

void setup() {
  Serial.begin(115200);
  Serial.println("PV_BOOT");

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();

  COL_TARGET = strip.Color(0,   80,  255);
  COL_HIT    = strip.Color(0,   255, 80);
  COL_WRONG  = strip.Color(255, 100, 0);
  COL_MISS   = strip.Color(255, 0,   0);
  COL_GOLD   = strip.Color(255, 200, 0);
  COL_OFF    = strip.Color(0,   0,   0);

  randomSeed(analogRead(A12));

  // Calibrate then start immediately
  calibrateBaselines();
  startGame();
}

void loop() {
  if (state == PLAYING) {
    if (millis() - padLitTime > PAD_TIMEOUT_MS) {
      registerMiss();
      return;
    }
    for (int pad = 0; pad < NUM_PADS; pad++) {
      int raw   = analogRead(piezoPins[pad]);
      int delta = raw - baseline[pad];
      bool pastDebounce = (millis() - lastHitTime[pad]) > (unsigned long)DEBOUNCE_MS;
      if (delta > SPIKE_THRESHOLD && pastDebounce) {
        lastHitTime[pad] = millis();
        if (activePads[pad]) registerHit(pad);
        else                 registerWrongHit(pad);
        return;
      }
    }
  }
}
