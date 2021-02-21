#ifndef __ALARM_ACTIVE__
#define __ALARM_ACTIVE__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classAlarmActive: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual bool onEvent(Event* pevent);

private:
    void handleKeyEvent(KeyEvent *pevent);
};

extern classAlarmActive AlarmActive;

#endif