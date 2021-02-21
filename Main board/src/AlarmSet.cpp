#include "AlarmSet.h"
#include "AlarmManager.h"
#include "MainScreen.h"

classAlarmSet AlarmSet;

void classAlarmSet::showEntry() {
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
    Output.setSize(FONT_TIME);
    Output.flush();

    int cw = Output.getCharWidth();    
    dX = cw;
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

bool classAlarmSet::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
    return true;
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
                MainScreen.activate();
                this->deactivate();
            }
            break;
    }
}
