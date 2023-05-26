// Daft_Punk_v3.ino by Nathan Birenbaum 10/24/2021
// Last updated by Nathan Birenbaum 10/28/2021
//  Drives LEDS for a replica of Daft Punk's Guy Manuel helmet.
//  Utilizes 2 strips of 24 WS2812 RGB LEDs to light the fans.
//  Uses rotary encoders for adjusting light pattern.
//  Uses MAX4466 microphone for dancing patterns.
//  https://github.com/FastLED/FastLED/wiki/Basic-usage
//  https://github.com/sparkfun/Rotary_Encoder_Breakout-Illuminated/blob/main/Firmware/RG_Rotary_Encoder/RG_Rotary_Encoder.ino
//  https://forum.arduino.cc/t/sketch-name-as-preprocessor-value/116152
//  https://learn.adafruit.com/adafruit-microphone-amplifier-breakout/measuring-sound-levels

// Libraries
#define FASTLED_INTERNAL
#include "FastLED.h"
#include <PinChangeInt.h>

// WS2812B LED strip definitions
#define NUM_LEDS 24
#define NUM_FANS 8
#define DATA_PIN 2
#define NUM_STATES 4
#define DEFAULT_BRIGHTNESS 12

// Rotary encoder definitions
#define ROT_LEDB 7  // blue LED
#define ROT_LEDG 8  // green LED
#define ROT_LEDR 9  // red LED
#define ROT_B    A3 // rotary B
#define ROT_A    3  // rotary A (do not change - used for interrupts)
#define ROT_SW   4  // rotary pushbutton

// Global variables that can be changed in interrupt routines
volatile int rotary_counter = 1; // current "position" of rotary encoder (increments CW)
volatile boolean rotary_change = false; // will turn true if rotary_counter has changed
volatile boolean button_pressed = false; // will turn true if the button has been pushed
volatile boolean button_released = false; // will turn true if the button has been released (sets button_downtime)
volatile unsigned long button_downtime = 0L; // ms the button was pushed before release
volatile int selected_program = 0; // State defined by rotary encoder

// "Static" variables are initalized once the first time that loop runs, but they keep their values through successive loops.
static unsigned char x = 0; //LED color
static boolean button_down = false;
static unsigned long int button_down_start, button_down_time;

// Fan colors
#define BLACK  0x000000
#define WHITE  0xffffff
#define RED    0xff0000
#define ORANGE 0xff2200
#define YELLOW 0xff6600
#define LGREEN 0x99ff00
#define GREEN  0x009900
#define LBLUE  0x0099ff
#define BLUE   0x000099
#define VIOLET 0x990099

// Rotary encoder colors (common anode LED: on=0, off=1)
#define ROT_BLACK B111
#define ROT_RED B110
#define ROT_GREEN B101
#define ROT_YELLOW B100
#define ROT_BLUE B011
#define ROT_PURPLE B010
#define ROT_CYAN B001
#define ROT_WHITE B000

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
int selected_fan = 1;
int fan_direction = 1;
int fade_amount = 1;
int brightness_difference_glow = 10;
int brightness_difference_wave = 128;
int adjusted_brightness = DEFAULT_BRIGHTNESS;
int faded_brightness = DEFAULT_BRIGHTNESS;
unsigned int sample; // for max4466

void setup() {
  Serial.begin(9600);
  printCurrentSketch();
  digitalWrite(13, LOW); // Turns off on-board LED
  setupRGBEncoder();
  setEncoderLED(ROT_WHITE);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS); // Max 255
  confirmBlink();
}


// Sets fans to rainbow color scheme
void setFanColors(long unsigned int color1, long unsigned int color2, long unsigned int color3, long unsigned int color4, long unsigned int color5, long unsigned int color6, long unsigned int color7, long unsigned int color8) {
  leds1 = CRGB(color1);
  leds2 = CRGB(color2);
  leds3 = CRGB(color3);
  leds4 = CRGB(color4);
  leds5 = CRGB(color5);
  leds6 = CRGB(color6);
  leds7 = CRGB(color7);
  leds8 = CRGB(color8);
}

void loop() {
  selected_program = abs(rotary_counter) % NUM_STATES;
  switch (selected_program) {
    case 0:
      solidRainbow();
      Serial.println("running solidRainbow()");
      break;
    case 1:
      glowRainbow();
      Serial.println("running glowRainbow()");
      break;
    case 2:
      waveRainbow();
      Serial.println("running waveRainbow()");
      break;
    case 3:
      danceGreenYellowRed();
      Serial.println("running danceGreenYellowRed()");
      break;
    default:
      confirmBlink();
      Serial.println("default reached in switch (selected_program) statement in loop()");
      break;
  }
  delay(10);

  if (rotary_change) {
    Serial.print("rotary: ");
    Serial.println(rotary_counter, DEC);
    rotary_change = false;
    setEncoderLED(ROT_BLACK);
    delay(50);
    setEncoderLED(ROT_WHITE);
    FastLED.clear();
  }

  if (button_pressed) {
    Serial.println("button press");
    button_pressed = false;
    button_down = true;
    button_down_start = millis();
  }

  if (button_released) {
    setEncoderLED(ROT_WHITE);
    Serial.print("button release, downtime: ");
    Serial.println(button_downtime, DEC);
    button_released = false;
    button_down = false;
  }

  if (button_down) {
    setEncoderLED(ROT_RED);
    button_down_time = millis() - button_down_start;
    Serial.println(button_down_time);
  }
}

// Fills all sets of LEDs with a solid color
void solidRainbow() {
  setFanColors(RED,ORANGE,YELLOW,LGREEN,GREEN,LBLUE,BLUE,VIOLET);
  FastLED.setBrightness(adjusted_brightness);
  FastLED.show();
}

// Fills all sets of LEDs with a solid color and pulses their brightness in unison
void glowRainbow() {
  EVERY_N_MILLISECONDS(50) {
    setFanColors(RED,ORANGE,YELLOW,LGREEN,GREEN,LBLUE,BLUE,VIOLET);
    if (faded_brightness <= adjusted_brightness - brightness_difference_glow) {
      fade_amount = 1;
    }
    if (faded_brightness >= adjusted_brightness + brightness_difference_glow) {
      fade_amount = -1;
    }
    faded_brightness = faded_brightness + fade_amount;
    FastLED.setBrightness(faded_brightness);
    FastLED.show();
  }
}

// Fills all sets of LEDs with a solid colorand pulses them in waves
void waveRainbow() {
  setFanColors(RED,ORANGE,YELLOW,LGREEN,GREEN,LBLUE,BLUE,VIOLET);
  EVERY_N_MILLISECONDS( 100 ) {
    FastLED.setBrightness(adjusted_brightness);
    switch (selected_fan) {
      case 0:
        leds1.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 1:
        leds1.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        leds2.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 2:
        leds1.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        leds2.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        leds3.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 3:
        leds2.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        leds3.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        leds4.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 4:
        leds3.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        leds4.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        leds5.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 5:
        leds4.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        leds5.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        leds6.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 6:
        leds5.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        leds6.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        leds7.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 7:
        leds6.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        leds7.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        leds8.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      case 8:
        leds7.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        leds8.fadeLightBy(adjusted_brightness+brightness_difference_wave);
        break;
      case 9:
        leds8.fadeLightBy(adjusted_brightness+brightness_difference_wave/2);
        break;
      default:
        Serial.println("default reached in switch (selected_fan) statement in rainbowGlow()");
        break;
    }
    FastLED.show();
    if (selected_fan <= -1) {
      fan_direction = 1;
    }
    if (selected_fan >= 10) {
      fan_direction = -1;
    }
    selected_fan = selected_fan+fan_direction;
    Serial.println(selected_fan);
    delay(10);
  }
}

// Dances fans to music while set to green/yellow/red color scheme.
void danceGreenYellowRed() {
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
   while (millis() - startMillis < 50) { // collect data for 50 mS
    sample = analogRead(A0);
    if (sample < 1024){  // toss out spurious readings
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin){
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
  Serial.println(volts);
  int fans = volts * 8/5;
  switch (fans) {
    case 1:
      setFanColors(GREEN,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK);
      break;
    case 2:
      setFanColors(GREEN,GREEN,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK);
      break;
    case 3:
      setFanColors(GREEN,GREEN,GREEN,BLACK,BLACK,BLACK,BLACK,BLACK);
      break;
    case 4:
      setFanColors(GREEN,GREEN,GREEN,GREEN,BLACK,BLACK,BLACK,BLACK);
      break;
    case 5:
      setFanColors(GREEN,GREEN,GREEN,GREEN,YELLOW,BLACK,BLACK,BLACK);
      break;
    case 6:
      setFanColors(GREEN,GREEN,GREEN,GREEN,YELLOW,YELLOW,BLACK,BLACK);
      break;
    case 7:
      setFanColors(GREEN,GREEN,GREEN,GREEN,YELLOW,YELLOW,YELLOW,BLACK);
      break;
    case 8:
      setFanColors(GREEN,GREEN,GREEN,GREEN,YELLOW,YELLOW,YELLOW,RED);
      break;
  }
  FastLED.setBrightness(adjusted_brightness);
  FastLED.show();
}

// Sets color of encoder LED
void setEncoderLED(unsigned char rotary_color) {
  digitalWrite(ROT_LEDR, rotary_color & B001);
  digitalWrite(ROT_LEDG, rotary_color & B010);
  digitalWrite(ROT_LEDB, rotary_color & B100);
}


// Blinks for setup confirmation
void confirmBlink() {
  for (int8_t ii = 0; ii < 5; ii++) {
    leds[0] = CRGB(WHITE);
    FastLED.show();
    delay(100);
    leds[0] = CRGB(BLACK);
    FastLED.show();
    delay(100);
  }
}

// All setup code for the RGB rotary encoder
void setupRGBEncoder() {
  pinMode(ROT_A, INPUT);
  pinMode(ROT_B, INPUT);
  digitalWrite(ROT_A, HIGH); // Soft pullup
  digitalWrite(ROT_B, HIGH); // Soft pullup
  pinMode(ROT_SW, INPUT);    // Common anode w/ pulldown
  pinMode(ROT_LEDB, OUTPUT);
  pinMode(ROT_LEDG, OUTPUT);
  pinMode(ROT_LEDR, OUTPUT);
  digitalWrite(ROT_LEDR, 100);
  digitalWrite(ROT_LEDG, 100);
  digitalWrite(ROT_LEDB, 100);
  attachInterrupt(1, rotaryIRQ, CHANGE); // Uses external interrupt pin
  PCintPort::attachInterrupt(ROT_SW, &buttonIRQ, CHANGE); // Uses pin change interrupt library
}

// Prints details of the current running sketch for debugging
void  printCurrentSketch() {
  String the_path = __FILE__;
  int slash_loc = the_path.lastIndexOf('/');
  String the_cpp_name = the_path.substring(slash_loc+1);
  int dot_loc = the_cpp_name.lastIndexOf('.');
  String the_sketchname = the_cpp_name.substring(0, dot_loc);

  Serial.print("\nArduino is running: ");
  Serial.println(the_sketchname);
  Serial.print("Compiled on: ");
  Serial.print(__DATE__);
  Serial.print(" at ");
  Serial.println(__TIME__);
}

// Process input from the rotary encoder. Rotary position is held in rotary_counter.
void rotaryIRQ() {
  static unsigned char rotary_state = 0; // current/previous encoder states
  rotary_state <<= 2;  // remember previous state
  rotary_state |= (digitalRead(ROT_A) | (digitalRead(ROT_B) << 1));  // mask in current state
  rotary_state &= 0x0F; // zero upper nybble
  if (rotary_state == 0x09) { // from 10 to 01, increment counter. Also try 0x06 if unreliable
    rotary_counter++;
    rotary_change = true;
  }
  else if (rotary_state == 0x03) { // from 00 to 11, decrement counter. Also try 0x0C if unreliable
    rotary_counter--;
    rotary_change = true;
  }
}

// Process rotary encoder button presses/releases and debounces noisy contact "presses".
// Sets button_pressed and button_released flags and computes button_downtime.
void buttonIRQ() {
  static boolean button_state = false;
  static unsigned long btn_start, btn_end;
  if ((PCintPort::pinState == HIGH) && (button_state == false)) { // Button was up, but now down
    // Discard button presses too close together (debounce)
    btn_start = millis();
    if (btn_start > (btn_end + 10)) {
      button_state = true;
      button_pressed = true;
    }
  }
  else if ((PCintPort::pinState == LOW) && (button_state == true)) { // Button was down, but now up
    // Discard button releases too close together (debounce)
    btn_end = millis();
    if (btn_end > (btn_start + 10))
    {
      button_state = false;
      button_released = true;
      button_downtime = btn_end - btn_start;
    }
  }
}
