#include "rm67162.h"
#include "animation.h"
#include <TFT_eSPI.h>
#include "NotoSansBold15.h"
#include "NotoSansBold36.h"
#include "NotoSansMonoSCB20.h"

#include "Latin_Hiragana_24.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

float batteryVoltage; 


//colors
#define gray 0x2A0A
#define lines 0x8C71
unsigned short rings[4] = { 0x47DD, 0xFB9F, 0x86BF, 0xFFD0 };

int x, y;
int n = 0;

//graph variables
int fromTop = 30;
int fromLeft = 180;
int w = 350;
int h = 100;
int values[18] = { 0 };
int values2[18] = { 0 };

long startedT = 0;
int finded = 0;
int timeF = 0;

int aniFrame = 0;

//_____________________________RINGS ________________________
String lbl[4] = { "CPU", "RAM", "GPU", "TMP" };
byte ringsD[4] = { 30, 65, 17, 88 };
byte changeD[4] = { 1, -1, -1, 1 };
byte goal[4] = { 89, 20, 3, 99 };

//.........................buttons
String battery;
#define up 21
#define down 0
#define led 38
bool deb = 0;
bool deb2 = 0;
byte bright = 5;
byte brightness[7] = {100, 120, 140, 180, 200, 230, 254 };
bool ledON = false;

//......................fps
bool evrySecond = 0;
double fps=0;
double lastMillis=0;
String counter;

long prevMillis;
int interval=400;

void setup() {
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  sprite.createSprite(536, 241);
  sprite.setSwapBytes(1);
  rm67162_init();  // amoled lcd initialization
  lcd_setRotation(1);
  for (int i = 0; i < 18; i++)
    values[i] = random(0, 100);
}

void drawSprite() {

  sprite.fillSprite(TFT_BLACK);
  sprite.fillRect(2,194,12,40,0x6924);
  sprite.setTextDatum(0);
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.drawString(counter, 10, 0, 7);
  sprite.setTextColor(rings[3], TFT_BLACK);
  sprite.drawString("AMOLED", 298, -2,2);

  sprite.setTextColor(rings[3], TFT_BLACK);
  sprite.loadFont(NotoSansMonoSCB20);
  sprite.drawString("FPS: "+String(fps), 10, 56);
  sprite.setTextColor(rings[2], TFT_BLACK);
  
  sprite.drawString(battery, 436, 0);
  sprite.drawString("Br", fromLeft-26, 222);
  sprite.unloadFont();

  sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
  sprite.loadFont(Latin_Hiragana_24);
  sprite.drawString("SCANING...", fromLeft, 0);
  sprite.unloadFont();

  //------------------------GRAPH-------------------------------
  sprite.setTextColor(TFT_WHITE, TFT_BLACK);
  sprite.loadFont(NotoSansBold15);
  sprite.setTextDatum(4);

  for (int i = 0; i < 340; i = i + 20) {
    sprite.drawLine(fromLeft + i, fromTop, fromLeft + i, fromTop + h, lines);
    if (i % 40 == 0)
      sprite.drawString(String(i / 10), fromLeft + i, fromTop + h + 14);
  }

  for (int i = 0; i < 100; i = i + 20) {
    sprite.drawLine(fromLeft, fromTop + h - i, fromLeft + w, fromTop + h - i, lines);
    if (i % 40 == 0 && i > 0)
      sprite.drawString(String(i / 10), fromLeft - 12, fromTop + h - i);
  }
  sprite.drawLine(fromLeft, fromTop, fromLeft, fromTop + h, TFT_WHITE);
  sprite.drawLine(fromLeft, fromTop + h, fromLeft + w, fromTop + h, TFT_WHITE);

  for (int i = 0; i < 17; i++) {
    sprite.drawLine(fromLeft + (i * 20), fromTop + values[i], fromLeft + ((i + 1) * 20), fromTop + values[i + 1], TFT_RED);
    sprite.drawLine(fromLeft + (i * 20), fromTop + values[i] - 1, fromLeft + ((i + 1) * 20), fromTop + values[i + 1] - 1, TFT_RED);
  }
  sprite.unloadFont();
  //.......................END OF GRAPH DRAW-------------------------


  //-----------------------RINGS----------------------------------------
  sprite.setTextColor(TFT_WHITE, gray);
  for (int i = 0; i < 4; i++) {
    sprite.fillSmoothRoundRect(fromLeft + (i * 90), fromTop + h + 30, 80, 80, 6, gray, TFT_BLACK);
    sprite.drawSmoothArc(fromLeft + (i * 90) + 40, fromTop + h + 70, 33, 32, 45, 315, 0x21C9, gray);
    sprite.drawSmoothArc(fromLeft + (i * 90) + 40, fromTop + h + 70, 33, 32, 45, map(ringsD[i], 0, 100, 46, 315), rings[i], gray);
    sprite.loadFont(NotoSansBold15);
    sprite.drawString(lbl[i], fromLeft + (i * 90) + 40, fromTop + h + 100);
    sprite.unloadFont();
    sprite.loadFont(NotoSansMonoSCB20);
    sprite.drawString(String(ringsD[i]), fromLeft + (i * 90) + 40, fromTop + h + 70);
    sprite.unloadFont();
  }



  sprite.pushImage(20, 90, 120, 154, animation[aniFrame]);

  for(int i=0;i<bright+1;i++)
  sprite.fillRect(fromLeft-24,214-(i*7),15,4,rings[2]);

 if(ledON)
  sprite.fillRoundRect(fromLeft-24,0,18,30,4,0x32E9);

  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
}

void readButtons(){
  if (digitalRead(up) == 0) {
    if (deb == 0) {
      deb = 1;
      bright++;
      if (bright == 7) bright = 0;
      lcd_brightness(brightness[bright]);
    }
  } else deb = 0;

  if (digitalRead(down) == 0) {
    if (deb2 == 0) {
      deb2 = 1;
      ledON = !ledON;
      digitalWrite(led, ledON);
    }
  } else deb2 = 0;
}


void loop() {

  readButtons();

  if (millis() - prevMillis > interval){
  prevMillis = millis();
  batteryVoltage  = ((analogRead(4) * 2 * 3.3 * 1000) / 4096)/1000;
  battery="BAT:"+String(batteryVoltage);
  }


  if(n<10000) counter=String(n);
  if(n<1000) counter="0"+String(n);
  if(n<100) counter="00"+String(n);
  if(n<10) counter="000"+String(n);
  

  fps=1000/(millis()-lastMillis);
  lastMillis=millis();

  



  //.................animation change frames ,rotating head
  evrySecond = !evrySecond;
  aniFrame++;
  if (aniFrame == 70){
    aniFrame = 0; n++;}

  if (values[17] < 90 && values[17] > 10)
    values[17] = random(values[17] - 10, values[17] + 10);
  else
    values[17] = random(0, 100);

  for (int i = 0; i < 18; i++)
    values2[i] = values[i];

  for (int i = 17; i > 0; i--)
    values[i - 1] = values2[i];



  if (evrySecond == 1)
    for (int i = 0; i < 4; i++) {
      ringsD[i] = ringsD[i] + changeD[i];
      if (ringsD[i] == goal[i]) {
        changeD[i] = changeD[i] * -1;
        if (changeD[i] == 1) goal[i] = random(ringsD[i] + 2, 99);
        else goal[i] = random(0, ringsD[i] - 2);
      }
    }

  
  drawSprite();
}
