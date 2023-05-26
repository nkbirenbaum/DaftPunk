
// Daft_Punk_ESP32_v1.ino by Nathan Birenbaum 10/29/2022
// Last updated by Nathan Birenbaum 10/29/2022
//  Drives LEDS for a replica of Daft Punk's Guy Manuel helmet.

// Libraries 
#define FASTLED_INTERNAL
#include <FastLED.h>

// WS2812B LED strip definitions
#define EAR_CUP_PIN 3
#define EAR_CUP_PIN2 4
#define TEST_PIN 2
#define NUM_LEDS_EAR_CUP 6
#define NUM_TEST_LEDS 2
#define DEFAULT_BRIGHTNESS 40

// Define the array of leds
CRGB earCupLeds[NUM_LEDS_EAR_CUP];
CRGB earCupLeds2[NUM_LEDS_EAR_CUP];
//CRGB testLeds[NUM_TEST_LEDS];

// Runs once at program startup
void setup() {
  
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  FastLED.addLeds<WS2812B, EAR_CUP_PIN, GRB>(earCupLeds, NUM_LEDS_EAR_CUP);
  FastLED.addLeds<WS2812B, EAR_CUP_PIN2, GRB>(earCupLeds2, NUM_LEDS_EAR_CUP);
  //FastLED.addLeds<WS2812B, TEST_PIN, GRB>(testLeds, NUM_TEST_LEDS);

  // Solid color for ear cup
  fill_solid(earCupLeds, NUM_LEDS_EAR_CUP, CRGB::Green);
  fill_solid(earCupLeds2, NUM_LEDS_EAR_CUP, CRGB::Green);
  //fill_solid(testLeds, NUM_TEST_LEDS, CRGB::Red);
  //earCupLeds = CRGB(0x009900);
  FastLED.show();
  
}

// Main program loop
void loop() {

  // Turn the LED on, then pause
  //testLeds[0] = CRGB::Red;
  //testLeds[1] = CRGB::Blue;
  //fill_solid(earCupLeds, NUM_LEDS_EAR_CUP, CRGB::Red);
  //FastLED.show();
  //delay(200);
  // Now turn the LED off, then pause
  //testLeds[0] = CRGB::Black;
  //testLeds[1] = CRGB::Black;
  //fill_solid(earCupLeds, NUM_LEDS_EAR_CUP, CRGB::Black);
  //FastLED.show();
  delay(200);

}
