#ifndef __SETTINGS__
#define __SETTINGS__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classSettings2: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual bool onEvent(Event* pevent);

private:
    void handleKeyEvent(KeyEvent *pevent);
};

extern classSettings2 Settings2;

#endif