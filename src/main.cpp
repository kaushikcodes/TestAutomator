#include <Arduino.h>
#include "Controller.h"
#include "Buttons.h"
#include "Display.h"
#include "tiny_time_source.h"

extern "C"
{
#include "tiny_timer.h"
}

static tiny_timer_group_t timerGroup;
static i_tiny_time_source_t *timeSource = tiny_time_source_init();

void setup()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting Setup");
  Serial.flush();
  delay(1000);
  tiny_timer_group_init(&timerGroup, timeSource);
  Display_Init();
  Buttons_Init();
  Controller_Init(&timerGroup);
  Serial.println("Finished Setup");
  delay(1000);
}

void loop()
{
  Buttons_Run();
  Controller_Run(NULL);
  tiny_timer_group_run(&timerGroup);
  Display_Run();
}
