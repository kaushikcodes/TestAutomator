#include <Arduino.h>
#include "Controller.h"
#include "Parameters.h"
#include "Buttons.h"
//75 mm extended fully, 5mm/s.
//Hence, 15s to extend
const int pin1 = 18;
const int pin2 = 17;

uint16_t count = 0;
//int threshold = 300000; //CHANGE THIS VALUE TO CHANGE DEFAULT NUMBER OF CYCLES

Controller_t instance;
uint8_t currSpeed;
uint8_t currState;
bool canHear = false;

uint16_t speedDelays[] = {3500, 5500, 12000}; //Change these values for each speed and see which one fits best
enum {
    speed_2,
    speed_4,
    speed_10
};

//ON
int holdAtSpeed[] = {3000, 8000, 10000}; //change these values for the time it should hold at a particular speed for
enum {
    time_2_holdspeed,
    time_4_holdspeed,
    time_10_holdspeed
};


//OFF
int holdAtZero[] = {4800, 12000, 5000}; //change these values for the time it should hold at a ZERO for
enum {
    time_2_holdzero,
    time_4_holdzero,
    time_10_holdzero
};

// ALWAYS KEEP POWER PLUGGED IN AT THE START

void Controller_Init(tiny_timer_group_t *timerGroup)
{
    if(timerGroup != NULL)
    {
        instance._private.timerGroup = timerGroup;
    }

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
//EXTENDS ALL THE WAY IN THE START
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    delay(13500);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        8600,
        NULL,
        Controller_Extend
    );
    Serial.print("Initializing");
}

void Controller_Extend(void *context)
{
    Serial.print("Trying to extend");
    canHear = false;
    if(count == 0)
    {
        count += 1;
        Controller_Hold;
    }

    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    count += 1;
    Serial.print("Extending");
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        speedDelays[currSpeed],
        NULL,
        Controller_Hold
    );
    Serial.print("End of extension");
}


void Controller_Hold(void *context)
{
    Serial.print("Trying to hold at zero");
    canHear = false;
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);

    Serial.print("HoldingAtZero");
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        holdAtZero[currSpeed],
        NULL,
        Controller_Retract
    );
}

void Controller_Retract(void *context)
{
    Serial.print("Trying to retract");
    if(Buttons_GetPower())
    {
        canHear = false;
        Serial.print("Retracting");

        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        tiny_timer_start(
            instance._private.timerGroup,
            &instance._private.controllerTimer,
            speedDelays[currSpeed] * 1.01,
            NULL,
            Controller_HoldAtEnd
        );
    }


}

void Controller_Reset()
{
    count = 0;
    Controller_Init(NULL);

}

void Controller_HoldAtEnd(void *context)
{
    Serial.print("Trying to hold at speed");
    canHear = true;
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
    Serial.print("Holding");
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        holdAtSpeed[currSpeed],
        NULL,
        Controller_Extend
    );
}

void Controller_IncreaseSpeed()
{

    Controller_PushToZero();
    count = 1;
    if(currSpeed < 2)
    {
        currSpeed += 1;
    }
    else if(currSpeed == 2)
    {
        currSpeed = 0;
    }


}


int Controller_GetTime()
{
    return speedDelays[currSpeed];
}

int Controller_GetSetting()
{
    return (currSpeed + 1) * 2;
}

int Controller_GetCount()
{
    return count;
}

bool Controller_CanHear()
{
    return canHear;
}

void Controller_EmergencyStop()
{
    //currState = Controller_GetSetting();
    if(!Buttons_GetPower())
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH); //EXTEND
        delay(speedDelays[currSpeed]);
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
    }
    else
    {
        count = 1;
        Controller_Extend;
    }


}

void Controller_PushToZero()
{
    if(Controller_CanHear())
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH); //EXTEND
        delay(speedDelays[currSpeed]);
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
    }

}

