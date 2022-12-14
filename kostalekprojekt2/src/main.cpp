#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

//definice pinů pro display
#define TFT_SCK    18
#define TFT_MOSI   23
#define TFT_MISO   19
#define TFT_CS     5
#define TFT_DC     21
#define TFT_RST    22
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);
//definice pravidel hry
#define FRAMESPEED 300
#define FOODSPEED 8000
#define VIRTUALVYSKA 32
#define VIRTUALSIRKA 24
#define VIRTUALNASOB 10
#define MAXHAD 500
#define MAXFOOD 5

//piny ovládacích tlačítek
#define CONTROLUP 16
#define CONTROLDOWN 4
#define CONTROLLEFT 17
#define CONTROLRIGHT 15

int hadY[MAXHAD], hadX[MAXHAD];
int foodY[MAXFOOD], foodX[MAXFOOD];
int direction = 0, score = 0;
bool pohybProveden = true, deleteLast = true;
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
  deleteLast = true;
}
//Nastaví základní hodnoty 
void defaultSetting() {
  for(int x = 0; x<MAXHAD; x++) {
    hadX[x] = -2;
    hadY[x] = -2;
  }
  for(int x = 0; x<MAXFOOD; x++) {
    foodX[x] = -1;
    foodY[x] = -1;
  }
  hadY[0] = 11;
  hadY[1] = 10;
  hadX[0] = 10;
  hadX[1] = 10;
  pohybProveden = true;
  deleteLast = true;
  score = 0;
  tft.fillScreen(ILI9341_BLACK);
  delay(1000);
}
//Napíše text na obrazovku a zastaví kód, dokud není zmáčknuto tlačítko nahoru
void smrt() {
  tft.setCursor(20,60);
  tft.println("You died!");
  tft.print("Push Up button to   continue.");

  while(digitalRead(CONTROLUP) == HIGH) {}
  defaultSetting();
}
//Kontrola vstupních údajů, pokud se jídlo nezobrazí na hadovi
bool foodCheck(int tfoodX, int tfoodY) {
  for(int x = 0; x<MAXHAD; x++) {
    if(hadX[x] != -2) {
      if(hadX[x] == tfoodX && hadY[x] == tfoodY) {
        return true;
      }
    } else {
      return false;
    }
  }
}
//Vybere náhodnou pozici v poli a přidá na ní jídlo
void addFood() {
  for(int x = 0; x<MAXFOOD; x++) {
    if(foodX[x] == -1) {
      int tfoodX, tfoodY;
      do{
        tfoodX = random(1,VIRTUALSIRKA);
        tfoodY = random(1,VIRTUALVYSKA);
      } while (foodCheck(tfoodX,tfoodY));
      foodX[x] = tfoodX;
      foodY[x] = tfoodY;
      break;
    }
  }
}
//Definice interuptů pro tlačítka
void IRAM_ATTR iup(){
  if (pohybProveden && direction != CONTROLDOWN) {
      direction = CONTROLUP;
      addHad(hadX[0],hadY[0]-1);
    }
}
void IRAM_ATTR idown(){
  if (pohybProveden && direction != CONTROLUP) {
      direction = CONTROLDOWN;
      addHad(hadX[0],hadY[0]+1);
  }
}
void IRAM_ATTR ileft(){
  if (pohybProveden && direction != CONTROLRIGHT) {
      direction = CONTROLLEFT;
      addHad(hadX[0]-1,hadY[0]);
    }
}
void IRAM_ATTR iright(){
  if (pohybProveden && direction != CONTROLLEFT) {
      direction = CONTROLRIGHT;
      addHad(hadX[0]+1,hadY[0]);
    }
}
//nastavení pinů a displaye
void setup() {
  Serial.begin(115200);
  pinMode(CONTROLUP, INPUT_PULLUP);
  pinMode(CONTROLLEFT, INPUT_PULLUP);
  pinMode(CONTROLDOWN, INPUT_PULLUP);
  pinMode(CONTROLRIGHT, INPUT_PULLUP);
  attachInterrupt(CONTROLUP,iup,FALLING);
  attachInterrupt(CONTROLDOWN,idown,FALLING);
  attachInterrupt(CONTROLLEFT,ileft,FALLING);
  attachInterrupt(CONTROLRIGHT,iright,FALLING);
  tft.begin();
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  defaultSetting();
  addFood();
}

void loop() {
  if(millis() > lastFrame + FRAMESPEED) { 
    //vyrenderování snímku
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
    for(int x = 0; x<MAXFOOD; x++) { //konzumace jidla
      if(foodX[x] == hadX[0] && foodY[x] == hadY[0]) {
        deleteLast = false;
        foodX[x] = -1;
        foodY[x] = -1;
        score++;
      }
    }
    if(deleteLast) { 
      //Smazání posledního pixelu hada, pokud nebylo snězeno jídlo
      for(int x = 0; x<MAXHAD; x++) {
        if(hadX[x] == -2) {
          //Mazání posledního pixelu na displayu pro urychlení pohybu (lepší než mazat display celý)
          tft.fillRect(hadX[x-1]*VIRTUALNASOB,hadY[x-1]*VIRTUALNASOB,VIRTUALNASOB,VIRTUALNASOB,0x0000);
          hadX[x-1] = -2;
          hadY[x-1] = -2;
        }
      }
    }
    for(int x = 0; x<MAXFOOD; x++) { //nakreslení jidla
      if(foodX[x] != -1)
        tft.fillRect(foodX[x]*VIRTUALNASOB,foodY[x]*VIRTUALNASOB,VIRTUALNASOB,VIRTUALNASOB,0x07E0);
    }
    for(int x = 0; x<MAXHAD; x++) { //nakreslení hada
      if(hadX[x] != -2 && x<2) {
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
    //přemazání starého skóre jiným
     tft.setTextColor(ILI9341_BLACK);
    tft.print(score-1);
    tft.setCursor(20,20);
     tft.setTextColor(ILI9341_WHITE);
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
  if(millis() > lastFood + random(FOODSPEED,FOODSPEED*2)) { //Přidání jídlo po náhodném časovém intervalu
    addFood();
    lastFood = millis();
  }
}
