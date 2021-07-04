#include "Settings.h"
#include "MainScreen.h"
#include "AlarmActive.h"
#include "AlarmManager.h"
#include "sunrise.xbm"

classAlarmActive AlarmActive;

void classAlarmActive::showScreen() {
    Output.clear();
    Output.drawBitmap(sunrise_bits);
    Output.setSize(FONT_SMALL);
    Output.addText(0, 0, muted ? "*=Off" : "*=Shh");
    Output.addText(100, 0, "#=Zz");
    Output.flush();
}

void classAlarmActive::activate() {
    muted = false;
    showScreen();
    AlarmManager.alarmOn();
    EventManager.addListener(EVENT_KEY, this);
}

void classAlarmActive::deactivate() {
    EventManager.removeListener(this);
}

bool classAlarmActive::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
    return true;
}

void classAlarmActive::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_HASH:
            AlarmManager.alarmSnooze(5);
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_STAR:
            if (muted) {
                AlarmManager.alarmOff();
                MainScreen.activate();
                this->deactivate();
            } else {
                AlarmManager.muteAlarm();
                muted = true;
                showScreen();
            }
            break;
    }
}
