#ifndef __LIGHT_SETTING__
#define __LIGHT_SETTING__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classLightSetting: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual bool onEvent(Event* pevent);

private:
    int levelY;
    void showLevel();
    void handleTimeEvent (TimeEvent *pevent);
    void handleKeyEvent (KeyEvent *pevent);
};

extern classLightSetting LightSetting;

#endif