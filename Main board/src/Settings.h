#ifndef __SETTINGS_2__
#define __SETTINGS_2__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classSettings: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual bool onEvent(Event* pevent);

private:
    void handleKeyEvent(KeyEvent *pevent);
};

extern classSettings Settings;

#endif