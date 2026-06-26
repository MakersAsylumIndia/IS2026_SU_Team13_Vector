#include <Adafruit_NeoPixel.h>

#define STRIP_A_PIN 18
#define STRIP_B_PIN 19
#define NUM_LEDS_A 141
#define NUM_LEDS_B 58

Adafruit_NeoPixel stripA(NUM_LEDS_A, STRIP_A_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripB(NUM_LEDS_B, STRIP_B_PIN, NEO_GRB + NEO_KHZ800);

const int piezoPins[14] = {
  32, 33, 34, 35, 36,
  39, 25, 26, 27, 14,
  12, 13, 4, 15
};

const int threshold = 1500;

// Format: {A_start1, A_end1, A_start2, A_end2, B_start1, B_end1, B_start2, B_end2}
const int tileToLEDRanges[15][8] = {
  {12, 18, 19, 29, 1, 3, 4, 6},     // Tile 0
  {23, 29, 61, 67, 6, 9, 10, 12},   // Tile 1
  {61, 71, 72, 80, 12, 15, 16, 18}, // Tile 2
  {112, 117, 74, 80, 18, 20, 21, 24}, // Tile 3
  {112, 117, 118, 128, 24, 26, 27, 29}, // Tile 4
  {106, 111, 129, 134, 24, 29, 30, 35}, // Tile 5
  {80, 85, 106, 111, 18, 24, 35, 31}, // Tile 6
  {55, 60, 80, 85, 12, 18, 41, 47}, // Tile 7
  {30, 35, 55, 61, 6, 12, 47, 53}, // Tile 8
  {6, 12, 30, 35, 1, 6, 54, 58},   // Tile 9
  {1, 6, 36, 43, 54, 56, 57, 58},  // Tile 10
  {36, 43, 44, 55, 47, 50, 51, 53}, // Tile 11
  {49, 54, 87, 93, 41, 45, 46, 47}, // Tile 12
  {87, 95, 96, 105, 35, 37, 38, 41}, // Tile 13
  {100, 105, 135, 141, 29, 33, 34, 35}  // Tile 14
};

const int targetTileIndex = 7;  // << Set this manually to the tile you want the bowler to hit (0 to 14)

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 14; i++) pinMode(piezoPins[i], INPUT);

  stripA.begin(); stripA.show();
  stripB.begin(); stripB.show();

  // Light only the target tile in blue
  clearAllTiles();
  lightUpTile(targetTileIndex, 0, 0, 255);  // Blue
  stripA.show();
  stripB.show();
}

void lightUpRange(Adafruit_NeoPixel &strip, int start, int end, uint32_t color) {
  for (int i = start; i <= end; i++) {
    if (i >= 0 && i < strip.numPixels()) strip.setPixelColor(i, color);
  }
}

void lightUpTile(int tileIndex, uint8_t r, uint8_t g, uint8_t b) {
  if (tileIndex < 0 || tileIndex >= 15) return;
  uint32_t color = stripA.Color(r, g, b);
  lightUpRange(stripA, tileToLEDRanges[tileIndex][0], tileToLEDRanges[tileIndex][1], color);
  lightUpRange(stripA, tileToLEDRanges[tileIndex][2], tileToLEDRanges[tileIndex][3], color);
  lightUpRange(stripB, tileToLEDRanges[tileIndex][4], tileToLEDRanges[tileIndex][5], color);
  lightUpRange(stripB, tileToLEDRanges[tileIndex][6], tileToLEDRanges[tileIndex][7], color);
}

void clearAllTiles() {
  for (int i = 0; i < stripA.numPixels(); i++) stripA.setPixelColor(i, 0, 0, 0);
  for (int i = 0; i < stripB.numPixels(); i++) stripB.setPixelColor(i, 0, 0, 0);
}

void loop() {
  int maxValue = 0, hitIndex = -1;

  for (int i = 0; i < 14; i++) {
    int sensorValue = analogRead(piezoPins[i]);
    if (sensorValue > maxValue) {
      maxValue = sensorValue;
      hitIndex = i;
    }
  }

  if (maxValue > threshold && hitIndex != -1) {
    if (hitIndex == 12 || hitIndex == 13) hitIndex++;  // Adjust for missing index
    int square = hitIndex + 1;
    Serial.print("Ball has bounced on square ");
    Serial.println(square);

    clearAllTiles();

    if (hitIndex == targetTileIndex) {
      lightUpTile(hitIndex, 0, 255, 0);  // Green if hit target
    } else {
      lightUpTile(targetTileIndex, 255, 0, 0); // Red for missed target
      lightUpTile(hitIndex, 255, 0, 0);       // Red for wrong hit
    }

    stripA.show();
    stripB.show();

    delay(1000);  // Show result briefly

    // Reset
    clearAllTiles();
    lightUpTile(targetTileIndex, 0, 0, 255);  // Blue target again
    stripA.show();
    stripB.show();
  }

  delay(10);
}
