#include "LightSetting.h"
#include "MainScreen.h"
#include "NetworkManager.h"
#include "Light.h"

classLightSetting LightSetting;

void classLightSetting::showLevel() {
    char s[32];

    sprintf(s, "%03d", Light.getLevel());
    Output.addText(CENTRED, levelY, s);
    Serial.println(s);
    Output.flush();
}

void classLightSetting::activate() {
    EventManager.addListener(EVENT_KEY, this);
    EventManager.addListener(EVENT_TIME, this);

    levelY = Output.getCharHeight() * 3 / 2;

    Output.clear();
    Output.addTitle("Light Level");
    Output.addFooter("*=Back", "");
    Output.setSize(FONT_ALARM);
    showLevel();
}

void classLightSetting::deactivate() {
    EventManager.removeListener(this);
}

bool classLightSetting::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_TIME:
            handleTimeEvent((TimeEvent*) pevent);
            break;
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
    return true;
}

void classLightSetting::handleTimeEvent(TimeEvent *pevent) {
    showLevel();
}

void classLightSetting::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_STAR:
            MainScreen.activate();
            this->deactivate();
            break;
    }
}
