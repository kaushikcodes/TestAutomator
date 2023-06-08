#include "Buttons.h"
#include <Button2.h>
#include "Parameters.h"
#include "Controller.h"
#include <Arduino.h>

Button2 btnA(PIN_BTN_A, INPUT_PULLUP);
Button2 btnB(PIN_BTN_B, INPUT_PULLUP);
Button2 btnC(PIN_BTN_C, INPUT_PULLUP);
Button2 btnD(PIN_BTN_D, INPUT_PULLUP);

void Buttons_Run(void)
{
   btnA.loop();
   btnB.loop();
   btnC.loop();
   btnD.loop();
}

void ButtonAClicked(Button2 &button)
{
    Serial.print("Button A was clicked!     ");
    Controller_DecreaseThreshold();
}

void ButtonBClicked(Button2 &button)
{
    Serial.print("Button B was clicked!      ");
    Controller_IncreaseThreshold();
}

void ButtonCClicked(Button2 &button)
{
    Serial.print("Button C was clicked!     ");
    Controller_DecreaseSpeed();
}

void ButtonDClicked(Button2 &button)
{
    Serial.print("Button D was clicked!     ");
    Controller_IncreaseSpeed();
}

void ResetMode(Button2 &button)
{
    Serial.print("Button D was long pressed!     ");
    Controller_Reset();
}

void Buttons_Init(void)
{

   btnA.begin(PIN_BTN_A, INPUT_PULLUP);
   btnB.begin(PIN_BTN_B, INPUT_PULLUP);
   btnC.begin(PIN_BTN_C, INPUT_PULLUP);
   btnD.begin(PIN_BTN_D, INPUT_PULLUP);

   btnA.setClickHandler(ButtonAClicked);
   btnB.setClickHandler(ButtonBClicked);
   btnC.setClickHandler(ButtonCClicked);
   btnD.setClickHandler(ButtonDClicked);
   btnD.setLongClickTime(1500);
   btnD.setLongClickHandler(ResetMode);
}
//LONG PRESS BUTTON D FOR 1.5 SECONDS TO RESET COUNTER FOR NO OF CYCLES
