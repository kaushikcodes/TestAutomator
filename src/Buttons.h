#ifndef BUTTONS_H
#define BUTTONS_H
#include "Button2.h"

void Buttons_Run(void);
void ButtonAClicked(Button2 &button);
void ButtonBClicked(Button2 &button);
void Buttons_Init(void);
bool Buttons_GetPower();
#endif
