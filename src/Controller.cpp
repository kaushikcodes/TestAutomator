#include <Arduino.h>
#include "Controller.h"
#include "Parameters.h"
#include "Buttons.h"
//75 mm extended fully, 5mm/s.
//Hence, 15s to extend
const int pin1 = 18;
const int pin2 = 17;
uint16_t count = 0;
uint16_t threshold = 2300;
Controller_t instance;
uint8_t currSpeed;
uint8_t currState;
bool canHear = false;

uint16_t speedDelaysRetract[] = {3500, 5600, 13000}; //Change these values for each speed and see which one fits best
enum {
    speed_2_r,
    speed_4_r,
    speed_10_r
};

uint16_t speedDelaysExtend[] = {3650, 5750, 13400}; //Change these values for each speed and see which one fits best
enum {
    speed_2_e,
    speed_4_e,
    speed_10_e
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

// UNPLUG POWER WHEN NOT AT 0 AND PLUG IT BACK IN

void Controller_Init(tiny_timer_group_t *timerGroup)
{
    if(timerGroup != NULL)
    {
        instance._private.timerGroup = timerGroup;
    }

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
//RETRACTS ALL THE WAY IN THE START AND EXTENDS BACK TO 0 SO THAT WE HAVE A ZERO POINT
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
    delay(13700);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        9800,
        NULL,
        Controller_Extend
    );
    Serial.print("Initializing");
}

void Controller_Extend(void *context)
{
    Serial.print("Trying to extend");
    canHear = false;
    if(count == 0) //FOR FIRST CYCLE WE ALREADY EXTEND ALL THE WAY
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
        speedDelaysExtend[currSpeed],
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
    if(count > threshold)
    {
        delay(10000000);
    }
    if(Buttons_GetPower())
    {
        canHear = false;
        Serial.print("Retracting");

        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, LOW);
        tiny_timer_start(
            instance._private.timerGroup,
            &instance._private.controllerTimer,
            speedDelaysRetract[currSpeed],
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
    return speedDelaysExtend[currSpeed];
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

int Controller_GetThreshold()
{
    return threshold;
}

void Controller_EmergencyStop()
{
    //currState = Controller_GetSetting();
    if(!Buttons_GetPower())
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH); //EXTEND
        delay(speedDelaysExtend[currSpeed]);
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW); //EXTEND
    }
    else
    {
        count = 1;
        Controller_Retract;
    }
//once you use emergency stop, to restart cycles, plug out the mixer, press reset mode, and plug it back in

}

void Controller_PushToZero()
{
    if(Controller_CanHear())
    {
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH); //EXTEND
        delay(speedDelaysExtend[currSpeed]);
        digitalWrite(pin1, HIGH);
        digitalWrite(pin2, HIGH);
    }

}
