#include <Time.h>
#include "MainScreen.h"
#include "Demo.h"
#include "AlarmManager.h"
#include "Settings.h"

classMainScreen MainScreen;

void classMainScreen::activate() {
    Output.clear();
    Output.addFooter("*=Alarm", "#=Options");
    Output.flush();

    currentTime = AlarmManager.getCurrentTime();
    if (currentTime > 0) {
        showTime();
    }

    EventManager.addListener(EVENT_TIME, this);
    EventManager.addListener(EVENT_KEY, this);
}

void classMainScreen::deactivate() {
    EventManager.removeListener(this);
}

void classMainScreen::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_TIME:
            handleTimeEvent((TimeEvent*) pevent);
            break;
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
}

void classMainScreen::handleTimeEvent (TimeEvent *pevent) {
    currentTime = pevent->time;
    showTime();
}

void classMainScreen::showTime() {
    tmElements_t elements;
    char text [32];

    breakTime(currentTime, elements);
    sprintf (text, "%02d:%02d:%02d", elements.Hour, elements.Minute, elements.Second);
    Output.setSize(FONT_TIME);
    Output.addText(CENTRED, 0, text);

    if (AlarmManager.isAlarmSet()) {
        breakTime(AlarmManager.getAlarmTime(), elements);
        sprintf (text, "%02d:%02d", elements.Hour, elements.Minute);
    } else {
        strcpy (text, "     ");
    }
    Output.setSize(FONT_ALARM);
    Output.addText(CENTRED, 30, text);

    Output.flush();
}

void classMainScreen::toggleAlarm() {
    if (AlarmManager.isAlarmSet()) {
        AlarmManager.disableAlarm();
    } else {
        AlarmManager.enableAlarm();
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
        case KEY_0:
            Demo.activate();
            this->deactivate();
            break;
    }
}
