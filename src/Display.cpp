#include "Display.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include "Controller.h"
#include "Arduino.h"

ThinkInk_290_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, -1);
int speedy = Controller_GetTime();
int setting = Controller_GetSetting();
int counter = Controller_GetCount();
int timeAtSpeed = Controller_GetHoldAtSpeedTime();
ThinkInk_290_Grayscale4_T5* Display_GetPtr(void)
{
   return &display;
}

void Display_Rest(void)
{
   display.powerDown();
}

void Display_RefreshFromBuffer(void)
{
   Serial.println("Partial Refresh");
   display.powerUp();
   display.displayPartial(0, 0, display.width(), display.height());
}

void Display_FullRefreshFromBuffer(void)
{
   Serial.println("Full Refresh");
   display.powerUp();
   display.display();
}

void Display_Init(void)
{
   if(setting == 6)
   {
      setting = 10;
   }
   display.begin(THINKINK_MONO);
   display.clearBuffer();
   display.display();
   display.setTextColor(EPD_BLACK, EPD_LIGHT);
   display.setTextSize(3);
   display.printf("KitchenAid Mixer");
   display.setCursor(10, 120);
   display.setTextSize(1.8);
   display.printf("Count");
   display.setCursor(30, 90);
   display.setTextSize(2);
   display.print(counter);
   display.setCursor(210, 90);
   display.setTextSize(2);
   display.print(setting);
   display.setCursor(180, 120);
   display.setTextSize(1.8);
   display.printf("- Setting +");
   display.display();
}


void Display_Run(void)
{
    int speedy2 = Controller_GetTime();
    int count2 = Controller_GetCount();
    //int thresholdy2 = Controller_GetThreshold();
    int timeAtSpeedNew = Controller_GetHoldAtSpeedTime();
    if(speedy2 != speedy)
    {
        speedy = speedy2;
        setting = Controller_GetSetting();
         if(setting == 6)
         {
            setting = 10;
         }
        display.setCursor(205, 90);
        display.setTextSize(2);
        display.print(setting);
        display.print(" ");
        display.display();
    }
/*
    if(thresholdy2 != thresholdy)
    {
        thresholdy = thresholdy2;
        display.setCursor(30, 90);
        display.setTextSize(2);
        display.print(thresholdy);
        display.print(" ");
        display.display();
    }
*/

/*
   if(timeAtSpeedNew != timeAtSpeed)
    {
        timeAtSpeed = timeAtSpeedNew;
        display.setCursor(30, 90);
        display.setTextSize(2);
        display.print(timeAtSpeed);
        display.print(" ");
        display.display();
    }
*/
   if(count2 != counter)
   {
      counter = count2;
      display.setCursor(30, 90);
      display.setTextSize(2);
      display.print(counter);
      display.print(" ");
      display.display();
   }



}
