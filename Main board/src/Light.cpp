#include <Arduino.h>
#include "Pins.h"
#include "Light.h"
#include "Output.h"

#define DARK_THRESHOLD      300
#define LIGHT_THRESHOLD     600

classLight Light;

void classLight::initialise() {
    EventManager.addListener(EVENT_KEY, this);
    EventManager.addListener(EVENT_INACTIVITY, this);

    int light = analogRead(LIGHT_LEVEL);
    Serial.printf("Initial light level = %d\n", light);
    isDark = light >= LIGHT_THRESHOLD;
    keypadActive = true;
    stateChanged = true;
    processState();
}

bool classLight::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            if (!keypadActive) {
                keypadActive = true;
                stateChanged = true;
                if (isDark) {
                    return false;
                }
            }
            break;
        case EVENT_INACTIVITY:
            if (keypadActive) {
                keypadActive = false;
                stateChanged = true;
            }
            break;
    }
    return true;
}

void classLight::loop() {
    int light = analogRead(LIGHT_LEVEL);

    if (isDark) {
        if (light >= LIGHT_THRESHOLD) {
            isDark = false;
            stateChanged = true;
        }
    } else {
        if (light <= DARK_THRESHOLD) {
            isDark = true;
            stateChanged = true;
        }
    }

    processState();
}

void classLight::processState() {
    if (!stateChanged) {
        return;
    }

    Serial.printf("Light state: %s, %s\n",
        isDark ? "dark" : "light",
        keypadActive ? "active" : "inactive");

    // If it's light then always show the display.
    if (!isDark) {
        Output.enable(true);
        goto Exit;
    }

    // It's dark, whether to show the display depends on whether the keyboard is active.
    Output.enable(keypadActive);

Exit:
    stateChanged = false;
}