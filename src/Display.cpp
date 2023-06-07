#include "Display.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include "Controller.h"
#include "Arduino.h"

ThinkInk_290_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, -1);
int speedy = Controller_GetTime();
int thresholdy = Controller_GetThreshold();
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
   display.begin(THINKINK_MONO);
   display.clearBuffer();
   display.display();
   display.setTextColor(EPD_BLACK, EPD_LIGHT);
   display.setTextSize(3);
   display.printf("KitchenAid Mixer");
   display.setCursor(25, 120);
   display.setTextSize(1.8);
   display.printf("- Cycles +");
   display.setCursor(30, 90);
   display.setTextSize(2);
   display.print(thresholdy);
   display.setCursor(210, 90);
   display.setTextSize(2);
   display.print((((speedy - 1000) / 350) * 2));
   display.setCursor(180, 120);
   display.setTextSize(1.8);
   display.printf("- Setting +");
   display.display();
}


void Display_Run(void)
{
    int speedy2 = Controller_GetTime();
    int thresholdy2 = Controller_GetThreshold();
    if(speedy2 != speedy)
    {
        speedy = speedy2;
        display.setCursor(205, 90);
        display.setTextSize(2);
        display.print((((speedy - 1000) / 350) * 2));
        display.print(" ");
        display.display();
    }

    if(thresholdy2 != thresholdy)
    {
        thresholdy = thresholdy2;
        display.setCursor(30, 90);
        display.setTextSize(2);
        display.print(thresholdy);
        display.print(" ");
        display.display();
    }
}
