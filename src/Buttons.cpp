#include "Buttons.h"
#include <Button2.h>
#include "Parameters.h"
#include "Controller.h"
#include <Arduino.h>

Button2 btnA(PIN_BTN_A, INPUT_PULLDOWN, false);
Button2 btnB(PIN_BTN_B, INPUT_PULLDOWN, false);
Button2 btnC(PIN_BTN_C, INPUT_PULLUP, true);
bool PowerOn = true;

void Buttons_Run(void)
{
   btnA.loop();
   btnB.loop();
   btnC.loop();
}


void ButtonAClicked(Button2 &button)
{
    Serial.print("Button A was clicked!     ");
    //Controller_DecreaseSpeed();
}

void ButtonBClicked(Button2 &button)
{
    Serial.print("Button B was clicked!     ");
    Controller_IncreaseSpeed();
}

void ButtonCClicked(Button2 &button)
{
    Serial.print("Button C was clicked!     ");
    //Controller_DecreaseSpeed();
    PowerOn = !PowerOn;
    Controller_EmergencyStop(); //EXTEND ALL THE WAY BY EXACT AMOUNT
}


void Buttons_Init(void)
{

   btnA.begin(PIN_BTN_A, INPUT_PULLDOWN, false);
   btnB.begin(PIN_BTN_B, INPUT_PULLDOWN, false);
   btnC.begin(PIN_BTN_C, INPUT_PULLUP, true);

   btnA.setClickHandler(ButtonAClicked);
   btnB.setClickHandler(ButtonBClicked);
   btnC.setClickHandler(ButtonCClicked);
   btnC.setLongClickTime(3000);
   btnC.setLongClickHandler(ResetMode);
}

bool Buttons_GetPower()
{
    return PowerOn;
}
//LONG PRESS BUTTON c FOR 1.5 SECONDS TO RESET COUNTER FOR NO OF CYCLES
