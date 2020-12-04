#include "Demo.h"
#include "MainScreen.h"

classDemo Demo;

void classDemo::activate() {
    EventManager.addListener(EVENT_KEY, this);

    Output.clear();
    Output.addText(10, 10, "Demo");
    Output.flush();
}

void classDemo::deactivate() {
    EventManager.removeListener(this);
}

void classDemo::onEvent(Event* pevent) {
    switch (pevent->type) {
        case EVENT_KEY:
            handleKeyEvent((KeyEvent*) pevent);
            break;
    }
}

void classDemo::handleKeyEvent(KeyEvent *pevent) {
    switch (pevent->key) {
        case KEY_STAR:
            MainScreen.activate();
            this->deactivate();
            break;
    }
}
