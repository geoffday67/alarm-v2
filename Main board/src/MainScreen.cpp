#include <Time.h>
#include "MainScreen.h"
#include "Demo.h"
#include "AlarmManager.h"
#include "AlarmActive.h"
#include "Settings.h"

classMainScreen MainScreen;

void classMainScreen::onActivate() {
    Output.clear();
    Output.addFooter("*=Alarm", "#=Set");
    Output.flush();

    currentTime = AlarmManager.getCurrentTime();
    if (currentTime > 0) {
        showTime();
    }

    EventManager.addListener(EVENT_TIME, this);
    EventManager.addListener(EVENT_KEY, this);
    EventManager.addListener(EVENT_ALARM, this);
}

void classMainScreen::onDeactivate() {
    EventManager.removeListener(this);
}

bool classMainScreen::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_TIME:
            handleTimeEvent((TimeEvent*) pevent);
            break;
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
        case EVENT_ALARM:
            handleAlarmEvent((AlarmEvent*) pevent);
            break;
    }
    return true;
}

void classMainScreen::handleTimeEvent (TimeEvent *pevent) {
    currentTime = pevent->time;
    showTime();
}

void classMainScreen::showTime() {
    tmElements_t elements;
    int hour, minute;
    char text [32];

    if (!active) {
        return;
    }

    breakTime(currentTime, elements);
    sprintf (text, "%02d:%02d", elements.Hour, elements.Minute);
    Output.setSize(FONT_TIME);
    Output.addText(CENTRED, 0, text);

    if (AlarmManager.isSnoozing()) {
        strcpy (text, " Zzz ");
    } else {
        if (AlarmManager.isEnabled()) {
            AlarmManager.getAlarm(hour, minute);
            sprintf (text, "%02d:%02d", hour, minute);
        } else {
            strcpy (text, "     ");
        }
    }
    Output.setSize(FONT_ALARM);
    Output.addText(CENTRED, 38, text);

    Output.flush();
}

void classMainScreen::toggleAlarm() {
    if (AlarmManager.isSnoozing()) {
        AlarmManager.snoozeOff();
    } else {
        if (AlarmManager.isEnabled()) {
            AlarmManager.disableAlarm();
        } else {
            AlarmManager.enableAlarm();
        }
    }

    showTime();
}

void classMainScreen::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_HASH:
            Settings.activate();
            this->deactivate();
            break;
        case KEY_STAR:
            toggleAlarm();
            break;
    }
}

void classMainScreen::handleAlarmEvent(AlarmEvent *pevent) {
    AlarmActive.activate();
    this->deactivate();
}
