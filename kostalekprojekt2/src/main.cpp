#include <Arduino_GFX_Library.h>

#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
#define TFT_DC     21
#define TFT_RESET  22

Arduino_ESP32SPI bus = Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCK, TFT_MOSI, TFT_MISO);
Arduino_ILI9341 display = Arduino_ILI9341(&bus, TFT_RESET);

 
void setup(void)
{
  pinMode(4, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

  //můžeš použít ještě 17,16
  //šipka dolů
 
  display.begin();
  display.fillScreen(WHITE);
  display.setCursor(20, 20);
  display.setTextSize(2);
  display.setTextColor(BLUE);
  display.print("Prvopocatek");
}
 
void loop() {   
  if(digitalRead(17) == LOW) {
    display.fillRect(20,40,40,20,WHITE);
    display.setCursor(20, 40);
    display.print("doleva");
    delay(200);
  }
  if(digitalRead(4) == LOW) {
    display.fillRect(20,60,40,20,WHITE);
    display.setCursor(20, 60);
    display.print("dolu");
    delay(200);
  }
    if(digitalRead(15) == LOW) {
    display.fillRect(20,60,40,20,WHITE);
    display.setCursor(20, 60);
    display.print("doprava");
    delay(200);
  }
    if(digitalRead(16) == LOW) {
    display.fillRect(20,60,40,20,WHITE);
    display.setCursor(20, 60);
    display.print("nahoru");
    delay(200);
  }
}
 


