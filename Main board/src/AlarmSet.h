#ifndef __ALARM_SET__
#define __ALARM_SET__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classAlarmSet: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual bool onEvent(Event* pevent);

private:
    int index, startX, dX, startY;
    char entry[4];
    void showEntry();
    void handleKeyEvent(KeyEvent *pevent);
};

extern classAlarmSet AlarmSet;

#endif