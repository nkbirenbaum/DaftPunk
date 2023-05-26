// Daft_Punk_v2.ino by Nathan Birenbaum 10/24/2021
// Last updated by Nathan Birenbaum 10/25/2021
//  Drives LEDS for a replica of Daft Punk's Guy Manuel helmet.
//  Utilizes 2 strips of 24 WS2812 RGB LEDs to light the fans.

// Libraries
#define FASTLED_INTERNAL
#include "FastLED.h"

// Definitions
#define NUM_LEDS 24
#define DATA_PIN 2
#define MY_STATE 1 // Placeholder for state defined by rotary encoder
#define WHITE 0xffffff
#define RED 0xff0000
#define ORANGE 0xff2200
#define YELLOW 0xff6600
#define LGREEN 0x99ff00
#define GREEN 0x009900
#define LBLUE 0x0099ff
#define BLUE 0x000099
#define VIOLET 0x990099

// Initialize LED arrays
CRGB leds[NUM_LEDS];
CRGBSet setleds(leds, NUM_LEDS);
CRGBSet leds1(setleds(0,2));
CRGBSet leds2(setleds(3,5));
CRGBSet leds3(setleds(6,8));
CRGBSet leds4(setleds(9,11));
CRGBSet leds5(setleds(12,14));
CRGBSet leds6(setleds(15,17));
CRGBSet leds7(setleds(18,20));
CRGBSet leds8(setleds(21,23));

void setup() {
  digitalWrite(13, LOW); // Turns off on-board LED
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(8); // Max 255
  confirmBlink();
}

void loop() { 
  switch (MY_STATE) {
    case 1:
      rainbowSolid();
      break;
    case 2:
      rainbowGlow();
      break;
    case 3:
      danceRainbow();
      break;
    case 4:
      danceGreenYellowRed();
      break;
    default:
      confirmBlink();
      break;
  }
  FastLED.show();
  delay(100);
}

void rainbowSolid() {
  // Fills all sets of LEDs with a solid color
  leds1 = CRGB(RED);
  leds2 = CRGB(ORANGE);
  leds3 = CRGB(YELLOW);
  leds4 = CRGB(LGREEN);
  leds5 = CRGB(GREEN);
  leds6 = CRGB(LBLUE);
  leds7 = CRGB(BLUE);
  leds8 = CRGB(VIOLET);
}

void rainbowGlow() {
  
}

void danceRainbow() {
      
}

void danceGreenYellowRed() {
      
}

void confirmBlink() {
  // Blinks for setup confirmation
  for (int8_t ii = 0; ii < 5; ii++) {
    leds[0] = CRGB::White;
    FastLED.show();
    delay(100);
    leds[0] = CRGB::Black;
    FastLED.show();
    delay(100);
  }
}
