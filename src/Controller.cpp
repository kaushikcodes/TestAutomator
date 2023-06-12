#include <Arduino.h>
#include "Controller.h"
#include "Parameters.h"
//75 mm extended fully, 5mm/s.
//Hence, 15s to extend
const int pin1 = 18;
const int pin2 = 10;
const int pwma = 33;
//bool resetIsPressed = false;

uint16_t count = 0;
int threshold = 300000; //CHANGE THIS VALUE TO CHANGE DEFAULT NUMBER OF CYCLES

Controller_t instance;
uint8_t currSpeed;
uint16_t speedDelays[] = {3500, 5000, 9750}; //Change these values for each speed and see which one fits best
enum {
    speed_2,
    speed_4,
    speed_10
};

uint16_t holdAtSpeed[] = {5000, 8000, 5000}; //change these values for the time it should hold at a particular speed for
enum {
    time_2_holdspeed,
    time_4_holdspeed,
    time_10_holdspeed
};

uint16_t holdAtZero[] = {8000, 12000, 500}; //change these values for the time it should hold at a ZERO for
enum {
    time_2_holdzero,
    time_4_holdzero,
    time_10_holdzero
};


//int timeTakenPerCycle = (2 * speedDelays[currSpeed]) + holdAtSpeed + holdAtZero;
//int NumberOfCycles = ((totalTime - 15000) / timeTakenPerCycle);
// ALWAYS KEEP POWER PLUGGED IN AT THE START

void Controller_Init(tiny_timer_group_t *timerGroup)
{
    instance._private.timerGroup = timerGroup;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
//EXTENDS ALL THE WAY IN THE START
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    delay(15000);

    Serial.print("Initializing");
    Controller_Extend(NULL);
}


void Controller_Extend(void *context)
{
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
    digitalWrite(pin1, HIGH); //MOTOR A ON THE LEFT
    digitalWrite(pin2, HIGH); //MOTOR A ON THE LEFT

    //int delaytime = ((((retracttime - 1000) / 350) - 1) * 500);
    Serial.print("Holding");
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
    Serial.print("Retracting");

    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        speedDelays[currSpeed],
        NULL,
        Controller_HoldAtEnd
    );



}

void Controller_Reset()
{
    count = 0;
    Controller_Init(NULL); // NEED TO CHANGE
}

void Controller_HoldAtEnd(void *context)
{

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

void Controller_IncreaseThreshold()
{
    threshold = threshold + 100;
}

void Controller_DecreaseThreshold()
{
    threshold = threshold - 100;
}

void Controller_IncreaseSpeed()
{
    if(currSpeed < 2)
    {
        currSpeed += 1;
    }
    else
    {
        currSpeed = currSpeed;
    }
}

void Controller_DecreaseSpeed()
{
    if(currSpeed > 0)
    {
        currSpeed -= 1;
    }
    else
    {
        currSpeed = currSpeed;
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

int Controller_GetHoldAtSpeedTime()
{
    return holdAtSpeed[currSpeed];
}

void Controller_IncreaseHoldTimeAtSpeed()
{
    holdAtSpeed[currSpeed] = holdAtSpeed[currSpeed] + 1000;

}

void Controller_DecreaseHoldTimeAtSpeed()
{
    holdAtSpeed[currSpeed] = holdAtSpeed[currSpeed] - 1000;
}

