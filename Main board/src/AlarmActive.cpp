#include "Settings.h"
#include "MainScreen.h"
#include "AlarmActive.h"
#include "AlarmManager.h"

classAlarmActive AlarmActive;

void classAlarmActive::activate() {
    Output.clear();
    Output.addTitle("Wake up!");
    Output.flush();

    AlarmManager.alarmOn();
    EventManager.addListener(EVENT_KEY, this);
}

void classAlarmActive::deactivate() {
    AlarmManager.alarmOff();
    EventManager.removeListener(this);
}

void classAlarmActive::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
}

void classAlarmActive::handleKeyEvent(KeyEvent *pevent) {
    MainScreen.activate();
    this->deactivate();
}
