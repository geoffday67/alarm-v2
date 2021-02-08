#include "MainScreen.h"
#include "AlarmManager.h"
#include "Tune.h"

classTune Tune;

void classTune::activate() {
    EventManager.addListener(EVENT_KEY, this);

    Output.clear();
    Output.addTitle("Tune");
    Output.addFooter("*=Back", "");
    Output.setSize(FONT_SMALL);

    int tune = AlarmManager.getTune();

    int dy = Output.getCharHeight();
    int starty = dy * 6 / 5;
    int y;

    y = starty;

    if (tune == 1) {
        Output.addText(0, y, "1>Beautiful");
    } else {
        Output.addText(0, y, "1 Beautiful");
    }
    y += dy;
    if (tune == 2) {
        Output.addText(0, y, "2>Broken");
    } else {
        Output.addText(0, y, "2 Broken");
    }
    y += dy;

    y = starty;
    if (tune == 3) {
        Output.addText(82, y, "3>Good");
    } else {
        Output.addText(82, y, "3 Good");
    }

    Output.flush();
}

void classTune::deactivate() {
    EventManager.removeListener(this);
}

void classTune::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
}

void classTune::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_1:
            AlarmManager.setTune(1);
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_2:
            AlarmManager.setTune(2);
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_3:
            AlarmManager.setTune(3);
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_STAR:
            MainScreen.activate();
            this->deactivate();
            break;
    }
}
