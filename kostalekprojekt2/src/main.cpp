#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

//definice pinů pro display
#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
#define TFT_DC     21
#define TFT_RST  22
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);
//definice pravidel hry
#define FRAMESPEED 200
#define VIRTUALVYSKA 32
#define VIRTUALSIRKA 24
#define VIRTUALNASOB 10
#define MAXHAD 50

//piny ovládacích tlačítek
#define CONTROLUP 16
#define CONTROLDOWN 4
#define CONTROLLEFT 17
#define CONTROLRIGHT 15

int hadY[MAXHAD], hadX[MAXHAD];
int direction = 0, score = 0;
bool pohybProveden = true;
unsigned long lastFrame = 0, lastFood = 0;


//Funkce, která posunuje hlavu hada
void addHad(int x, int y) {
  //Projede pole, posune data v poli o jedno navíc
  for(int z = MAXHAD; z>-1; z--) {
    if(z<MAXHAD-1) {
      hadY[z+1] = hadY[z];
      hadX[z+1] = hadX[z];
    } 
  }
  //Vloží se přijaté data na začátek pole
  hadX[0] = x;
  hadY[0] = y;
  pohybProveden = false;
}
//Nastaví základní hodnoty 
void defaultSetting() {
  for(int x = 0; x<MAXHAD; x++) {
    hadX[x] = -2;
    hadY[x] = -2;
  }

  hadY[0] = 11;
  hadY[1] = 10;
  hadX[0] = 10;
  hadX[1] = 10;
  pohybProveden = true;
  score = 0;
  delay(1000);
}
//Napíše text na obrazovku a zastaví kód, dokud není zmáčknuto tlačítko nahoru
void smrt() {
  tft.setCursor(20,60);
  tft.println("You died!");
  tft.print("Zmackni nahoru pro pokracovani!");
  while(digitalRead(CONTROLUP) == HIGH) {}
  defaultSetting();
}
//nastavení pinů a displaye
void setup() {
  Serial.begin(115200);
  pinMode(CONTROLUP, INPUT_PULLUP);
  pinMode(CONTROLLEFT, INPUT_PULLUP);
  pinMode(CONTROLDOWN, INPUT_PULLUP);
  pinMode(CONTROLRIGHT, INPUT_PULLUP);

  tft.begin();
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  defaultSetting();
}

void loop() {
  if(pohybProveden) { //posunutí hada podle tlačítek
    if (digitalRead(CONTROLUP) == LOW && direction != CONTROLDOWN) {
      direction = CONTROLUP;
      addHad(hadX[0],hadY[0]-1);
    } else if (digitalRead(CONTROLLEFT) == LOW && direction != CONTROLRIGHT) {
      direction = CONTROLLEFT;
      addHad(hadX[0]-1,hadY[0]);
    } else if (digitalRead(CONTROLDOWN) == LOW && direction != CONTROLUP) {
      direction = CONTROLDOWN;
      addHad(hadX[0],hadY[0]+1);
    } else if (digitalRead(CONTROLRIGHT) == LOW && direction != CONTROLLEFT) {
      direction = CONTROLRIGHT;
      addHad(hadX[0]+1,hadY[0]);
    }
  }

  if(millis() > lastFrame + FRAMESPEED) { 
    //vyrenderování snímku
    tft.fillScreen(ILI9341_BLACK);
    if(pohybProveden) { //Posunutí hada pokud nebyla určena změna směru
      if(hadX[0] > hadX[1]) {
        addHad(hadX[0]+1,hadY[0]);
      } else if(hadX[0] < hadX[1]) {
        addHad(hadX[0]-1,hadY[0]);
      } else if(hadY[0] > hadY[1]) {
        addHad(hadX[0],hadY[0]+1);
      } else if(hadY[0] < hadY[1]) {
        addHad(hadX[0],hadY[0]-1);
      }
    
    }
    for(int x = 0; x<MAXHAD; x++) {
      //Smazání posledního pixelu hada
      if(hadX[x] == -2) {
        hadX[x-1] = -2;
        hadY[x-1] = -2;
      }
    }

    for(int x = 0; x<MAXHAD; x++) { //nakreslení hada

      if(hadX[x] != -2) {
        if(x == 0) {
          //Vykreslení hlavy hada jinou barvou
          tft.fillRect(hadX[x]*VIRTUALNASOB,hadY[x]*VIRTUALNASOB,VIRTUALNASOB,VIRTUALNASOB,0xFC00);
        } else {
          tft.fillRect(hadX[x]*VIRTUALNASOB,hadY[x]*VIRTUALNASOB,VIRTUALNASOB,VIRTUALNASOB,0xF800);
        }
      } else {
        break;
      }
    }
    tft.drawRect(0,0,240,320,0xFFFF);
    tft.setCursor(20,20);
    tft.print(score);
    //Kontrola nabourání do stěny
    if(hadX[0] > VIRTUALSIRKA-1 || hadX[0] < 0 || hadY[0] > VIRTUALVYSKA-1 || hadY[0] < 0) {
      smrt();
    }
    for(int x = 0; x<MAXHAD; x++) {
       //Kontrola nabourání do sebe
      if(hadX[x] != -2 && x != 0) {
        if(hadX[0] == hadX[x] && hadY[0] == hadY[x])
          smrt();
      }
    }
    pohybProveden = true;
    lastFrame = millis();
  }
}