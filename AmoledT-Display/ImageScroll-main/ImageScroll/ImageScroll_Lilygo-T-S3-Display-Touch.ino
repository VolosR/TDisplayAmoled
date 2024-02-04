#define LILYGO_TDISPLAY_AMOLED_SERIES

#include "LilyGo_AMOLED.h"  
#include <TFT_eSPI.h>
#include "image.h"

TFT_eSPI tft= TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);
LilyGo_Class amoled;

#define WIDTH amoled.width()
#define HEIGHT amoled.height()

int imageW=800;
int imageH=500;


int m=imageW;

int start=1;
unsigned short imageS[128640]={0}; // edit this WIDTH * screen H
int chosen=0;
bool deb=0;
bool deb2=0;
bool led=0;

void setup() {
    amoled.begin(); // amoled lcd initialization
    pinMode(38,OUTPUT);
    pinMode(0,INPUT_PULLUP);
    pinMode(21,INPUT_PULLUP);
    
    sprite.createSprite(536,241);
    sprite.setSwapBytes(1);
    //lcd_setRotation(1);
}
int pos=0;
int x=0;
int y=30;
int changeX=1;
int changeY=1;
void loop() {

  if(digitalRead(0)==0)
  {if(deb==0)
    {
    deb=1;
    chosen++;
    if(chosen==3)
    chosen=0;
    }
  }else deb=0;

  if(digitalRead(21)==0)
  {if(deb2==0)
    {
    deb2=1;
    led=!led;
    digitalWrite(38,led);
    }
  }else deb2=0;

  pos=x+imageW*y;
  start=pos;
  m=WIDTH+pos;
  for(int i=0;i<WIDTH*HEIGHT;i++)
  {
  if(start%m==0)
  {
  start=start+(imageW-WIDTH);
  m=m+imageW;
  }
  imageS[i]=picture[chosen][start];
  start++;
  
  }
  x=x+changeX;
  if(x==imageW-WIDTH-1 || x<0)
  changeX=changeX*-1;

   y=y+changeY;
  if(y==imageH-HEIGHT-1 || y<0)
  changeY=changeY*-1;
  
  sprite.pushImage(0,0,WIDTH,HEIGHT,imageS);
  amoled.pushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  

}
