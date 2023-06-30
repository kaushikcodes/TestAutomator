#include "Display.h"
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include "Controller.h"
#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "Parameters.h"
#include "Buttons.h"
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
int speedy = Controller_GetTime();
int setting = Controller_GetSetting();
int counter = Controller_GetCount();
bool power = Buttons_GetPower();
int cycles = Controller_GetThreshold();

void Display_Init(void)
{
   if(setting == 6)
   {
      setting = 10;
   }
   pinMode(TFT_BACKLITE, OUTPUT);
   digitalWrite(TFT_BACKLITE, HIGH);

   pinMode(TFT_I2C_POWER, OUTPUT);
   digitalWrite(TFT_I2C_POWER, HIGH);


   display.init(135, 240);
   display.fillScreen(ST77XX_BLACK);
   display.setTextColor(ST77XX_WHITE);

   display.setTextSize(2.3);
   display.setCursor(5, 5);
   display.printf("KitchenAid Mixer");

   display.setCursor(5, 150); //was 220
   display.setTextSize(1.8);
   display.printf("Count");

   display.setCursor(10, 120); //was 195
   display.setTextSize(2);
   display.drawRect(8, 115, 46, 23, ST77XX_BLUE);
   display.fillRect(8, 115, 46, 23, ST77XX_BLUE);
   display.print(counter);

   display.setCursor(5, 220); //was 220
   display.setTextSize(1.8);
   display.printf("Power");

   display.setCursor(80, 150); //was 220
   display.setTextSize(1.8);
   display.printf("Cycles");

   display.setCursor(90, 120); //was 195
   display.setTextSize(2);
   display.drawRect(70, 115, 53, 23, ST77XX_BLUE);
   display.fillRect(70, 115, 53, 23, ST77XX_BLUE);
   display.setCursor(77, 120);
   display.print(cycles);

   display.setCursor(10, 195); //was 195
   display.setTextSize(2);
   display.drawRect(8, 190, 46, 23, ST77XX_RED);
   display.fillRect(8, 190, 46, 23, ST77XX_RED);
   display.printf("ON");

   display.setCursor(90, 195);
   display.setTextSize(2.3);
   display.drawRect(73, 190, 46, 23, ST77XX_BLUE);
   display.fillRect(73, 190, 46, 23, ST77XX_BLUE);
   display.print(setting);


   display.setCursor(60, 220);
   display.setTextSize(1.9);
   display.printf("- Setting +");

}


void Display_Run(void)
{

   int speedy2 = Controller_GetTime();
   int count2 = Controller_GetCount();
   bool power2 = Buttons_GetPower();

   if(speedy2 != speedy)
   {
      speedy = speedy2;
      setting = Controller_GetSetting();
      if(setting == 6)
      {
         setting = 10;
      }
      display.drawRect(73, 190, 46, 23, ST77XX_BLUE);
      display.fillRect(73, 190, 46, 23, ST77XX_BLUE);
      display.setCursor(90, 195);
      display.setTextSize(2);
      display.print(setting);
      display.print(" ");
   }


   if(count2 != counter)

   {
      counter = count2;
      display.drawRect(8, 115, 46, 23, ST77XX_BLUE);
      display.fillRect(8, 115, 46, 23, ST77XX_BLUE);
      display.setCursor(10, 120);
      display.setTextSize(2);
      display.print(counter);
      display.print(" ");

   }

   if(power != power2)
   {
      power = power2;
      display.drawRect(8, 190, 46, 23, ST77XX_RED);
      display.fillRect(8, 190, 46, 23, ST77XX_RED);
      display.setCursor(10, 195);
      display.setTextSize(2);
      if(power)
      {
         display.printf("ON");
      }
      else
      {
         display.printf("OFF");
      }
      display.print(" ");
   }

}
