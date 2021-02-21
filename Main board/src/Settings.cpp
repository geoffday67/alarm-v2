#include "Settings.h"
#include "Settings2.h"
#include "MainScreen.h"
#include "AlarmSet.h"
#include "WifiSettings.h"
#include "NetworkManager.h"
#include "AlarmActive.h"

classSettings Settings;

void classSettings::activate() {
    EventManager.addListener(EVENT_KEY, this);

    Output.clear();
    Output.addTitle("Options");
    Output.addFooter("*=Back", "#=More");
    Output.setSize(FONT_SMALL);

    int dy = Output.getCharHeight();
    int starty = dy * 6 / 5;
    int y;

    y = starty;
    Output.addText(0, y, "1 Alarm"); y += dy;
    Output.addText(0, y, "2 Kettle"); y += dy;

    y = starty;
    Output.addText(64, y, "3 Demo"); y += dy;
    Output.addText(64, y, "4 WiFi"); y += dy;

    Output.flush();
}

void classSettings::deactivate() {
    EventManager.removeListener(this);
}

bool classSettings::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
    return true;
}

void classSettings::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_STAR:
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_HASH:
            Settings2.activate();
            this->deactivate();
            break;

        case KEY_1:
            AlarmSet.activate();
            this->deactivate();
            break;

        case KEY_2:
            showWaiting();
            NetworkManager.kettleOn();
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_3:
            AlarmActive.activate();
            this->deactivate();
            break;

        case KEY_4:
            WifiSettings.activate();
            this->deactivate();
            break;
    }
}
