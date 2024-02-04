#define LILYGO_TDISPLAY_AMOLED_SERIES

#include "LilyGo_AMOLED.h"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
LilyGo_Class amoled;

void setup()
{  
  amoled.begin();  // amoled lcd initialization
  //lcd_setRotation(1);
  sprite.createSprite(536, 240);
  sprite.setSwapBytes(1);
}

void draw()
{
 sprite.fillSprite(TFT_BLACK);
 sprite.drawString("Hello World",20,20,4);
 sprite.fillRect(10,100,60,60,TFT_RED);
 sprite.fillRect(80,100,60,60,TFT_GREEN);
 sprite.fillRect(150,100,60,60,TFT_BLUE);
 
 amoled.pushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
}


void loop()
{
  draw();
}
