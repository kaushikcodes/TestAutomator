#include <Arduino.h>
#include "Controller.h"
#include "Parameters.h"

const int pin1 = 18;
const int pin2 = 10;
const int pwma = 33;
//bool resetIsPressed = false;

uint16_t count = 0;
uint16_t threshold = 2300; //CHANGE THIS VALUE TO CHANGE DEFAULT NUMBER OF CYCLES

Controller_t instance;
uint8_t currSpeed;
uint16_t speedDelays[] = {1550, 2000, 2500, 3400, 4000}; //Change these values for each speed and see which one fits best
enum {
    speed_2,
    speed_4,
    speed_6,
    speed_8,
    speed_10
};

// ALWAYS KEEP POWER PLUGGED IN AT THE START

void Controller_Init(tiny_timer_group_t *timerGroup)
{
    instance._private.timerGroup = timerGroup;
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
//EXTENDS ALL THE WAY IN THE START
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    delay(3000);

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
        1500,
        NULL,
        Controller_Retract
    );
}

void Controller_Retract(void *context)
{
    Serial.print("Retracting");
    count += 1;

    if(count < threshold) //COMMENT OUT THIS IF-STATEMENT IF RUNNING INFINITELY
    {
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
    if(currSpeed < 5)
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

int Controller_GetThreshold()
{
    return threshold;
}

