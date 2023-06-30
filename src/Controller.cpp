#include <Arduino.h>
#include "Controller.h"
#include "Parameters.h"
#include "Buttons.h"
//75 mm extended fully, 5mm/s.
//Hence, 15s to extend
const int pin1 = 18;
const int pin2 = 17;
const int switchpin = 13;
uint16_t count = 0;
uint16_t clickCount = 0;
uint16_t threshold = 2300;
Controller_t instance;
uint8_t currSpeed;
uint8_t currState;
bool canHear = false;

uint16_t speedDelaysRetract[] = {3600, 5600, 13000}; //Change these values for each speed and see which one fits best
enum {
    speed_2_r,
    speed_4_r,
    speed_10_r
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

bool ButtonIsPressed()
{
    delay(80);
    byte buttonState = digitalRead(switchpin);
    if(buttonState == HIGH)
    {
        Serial.print("Button is NOT pressed");
        return false;
    }

    Serial.print("Button IS pressed");
    return true;
}


bool extending = false;

void ExtendActuator(void *context)
{
    delay(250);
    extending = true;
    Serial.println("Extending actuator");
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, HIGH);
}

void StopActuatorAtZero()
{
    extending = false;
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
}

void RetractActuator()
{
    Serial.println("Retracting");
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
}

void StopActuatorAtSpeed()
{
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, HIGH);
}


void Controller_Init(tiny_timer_group_t *timerGroup)
{
    if(timerGroup != NULL)
    {
        instance._private.timerGroup = timerGroup;
    }

    Serial.print("Initializing");
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(switchpin, INPUT_PULLUP);

    ExtendActuator(NULL);

}


void Controller_Run(void *context)
{

    if(extending)
    {
        if(ButtonIsPressed() == true)
        {
            clickCount += 1;
            if(clickCount > 1) //CHANGE THIS TO 2 IF YOU WANT BUTTON TO BE LESS SENSITIVE AND CHANGE TO 0 IF YOU WANT IT TO BE MORE SENSITIVE
            {
                clickCount = 0;
                Serial.print("Stopping actuator at 0");
                StopActuatorAtZero();
                tiny_timer_start(
                    instance._private.timerGroup,
                    &instance._private.controllerTimer,
                    holdAtZero[currSpeed],
                    NULL,
                    Controller_Retract
                );
            }

        }

    }

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

        RetractActuator();
        tiny_timer_start(
            instance._private.timerGroup,
            &instance._private.controllerTimer,
            speedDelaysRetract[currSpeed],
            NULL,
            Controller_HoldAtEnd
        );
    }


}


void Controller_HoldAtEnd(void *context)
{
    Serial.print("Trying to hold at speed");
    canHear = true;
    StopActuatorAtSpeed();
    Serial.print("Holding");
    tiny_timer_start(
        instance._private.timerGroup,
        &instance._private.controllerTimer,
        holdAtSpeed[currSpeed],
        NULL,
        ExtendActuator
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
    return speedDelaysRetract[currSpeed];
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
        while(!ButtonIsPressed())
        {
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, HIGH); //EXTEND
        }
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
        while(!ButtonIsPressed())
        {
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, HIGH); //EXTEND
        }
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, LOW); //EXTEND
    }

}

