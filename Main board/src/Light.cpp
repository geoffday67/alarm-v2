#include <Arduino.h>
#include "Pins.h"
#include "Light.h"

#define DARK_LEVEL      100
#define LIGHT_LEVEL     200

classLight Light;

void classLight::loop() {
    //Serial.printf("Light level = %d\n", analogRead(LIGHT_LEVEL));

/*
When the light level is low:
Don't update the display (flag in Output).
Hide the display immediately.
If any key is pressed then light up the display.
If the alarm goes off then light up the display.
If there are no keys pressed for a timeout then dim the display.
*/

}