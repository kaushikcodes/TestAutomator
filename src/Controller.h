
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
int Controller_GetCount();
int Controller_GetTime();
int Controller_GetSetting();
void Controller_Reset();
void Controller_PushToZero();
bool Controller_CanHear();
void Controller_EmergencyStop();

