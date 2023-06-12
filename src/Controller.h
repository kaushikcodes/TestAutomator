
extern "C"
{
#include "tiny_timer.h"
}

typedef struct {
   struct
   {
      tiny_timer_group_t *timerGroup;
      tiny_timer_t controllerTimer;
      tiny_timer_t cycleTimer;
   }_private;
} Controller_t;


void Controller_Init(tiny_timer_group_t *timerGroup);
void Controller_Retract(void *context);
void Controller_Hold(void *context);
void Controller_HoldAtEnd(void *context);
void Controller_Extend(void *context);
void Controller_IncreaseSpeed();
void Controller_DecreaseSpeed();
void Controller_IncreaseThreshold();
void Controller_DecreaseThreshold();
int Controller_GetCount();
int Controller_GetTime();
int Controller_GetSetting();
void Controller_Reset();
int Controller_GetHoldAtSpeedTime();
void Controller_IncreaseHoldTimeAtSpeed();
void Controller_DecreaseHoldTimeAtSpeed();


