

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#define TFT_DC 2
#define TFT_CS 15
#define INPUT_POT 5
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
int aaa = 10;
void setup() {
  pinMode(INPUT_POT, INPUT_PULLUP);
}

void loop() {
  tft.begin();
tft.setCursor(0, 0);
tft.setTextColor(ILI9341_RED);
tft.setTextSize(3);
tft.println("Hello, TFT!");
tft.setCursor(0,aaa);
tft.setTextColor(ILI9341_GREEN);
tft.setTextSize(2);
tft.println("I can has colors?");
  if ( digitalRead(INPUT_POT) == LOW) {
    aaa = aaa +10;
if ( aaa > 100) {
  aaa = 0;
  }
  }
}
