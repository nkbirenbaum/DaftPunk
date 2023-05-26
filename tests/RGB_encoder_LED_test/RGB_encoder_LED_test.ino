// Libraries
#include <PinChangeInt.h>

// Rotary encoder definitions
#define ROT_LEDB 7  // blue LED
#define ROT_LEDG 8  // green LED
#define ROT_LEDR 9  // red LED

// Fan colors
#define BLACK  0xffffff
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

void setup() {
  digitalWrite(13, LOW); // Turns off on-board LED
  setupRGBEncoder();
  setEncoderLED(WHITE);
}

void loop() { 
  setEncoderLED(WHITE);
  delay(1000);
}

// Sets color of encoder LED
void setEncoderLED(unsigned char rotary_color) {
  digitalWrite(ROT_LEDR, rotary_color & B001);
  digitalWrite(ROT_LEDG, rotary_color & B010);
  digitalWrite(ROT_LEDB, rotary_color & B100);
}

// All setup code for the RGB rotary encoder
void setupRGBEncoder() {
  pinMode(ROT_LEDB, OUTPUT);
  pinMode(ROT_LEDG, OUTPUT);
  pinMode(ROT_LEDR, OUTPUT);
  digitalWrite(ROT_LEDR, HIGH);
  digitalWrite(ROT_LEDG, HIGH);
  digitalWrite(ROT_LEDB, HIGH);
  delay(500);
}
