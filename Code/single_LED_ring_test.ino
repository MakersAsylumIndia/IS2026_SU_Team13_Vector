// PROJECT VECTOR - Day 1, Step 10
// Single LED Ring Test (one 12-LED WS2812B ring on D6)
// Cycles red, green, blue, white, then off.
 
#include <Adafruit_NeoPixel.h>
 
#define LED_PIN   6
#define NUM_LEDS  24
 
Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
 
void fillColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    ring.setPixelColor(i, color);
  }
  ring.show();
}
 
void setup() {
  ring.begin();
  ring.setBrightness(80);   // keep current draw safe on bench power
  ring.show();
}
 
void loop() {
  fillColor(ring.Color(255, 0, 0));     // red
  delay(800);
  fillColor(ring.Color(0, 255, 0));     // green
  delay(800);
  fillColor(ring.Color(0, 0, 255));     // blue
  delay(800);
  fillColor(ring.Color(255, 255, 255)); // white
  delay(800);
  fillColor(ring.Color(0, 0, 0));       // off
  delay(800);
}

