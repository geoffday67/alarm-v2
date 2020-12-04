#include "AlarmSet.h"
#include "AlarmManager.h"
#include "MainScreen.h"

classAlarmSet AlarmSet;

void classAlarmSet::showEntry() {
    /*char s [6];

    s [0] = entry[0];
    s [1] = entry[1];
    s [2] = ':';
    s [3] = entry[2];
    s [4] = entry[3];
    s [5] = 0;
    Output.addText(CENTRED, 20, s);*/

    int x = startX;

    Output.setSize(FONT_TIME);
    Output.addText(x, startY, entry[0]); x += dX;
    Output.addText(x, startY, entry[1]); x += dX;
    Output.addText(x, startY, ':'); x += dX;
    Output.addText(x, startY, entry[2]); x += dX;
    Output.addText(x, startY, entry[3]); x += dX;

    Output.flush();
}

void classAlarmSet::activate() {
    EventManager.addListener(EVENT_KEY, this);

    Output.clear();
    Output.addTitle("Set Alarm");
    Output.addFooter("*=Back", "");
    Output.setSize(FONT_MEDIUM);
    Output.flush();

    int cw = Output.getCharWidth();    
    dX = cw * 3 / 2;
    startX = (Output.getDisplayWidth() - (dX * 5) - (cw  / 2)) / 2;
    startY = 21;

    index = 0;
    for (int n = 0; n < 4; n++) {
        entry[n] = '-';
    }
    showEntry();
}

void classAlarmSet::deactivate() {
    EventManager.removeListener(this);
}

void classAlarmSet::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
}

void classAlarmSet::handleKeyEvent(KeyEvent *pevent) {
    if (pevent->key >= 0 && pevent->key <= 9) {
        if (index < 4) {
            entry [index++] = '0' + pevent->key;
            showEntry();
            if (index == 4) {
                Output.addFooter("*=Back", "#=Set");
                Output.flush();
            }
        }
        return;
    }

    switch (pevent->key) {
        case KEY_STAR:
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_HASH:
            if (index == 4) {
                AlarmManager.setAlarm(
                    (entry[0] - '0') * 10 + entry[1] - '0',
                    (entry[2] - '0') * 10 + entry[3] - '0');
                AlarmManager.enableAlarm();
                MainScreen.activate();
                this->deactivate();
            }
            break;
    }
}
