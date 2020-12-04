#ifndef __DEMO__
#define __DEMO__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classDemo: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual void onEvent(Event* pevent);

private:
    void handleKeyEvent(KeyEvent *pevent);
};

extern classDemo Demo;

#endif