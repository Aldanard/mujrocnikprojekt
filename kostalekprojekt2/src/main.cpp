#include <Arduino_GFX_Library.h>

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
#define TFT_DC     21
#define TFT_RESET  22

Arduino_ESP32SPI bus = Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_ILI9341 display = Arduino_ILI9341(&bus, TFT_RESET);

int cislo = 0;
 
void setup(void)
{
  pinMode(4, INPUT_PULLUP);
  //můžeš použít ještě 17,16,2
 
  display.begin();
  display.fillScreen(WHITE);
  display.setCursor(20, 20);
  display.setTextSize(2);
  display.setTextColor(BLUE);
  display.print("Prvopocatek");
}
 
void loop() {
  if(digitalRead(4) == LOW) {
    cislo++;
    display.fillRect(20,40,60,40,WHITE);
    display.setCursor(20, 40);
    display.print(cislo);
    delay(200);
  }
}