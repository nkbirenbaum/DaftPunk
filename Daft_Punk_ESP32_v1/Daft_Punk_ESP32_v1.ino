// Daft_Punk_ESP32_v1.ino by Nathan Birenbaum 10/29/2022
// Last updated by Nathan Birenbaum 10/29/2022
//  Drives LEDS for a replica of Daft Punk's Guy Manuel helmet.

// Libraries 
#define FASTLED_INTERNAL
#include <FastLED.h>

// WS2812B LED strip definitions
#define NUM_LEDS_EAR_CUP 6
#define EAR_CUP_PIN A0
#define NUM_TEST_LEDS 2
#define TEST_PIN A1
#define DEFAULT_BRIGHTNESS 40

// Define the array of leds
CRGB earCupLeds[NUM_LEDS_EAR_CUP];
CRGB testLeds[NUM_TEST_LEDS];

// Runs once at program startup
void setup() {
  
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  FastLED.addLeds<WS2812B, EAR_CUP_PIN, GRB>(earCupLeds, NUM_LEDS_EAR_CUP);
  FastLED.addLeds<WS2812B, TEST_PIN, GRB>(testLeds, NUM_TEST_LEDS);

  // Solid color for ear cup
  fill_solid(earCupLeds, NUM_LEDS_EAR_CUP, CRGB::Green);
  //earCupLeds = CRGB(0x009900);
  //CRGB::Green;
  FastLED.show();
  
}

// Main program loop
void loop() {
  // Turn the LED on, then pause
  testLeds[0] = CRGB::Red;
  testLeds[1] = CRGB::Blue;
  FastLED.show();
  delay(200);
  // Now turn the LED off, then pause
  testLeds[0] = CRGB::Black;
  testLeds[1] = CRGB::Black;
  FastLED.show();
  delay(200);
}
