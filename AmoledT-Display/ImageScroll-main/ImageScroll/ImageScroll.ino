#include "rm67162.h"
#include "TFT_eSPI.h"
#include "image.h"
TFT_eSPI tft= TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);


int imageW=800;
int imageH=500;

int screenW=536;
int screenH=240;
int m=imageW;

int start=1;
unsigned short imageS[128640]={0}; // edit this screenW * screen H
int chosen=0;
bool deb=0;
bool deb2=0;
bool led=0;

void setup() {
    pinMode(38,OUTPUT);
    pinMode(0,INPUT_PULLUP);
     pinMode(21,INPUT_PULLUP);
    
    sprite.createSprite(536,241);
    sprite.setSwapBytes(1);
    rm67162_init(); // amoled lcd initialization
    lcd_setRotation(1);
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
  m=screenW+pos;
  for(int i=0;i<screenW*screenH;i++)
  {
  if(start%m==0)
  {
  start=start+(imageW-screenW);
  m=m+imageW;
  }
  imageS[i]=picture[chosen][start];
  start++;
  
  }
  x=x+changeX;
  if(x==imageW-screenW-1 || x<0)
  changeX=changeX*-1;

   y=y+changeY;
  if(y==imageH-screenH-1 || y<0)
  changeY=changeY*-1;
  
  sprite.pushImage(0,0,screenW,screenH,imageS);
  lcd_PushColors(0, 0, 536, 240, (uint16_t*)sprite.getPointer());
  

}
