#include "WifiSettings.h"
#include "MainScreen.h"
#include "NetworkManager.h"

classWifiSettings WifiSettings;

void classWifiSettings::activate() {
    EventManager.addListener(EVENT_KEY, this);

    Output.clear();
    Output.addTitle("WiFi");
    Output.addFooter("*=Back", "");
    Output.setSize(FONT_SMALL);

    int dy = Output.getCharHeight();
    int starty = dy * 6 / 5;
    int y;

    y = starty;
    Output.addText(0, y, "1 Wario"); y += dy;
    Output.addText(0, y, "2 HobbyHouse"); y += dy;

    y = starty;
    Output.addText(64, y, "3 Off"); y += dy;

    Output.flush();
}

void classWifiSettings::deactivate() {
    EventManager.removeListener(this);
}

void classWifiSettings::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
}

void classWifiSettings::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_1:
            NetworkManager.setSSID("Wario", "mansion1");
            NetworkManager.enable();
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_2:
            NetworkManager.setSSID("HobbyHouse", "mansion1");
            NetworkManager.enable();
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_3:
            NetworkManager.disable();
            MainScreen.activate();
            this->deactivate();
            break;

        case KEY_STAR:
            MainScreen.activate();
            this->deactivate();
            break;
    }
}
