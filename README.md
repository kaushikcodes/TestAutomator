# TestAutomator
For the automated testing of a mixer, I designed a device with a display and modifiable parameters that would run cycles at different speeds and settings, customizable by the user. 

In order to perform this task, I used the Adafruit Magtag 2.9 microcontroller along with its 2.9" Grayscale E-Ink WiFi Display, an Adafruit TBB6612 Stepper Motor Driver, and a linear actuator. 

First, I designed the display (as seen below) with the help of the ThinkInk library. After this, I programmed the four buttons on the bottom so that the user can change the number of cycles that they want the test to run for, as well as the setting of the mixer during a set of cycles. In order to make sure that the delay due to the controller does not affect the user's input via the buttons, I make use of a timer that would be responsible for the cycles of the controller. 

![MixerDisplay](https://github.com/kaushikcodes/TestAutomator/assets/66272646/7152e448-630a-4205-9370-5e4c6a3eace0)

With the help of this setup, once a user chooses a particular number of cycles and setting, the microcontroller sends this information over to the stepper motor 
driver, which outputs the appropriate amount of voltage to drive either a linear actuator or a DC motor (actuator in our case). This actuator has a circular mount 
on the other end which pushes and pulls the lever of the mixer as it extends and retracts respectively. 

After multiple modifications due to customer expectations, this was our initial testing setup: 

![MixerTest](https://github.com/kaushikcodes/TestAutomator/assets/66272646/93b68d08-07d1-4393-842e-dbafe13dca50)

After multiple iteration of the tests and the testing process itself, this was our final setup. We were able to test multiple units with the help of multiple microcontrollers with minimal modifications to the code. This was our final testing unit:

![MicrosoftTeams-image](https://github.com/kaushikcodes/TestAutomator/assets/66272646/9a94d2f5-bb8f-4fd8-8d1f-dde0a6ea3cda)

This was our final testing setup:

![MicrosoftTeams-image (3)](https://github.com/kaushikcodes/TestAutomator/assets/66272646/ad1472ed-a392-4344-bb4b-137b1d8aef78)
