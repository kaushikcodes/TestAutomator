#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_ThinkInk.h>
ThinkInk_290_Grayscale4_T5* Display_GetPtr(void);

void Display_Rest(void);

void Display_RefreshFromBuffer(void);

void Display_FullRefreshFromBuffer(void);

void Display_Init(void);

void Display_Run(void);

#endif
