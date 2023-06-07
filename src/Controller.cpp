#include <Arduino.h>
#include "Controller.h"
#include "Parameters.h"

const int pin1 = 18;
const int pin2 = 10;
const int pwma = 33;

const int extendtime = 3000;
uint16_t retracttime = 1700;

uint16_t count = 0;
uint16_t threshold = 2300;

Controller_t instance;


void Controller_Init(tiny_timer_group_t *timerGroup)
{
    instance._private.timerGroup = timerGroup;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);

    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);

    Serial.print("Initializing");
    Controller_Extend(NULL);
}


void Controller_Extend(void *context)
{
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);

    Serial.print("Extending");
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        extendtime,
        NULL,
        Controller_Hold
    );
    Serial.print("End of extension");
}


void Controller_Hold(void *context)
{
    digitalWrite(pin1, HIGH); //MOTOR A ON THE LEFT
    digitalWrite(pin2, HIGH); //MOTOR A ON THE LEFT

    int delaytime = ((((retracttime - 1000) / 350) - 1) * 500);
    Serial.print("Holding");
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        delaytime,
        NULL,
        Controller_Retract
    );
}

void Controller_Retract(void *context)
{
    Serial.print("Retracting");
    if(count > threshold)
    {
        delay(10000000);
    }
    count += 1;
    digitalWrite(pin1, HIGH); //MOTOR A ON THE LEFT
    digitalWrite(pin2, LOW); //MOTOR A ON THE LEFT
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        retracttime,
        NULL,
        Controller_HoldAtEnd
    );
}

void Controller_HoldAtEnd(void *context)
{

    digitalWrite(pin1, HIGH); //MOTOR A ON THE LEFT
    digitalWrite(pin2, HIGH); //MOTOR A ON THE LEFT
    Serial.print("Holding");
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        2000,
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
    if(retracttime < 2800)
    {
        retracttime = retracttime + 350;
    }
    else
    {
        retracttime = retracttime;
    }
}

void Controller_DecreaseSpeed()
{
    if(retracttime > 1351)
    {
        retracttime = retracttime - 350;
    }
    else
    {
        retracttime = retracttime;
    }
}

int Controller_GetTime()
{
    return retracttime;
}

int Controller_GetThreshold()
{
    return threshold;
}
