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

    char ssid[32];
    NetworkManager.getSSID(ssid);
    bool enabled = NetworkManager.getEnabled();

    int dy = Output.getCharHeight();
    int starty = dy * 6 / 5;
    int y;

    y = starty;

    if (enabled && !strcmp(ssid, "Wario")) {
        Output.addText(0, y, "1>Wario");
    } else {
        Output.addText(0, y, "1 Wario");
    }
    y += dy;
    if (enabled && !strcmp(ssid, "HobbyHouse")) {
        Output.addText(0, y, "2>HobbyHouse");
    } else {
        Output.addText(0, y, "2 HobbyHouse");
    }
    y += dy;

    y = starty;
    if (!enabled) {
        Output.addText(64, y, "3>Off");
    } else {
        Output.addText(64, y, "3 Off");
    }

    Output.flush();
}

void classWifiSettings::deactivate() {
    EventManager.removeListener(this);
}

bool classWifiSettings::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
    return true;
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
