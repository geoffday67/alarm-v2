#include "Settings2.h"
#include "MainScreen.h"
#include "NetworkManager.h"
#include "Tune.h"

classSettings2 Settings2;

void classSettings2::activate() {
    EventManager.addListener(EVENT_KEY, this);

    Output.clear();
    Output.addTitle("Options");
    Output.addFooter("*=Back", "");
    Output.setSize(FONT_SMALL);

    int dy = Output.getCharHeight();
    int starty = dy * 6 / 5;
    int y;

    y = starty;
    Output.addText(0, y, "1 Sync time"); y += dy;
    Output.addText(0, y, "2 Tune"); y += dy;

    Output.flush();
}

void classSettings2::deactivate() {
    EventManager.removeListener(this);
}

void classSettings2::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
}

void classSettings2::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_STAR:
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_1:
            showWaiting();
            NetworkManager.syncTime();
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_2:
            Tune.activate();
            this->deactivate();
            break;
    }
}
